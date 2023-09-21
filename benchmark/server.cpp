#ifndef SERVER_CPP
#define SERVER_CPP

#include <iostream>
#include <thread>
#include <chrono>
#include <signal.h>
#include <atomic>
#include <array>
#include <filesystem>

#include "src/include/CLI11.hpp"

#include "src/config.h"
#include "src/network_statistics.cpp"
#include "src/query_results.cpp"

#include "src/sender.cpp"
#include "src/receiver.cpp"

namespace fs = std::filesystem;

//+++++++++++++++++++++++++++++++++++++++++++++++

int query = 1;

// Threads
std::thread* sendData;
std::atomic<bool> stop_send_data(true);

std::thread* receiveData;
std::atomic<bool> stop_receive_data(true);

std::atomic<bool> stopFlag = true;

// Statistics and Config
Benchmark::Config config;
Benchmark::NetworkStatistics* stats;

void handle_stop(int signum);


void print_stats() {
    std::cout << "####################################### Result Stats (Query " + ::to_string(query) + ") ########################################" << std::endl;
    std::cout << "tuples sent: " << stats->get_sent_tuples() << "         tuples received: " << stats->get_received_tuples() << "      total runtime (s): " << stats->get_runtime() << std::endl;
    std::cout << "tuples sent/second: " << stats->get_tuples_per_second_sent() << "    tuples received/second: " << stats->get_tuples_per_second_received() << std::endl;
    std::cout << "average latency (microseconds): " << std::to_string(stats->get_latency()) << std::endl;
    std::cout << "------------------------------ Throughput -------------------------------------" << std::endl;
    std::cout << "MB   sent: " << stats->get_mb_sent() << "            MB received: " << stats->get_mb_received() <<  std::endl;
    std::cout << "MB/s sent: " << stats->get_output_throughput() << "            MB/s received: " << stats->get_input_throughput() <<  std::endl;
    std::cout << "####################################### Result Stats (Query " + ::to_string(query) + ") ########################################" << std::endl;
}


/**
 * Send the tuples to the SPS
 * @param config
 */
void send_data(Benchmark::Config &config) {
    Benchmark::Sender sender(config,stats);
    cout << "starting process send_data [x]" << std::endl;
    stats->start_stop_watch();
    if (config.get_latency()){
        while(stop_send_data){
            if (!sender.send_data_latency()){
                sender.close_file();
                break;
            }
        }
    }else {
        while(stop_receive_data){
            if (!sender.send_data()){
                sender.close_file();
                break;
            }
        }
    }
    cout << "stopped process send_data [x]" << std::endl;
    sender.close_file();
    handle_stop(0);
}

/**
 * Specific function for each query
 * @param config
 */
void receive_q1(Benchmark::Config config) {
    Benchmark::Receiver<Benchmark::Q1> receiver(config,stats);
    while(receiver.receive_data() ||stop_receive_data  ){}
}

void receive_q2(Benchmark::Config config) {
    Benchmark::Receiver<Benchmark::Q2> receiver(config,stats);
    while(receiver.receive_data() ||stop_receive_data  ){}
}

void receive_q3(Benchmark::Config config) {
    Benchmark::Receiver<Benchmark::Q3> receiver(config,stats);
    while(receiver.receive_data() ||stop_receive_data  ){}
}
void receive_q4(Benchmark::Config config) {
    Benchmark::Receiver<Benchmark::Q4> receiver(config,stats);
    while(receiver.receive_data() ||stop_receive_data  ){}
}

void receive_q5(Benchmark::Config config) {
    Benchmark::Receiver<Benchmark::Q5> receiver(config,stats);
    while(receiver.receive_data() ||stop_receive_data  ){}
}

void receive_q6(Benchmark::Config config) {
    Benchmark::Receiver<Benchmark::Q6> receiver(config,stats);
    while(receiver.receive_data() ||stop_receive_data  ){}
}

void receive_q7(Benchmark::Config config) {
    Benchmark::Receiver<Benchmark::Q7> receiver(config,stats);
    while(receiver.receive_data() ||stop_receive_data  ){}
}

void receive_q8(Benchmark::Config config) {
    Benchmark::Receiver<Benchmark::Q8> receiver(config,stats);
    while(receiver.receive_data() ||stop_receive_data  ){}
}

void receive_q9(Benchmark::Config config) {
    Benchmark::Receiver<Benchmark::Q9> receiver(config,stats);
    while(receiver.receive_data() ||stop_receive_data  ){}
}

void receive_q10(Benchmark::Config config) {
    Benchmark::Receiver<Benchmark::Q10> receiver(config,stats);
    while(receiver.receive_data() ||stop_receive_data  ){}
}

/**
 * Receives data from the SPS and processes the message
 * in_timestamps
 * @param config
 */
void receive_data(Benchmark::Config &config) {
    cout << "starting process receive_data [x]" << std::endl;
    switch(config.get_query_id()){
        case 1:
            receive_q1(config);
            break;
        case 2:
            receive_q2(config);
            break;
        case 3:
            receive_q3(config);
            break;
        case 4:
            receive_q4(config);
            break;
        case 5:
            receive_q5(config);
            break;
        case 6:
            receive_q6(config);
            break;
        case 7:
            receive_q7(config);
            break;
        case 8:
            receive_q8(config);
            break;
        case 9:
            receive_q9(config);
            break;
        case 10:
            receive_q10(config);
            break;
    }
    cout << "stopped process receive_data [x]" << std::endl;
    handle_stop(0);
}

bool folder_exists(const fs::path& path) {
return fs::exists(path);
}

bool delete_folder(const fs::path& path) {
    try {
        fs::remove_all(path);
        return true;
    } catch (const fs::filesystem_error& e) {
        std::cerr << e.what() << '\n';
        return false;
    }
}

bool create_folder(const fs::path& path) {
    try {
        return fs::create_directories(path);
    } catch (const fs::filesystem_error& e) {
        std::cerr << e.what() << '\n';
        return false;
    }
}

/**
 * Called when the user uses ctrl + c to stop the program or when we have no more tuples to send
 * @param signum
 */
void handle_stop(int signum){
    print_stats();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "------------------------- Benchmark is stopping ---------------------------------------------"<< std::endl;
    try {
        stop_send_data = false;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        stop_receive_data = false;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        stopFlag = false;
        stats->close_stats();
    } catch (exception e) {
        std::cout << "Error while shutting down:" << e.what() << std::endl;
    }
    std::cout << "---------------------------- Benchmark stopped ----------------------------------------------"<< std::endl;
    exit(0);
}




int main(int argc, char **array) {
    CLI::App app("------------------------ SKYSHARK Benchmarking Tool Version 1.0 -----------------------------");
    std::cout << "------------------------ SKYSHARK Benchmarking Tool Version 1.0 -----------------------------"<< std::endl;
    std::cout << "  Author: Maximilian Langohr   E-Mail: Maximilian.Langohr@fau.de     Website: skyshark.org   "<< std::endl;
    std::cout << "#############################################################################################"<< std::endl;
    std::cout << "------------------------------ Benchmark is starting ----------------------------------------"<< std::endl;
    signal(SIGINT,handle_stop);
    signal(SIGTERM,handle_stop);


    app.add_option("-q, --query",query, "ID of the query you want to benchmark");
    config.set_query_id(query);

    fs::path folder_path = "../results/query" + std::to_string(query);
    if (folder_exists(folder_path)){
        delete_folder(folder_path);
        create_folder(folder_path);
    } else {
        create_folder(folder_path);
    }

    config =  Benchmark::Config("./config.json");
    stats = new Benchmark::NetworkStatistics(config);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    receiveData = new thread(receive_data,ref(config));
    sendData = new thread(send_data,ref(config));

    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "---------------------------- Benchmark started ----------------------------------------------"<< std::endl;
    while(stopFlag){}
}

#endif