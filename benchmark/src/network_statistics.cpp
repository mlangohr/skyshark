#ifndef NETSTAT_CPP
#define NETSTAT_CPP

#include <iostream>
#include <fstream>
#include <atomic>
#include <chrono>
#include <string>

#include "config.h"
#include "query_results.cpp"

namespace fs = std::filesystem;

namespace Benchmark {
    class NetworkStatistics {
    private:
        std::atomic_uint64_t sent_tuples;
        std::atomic_uint64_t sent_bytes;
        std::atomic_uint64_t received_tuples{0};
        std::atomic_uint64_t received_bytes{0};
        double avg_latency{0.0};
        std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>  benchmark_start;
        std::ofstream result;

    public:
        NetworkStatistics(){}

        NetworkStatistics(Benchmark::Config config) {
            sent_tuples = 0;
            sent_bytes = 0;
            received_bytes = 0;
            received_tuples = 0;
            result = std::ofstream("../results/query" + std::to_string(config.get_query_id()) + "/result.bin", std::ios::binary | std::ios::out);
        }


        template<typename T, typename= std::enable_if_t<std::is_base_of<QueryResult,T>::value>>
        void write_result(T item){
            result.write(reinterpret_cast<const char*>(&item),sizeof(item));
        }

        void write_raw_result(const char* value, int size){
            result.write(value,size);
        }

        void inc_sent_tuples() {
            ++sent_tuples;
        }

        void add_sent_bytes(uint64_t bytes) {
            sent_bytes += bytes;
        }

        void inc_received_tuples() {
            ++received_tuples;
        }

        void inc_received_tuples(int tuples){
            received_tuples += tuples;
        }

        void add_received_bytes(uint64_t bytes) {
            received_bytes += bytes;
        }


        void start_stop_watch() {
            benchmark_start = std::chrono::system_clock::now();
        }

        uint64_t get_runtime() {
            std::chrono::time_point now = std::chrono::system_clock::now();
            std::chrono::duration<double> diff = now - benchmark_start;
            return diff.count();
        }

        double get_mb_sent() {
            return static_cast<double>(sent_bytes) / 1048576;
        }

        double get_output_throughput(){
            return get_mb_sent() / get_runtime();
        }

        double get_mb_received() {
            return static_cast<double>(received_bytes) / 1048576;
        }

        double get_input_throughput(){
            return get_mb_received() / get_runtime();
        }

        int get_tuples_per_second_sent(){
            return get_sent_tuples() / get_runtime();
        }

        int get_tuples_per_second_received() {
            return get_received_tuples() / get_runtime();
        }

        uint64_t get_sent_tuples() const {
            return sent_tuples;
        }

        uint64_t get_sent_bytes() const {
            return sent_bytes;
        }

        uint64_t get_received_tuples() const {
            return received_tuples;
        }

        uint64_t get_received_bytes() const {
            return received_bytes;
        }

        double  get_latency(){
            return this->avg_latency;
        }

        void add_latency(double latency){
            if (latency < 1000000){
                this->avg_latency = ((avg_latency * (this->received_tuples - 1)) + latency) / this->received_tuples;
            }

        }

        void close_stats(){
            try {
                this->result.close();
            } catch (exception e){
                std::cerr << "Could not close file (result,in_time,out_time)" << std::endl;
            }

        }
    };
}

#endif