#ifndef CONFIG_H
#define CONFIG_H


#include <iostream>
#include <fstream>
#include "include/json.hpp"

namespace Benchmark {
    class Config {
    private:
        // type of adapter to use for sending and receiving data to/from the SPS
        // currently supports tcp, udp, kafka, zeromq
        std::string adapter;
        
        // if true the system measures the latency for all tuples
        // ATTENTION: setting this value to true will dramatically decrease the throughput of the benchmark
        bool latency;

        // query id that is benchmarked (1 - 10)
        int queryId = 1;
        
        // name of the file containing the states
        std::string input_file;

        // size of the dataframe sent
        // the benchmark packs as many tuples a possible in on frame
        int data_frame_size;

        // ip to bind the benchmark to, if multiple NICS are available
        std::string bind_ip;

        // port to listen for incomming data from the SPS
        int bind_port;

        /**
         * TCP Config
         */
         // IP of the SPS to send the states to
        std::string tcp_ip;
        // Port of the SPS
        int tcp_port;

        /**
         * UDP Config
         */
         // IP of the SPS to send the states to
        std::string udp_ip;
        // Port of the SPS
        int udp_port;

        /**
         * Kafka Config
         */
         // Kafka Broker and Port
        std::string broker;
        // Topic the benchmarking tool should send the states so
        std::string producer_topic;
        // Consumer topic the benchmarking tool should poll from to receive the result tuples
        std::string consumer_topic;

        /**
         * ZERO MQ
         */
        // IP of the ZeroMQ Broker
        std::string zeromq_ip;
        // Port of the ZeroMQ Broker
        int zeromq_port;
    public:

        Config(){}

        Config(std::string filepath) {
            std::ifstream f(filepath);
            nlohmann::json data;
            f >> data;

            // BASE
            this->adapter = data["base"]["adapter"];
            this->latency = data["base"]["latency"];
            this->queryId = data["base"]["queryId"];
            this->input_file = data["base"]["input_file"];
            this->data_frame_size = data["base"]["data_frame_size"];
            this->bind_ip = data["base"]["bind_ip"];
            this->bind_port = data["base"]["bind_port"];

            // TCP
            this->tcp_ip = data["tcp"]["ip"];
            this->tcp_port = data["tcp"]["port"];

            // UDP
            this->udp_ip = data["udp"]["ip"];
            this->udp_port = data["udp"]["port"];

            // KAFKA
            this->broker = data["kafka"]["broker"];
            this->producer_topic = data["kafka"]["producer_topic"];
            this->consumer_topic = data["kafka"]["consumer_topic"];

            // ZEROMQ
            this->zeromq_ip = data["zeromq"]["ip"];
            this->zeromq_port = data["zeromq"]["port"];
        }

        std::string get_bind_ip() {return this->bind_ip;}

        int get_bind_port(){return this->bind_port;}

        bool get_latency(){return this->latency;}

        int get_query_id(){return this->queryId;}

        void set_query_id(int queryId){this->queryId = queryId;}

        std::string get_zeromq_ip(){return this->zeromq_ip;}

        int get_zeromq_port(){return this->zeromq_port;}

        std::string get_udp_ip(){return this->udp_ip;}

        int get_udp_port(){return this->udp_port;}

        std::string get_input_file(){return this->input_file;}

        std::string  get_adapter() { return this->adapter; }

        std::string  get_tcp_ip() { return this->tcp_ip; }

        int  get_tcp_port() { return this->tcp_port; }

        std::string  get_kafka_broker() { return this->broker; }

        std::string get_kafka_producer_topic() { return this->producer_topic; }

        std::string get_kafka_consumer_topic() { return this->producer_topic; }

        int get_dataframe_size(){return this->data_frame_size;}
    };
}
#endif