#ifndef ZERO_ADAPTER
#define ZERO_ADAPTER

#include <iostream>
#include <cstring>
#include <zmq.hpp>
#include <string>

#include "AdapterInterface.h"

using namespace std;

namespace Benchmark {
    class ZMQAdapter: public Benchmark::AdapterInterface {
    private:
        zmq::context_t context;
        zmq::socket_t* socket{};
        std::vector<zmq::pollitem_t> items = {{socket, 0, ZMQ_POLLIN, 0}};
    public:
        ZMQAdapter(){}

        void init_sender(Benchmark::Config config) override {
            this->socket =  new zmq::socket_t(context, zmq::socket_type::push);
            std::string connection_string = config.get_zeromq_ip() + ":" + std::to_string(config.get_zeromq_port());
            this->socket->connect(connection_string);
        }

        void init_receiver(Benchmark::Config config) override {
            this->socket = new zmq::socket_t(context,zmq::socket_type::pull);
            std::string bind_string = config.get_bind_ip() + ":" + std::to_string(config.get_zeromq_port());
            this->socket->bind(bind_string);
        }

        void send_data(char* data, int message_size) override {
            zmq::message_t message(message_size);
            memcpy(message.data(),data,message_size);
            this->socket->send(message, zmq::send_flags::dontwait);
        }

        int receive_data(char &data) override {
            zmq::message_t message;
            auto recv = this->socket->recv(message);
            ::memcpy(&data,message.data(),recv.value());
            return recv.value();
        }
    };
}

#endif