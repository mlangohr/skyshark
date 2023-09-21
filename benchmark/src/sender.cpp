#include <iostream>

#include "config.h"
#include "network_statistics.cpp"
#include "data_loader.cpp"
#include "state.cpp"

// Adapter
#include "adapter/KafkaAdapter.cpp"
#include "adapter/TCPAdapter.cpp"
#include "adapter/UDPAdapter.cpp"
#include "adapter/ZeroMQAdapter.cpp"


namespace Benchmark {
    class Sender {
    private:
        Benchmark::Config config;
        Benchmark::NetworkStatistics* stats;
        Benchmark::DataLoader* dataLoader;
        Benchmark::AdapterInterface* sender;

        int state_size = sizeof(Benchmark::State);
        int states_per_frame;
        int buffer_size;
    public:
        Sender(){}
        Sender(Benchmark::Config config, Benchmark::NetworkStatistics* stats) {
            this->config = config;
            this->stats = stats;
            this->dataLoader = new DataLoader(config);
            this->states_per_frame = config.get_dataframe_size() / state_size;
            this->sender = get_interface(config);
            this->sender->init_sender(config);
            this->buffer_size = this->state_size * this->states_per_frame;
        }

        Benchmark::AdapterInterface* get_interface(Benchmark::Config config){
            Benchmark::AdapterInterface* interface;
            if (config.get_adapter() == "tcp"){
                interface = new Benchmark::TCPAdapter();
            } else if (config.get_adapter() == "kafka"){
                interface = new Benchmark::KafkaAdapter();
            } else if (config.get_adapter() == "udp") {
                interface = new Benchmark::UDPAdapter();
            } else if (config.get_adapter() == "zeromq") {
                interface = new Benchmark::ZMQAdapter();
            }else {
                throw new Exception("Adapter not defined or wrong");
            }
            return interface;
        }

        bool send_data(){
            int data_counter = 0;
            char send_buffer[this->buffer_size];
            while (data_counter < this->states_per_frame){
                auto opt_state  = this->dataLoader->get();
                if (opt_state.has_value()){
                    Benchmark::State state = opt_state.value();
                    ::memcpy(send_buffer + (data_counter * state_size),reinterpret_cast<const char*>(&state),state_size);
                    data_counter++;
                    stats->inc_sent_tuples();
                } else {
                    stats->add_sent_bytes(data_counter * state_size);
                    sender->send_data(send_buffer,sizeof(send_buffer));
                    return false;
                }
            }
            stats->add_sent_bytes(data_counter * state_size);
            sender->send_data(send_buffer, data_counter * state_size);
            data_counter = 0;
            return true;
        }

        bool send_data_latency(){
            int data_counter = 0;
            char send_buffer[this->buffer_size];
            std::chrono::time_point now = std::chrono::system_clock::now();
            ::uint64_t timestamp = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
            while (data_counter < this->states_per_frame){
                auto opt_state  = this->dataLoader->get();
                if (opt_state.has_value()){
                    Benchmark::State state = opt_state.value();
                    state.id = timestamp;
                    ::memcpy(send_buffer + (data_counter * state_size),reinterpret_cast<const char*>(&state),state_size);
                    data_counter++;
                    stats->inc_sent_tuples();
                } else {
                    stats->add_sent_bytes(data_counter * state_size);
                    sender->send_data(send_buffer,sizeof(send_buffer));
                    return false;
                }
            }
            stats->add_sent_bytes(data_counter * state_size);
            sender->send_data(send_buffer, data_counter * state_size);
            data_counter = 0;
            return true;
        }

        void close_file(){
            try {
                this->dataLoader->close_file();
            } catch (exception e){
                std::cerr << "Could not close sender file" << std::endl;
            }

        }

    };
}