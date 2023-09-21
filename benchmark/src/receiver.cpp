#include <iostream>
#include <thread>

#include "config.h"
#include "network_statistics.cpp"
#include "query_results.cpp"
#include "state.cpp"
#include "ring_buffer.cpp"

// Adapter
#include "adapter/KafkaAdapter.cpp"
#include "adapter/TCPAdapter.cpp"
#include "adapter/UDPAdapter.cpp"
#include "adapter/ZeroMQAdapter.cpp"

namespace Benchmark {
    template<typename T, typename = std::enable_if_t<std::is_base_of<QueryResult,T>::value>>
    class Receiver {
    private:
        Benchmark::Config config;
        Benchmark::NetworkStatistics* stats;
        Benchmark::AdapterInterface* receiver;
    public:
        Receiver(){}

        Receiver(Benchmark::Config config, Benchmark::NetworkStatistics* stats) {
            this->config = config;
            this->stats = stats;
            this->receiver = get_interface(config);
            this->receiver->init_receiver(config);
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
        void create_and_store_timestamp(::uint64_t  out_time, ::uint64_t in_time){
            double  time_diff = (in_time - out_time);
            stats->add_latency(time_diff);
        }


        bool receive_data(){
            char data[config.get_dataframe_size()];
            int received_bytes = receiver->receive_data(reinterpret_cast<char &>(data));
            int states_per_package = received_bytes / sizeof(T);
            if (received_bytes > 0){
                stats->add_received_bytes(received_bytes);
                if (config.get_latency()){
                    T result;
                    for (int i=0;i<states_per_package;i++){
                        ::size_t pointer = i * sizeof(T);
                        ::memcpy(&result,data + pointer,sizeof(T));
                        stats->inc_received_tuples();
                        std::chrono::time_point now = std::chrono::system_clock::now();
                        ::uint64_t timestamp = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
                        create_and_store_timestamp(result.get_id(),timestamp);
                        stats->write_result<T>(static_cast<T>(result));
                    }
                    return true;
                } else {
                    stats->inc_received_tuples(states_per_package);
                    stats->write_raw_result(data,states_per_package * sizeof(T));
                }
            }
            return false;
        }
    };
}