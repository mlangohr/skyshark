#include <iostream>
#include <unistd.h>
#include <array>
#include <thread>
#include <chrono>

#include "src/include/csv.h"
#include "src/include/CLI11.hpp"

#include "src/config.h"
#include "src/query_results.cpp"
#include "src/state.cpp"


void convert (std::string input_file, std::string output_file){
    std::ofstream output(output_file, std::ios::binary | std::ios::out);
    try {
        io::CSVReader<17> in(input_file);
        std::string icao24;
        std::string callsign;
        std::string origin_country;
        uint32_t time_position;
        uint32_t last_contact;
        double longitude;
        double latitude;
        double baro_altitude;
        std::string on_ground;
        double velocity;
        double true_track;
        double vertical_rate;
        std::string sensors;
        double geo_altitude;
        uint16_t squawk;
        std::string spi;
        uint8_t position_source;
        while (in.read_row(icao24, callsign, origin_country, time_position, last_contact,
                           longitude, latitude,baro_altitude, on_ground, velocity,true_track,
                           vertical_rate, sensors, geo_altitude, squawk, spi, position_source)) {

            std::chrono::time_point now = std::chrono::system_clock::now();
            ::uint64_t microseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
               Benchmark::State state(
                       microseconds_since_epoch,
                       icao24,
                       callsign,
                       origin_country,
                       time_position,
                       last_contact,
                       longitude,
                       latitude,
                       baro_altitude,
                       on_ground,
                       velocity,
                       true_track,
                       vertical_rate,
                       sensors,
                       geo_altitude,
                       squawk,
                       spi,
                       position_source
                       );
            output.write(reinterpret_cast<const char*>(&state),sizeof(state));

        }
        output.close();
    }catch(std::exception & e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

template<typename T, typename = std::enable_if_t<std::is_base_of<Benchmark::QueryResult,T>::value>>
void convert_result(Benchmark::Config config){
    cout << "starting process convert_result [x]" << std::endl;
    std::fstream in_file("../results/query" + std::to_string(config.get_query_id()) + "/result.bin", std::ios::binary | std::ios::in);
    std::ofstream out_file("../results/query" + std::to_string(config.get_query_id()) + "/result.csv", std::ios::binary);
    T item;
    out_file << item.get_header() << std::endl;
    while(in_file.read(reinterpret_cast<char*>(&item),sizeof(T))){
        out_file << item.to_csv() << std::endl;
    }
    in_file.close();
    out_file.close();
    cout << "process convert_result stopped [x]" << std::endl;
}

void convert_bin_result(Benchmark::Config config){
    switch (config.get_query_id()){
        case 1:
            ::convert_result<Benchmark::Q1>(config);
            break;
        case 2:
            ::convert_result<Benchmark::Q2>(config);
            break;
        case 3:
            ::convert_result<Benchmark::Q3>(config);
            break;
        case 4:
            ::convert_result<Benchmark::Q4>(config);
            break;
        case 5:
            ::convert_result<Benchmark::Q5>(config);
            break;
        case 6:
            ::convert_result<Benchmark::Q6>(config);
            break;
        case 7:
            ::convert_result<Benchmark::Q7>(config);
            break;
        case 8:
            ::convert_result<Benchmark::Q8>(config);
            break;
        case 9:
            ::convert_result<Benchmark::Q9>(config);
            break;
        case 10:
            ::convert_result<Benchmark::Q10>(config);
            break;
    }
}

int main(int argc, char **argv) {
    CLI::App app("---------- SKYSHARK Converter ----------");

    std::string mode = "states";
    std::string input_file = "states.csv";
    std::string output_file = "states.bin";
    int query_id = 1;

    app.add_option("-i, --input", input_file, "Input file, usually states.csv");
    app.add_option("-o, --output", output_file, "Output file, usually states.bin");
    app.add_option("-m, --mode", mode, "Type of conversion performed [states,results]");
    app.add_option("-q, --query", query_id, "ID of the query that was benchmarked");
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    if (mode == "states") {
        cout << "---------------------------- Starting to parse state csv -------------------------------------"<< std::endl;
        convert(input_file, output_file);
        cout << "-------------------------------------- Done --------------------------------------------------"<< std::endl;
    } else if (mode == "results") {
        cout << "------------------------------ Converting result ---------------------------------------------"<< std::endl;
        Benchmark::Config config("./config.json");
        config.set_query_id(query_id);
        convert_bin_result(config);
        cout << "------------------------------ Results converted ---------------------------------------------"<< std::endl;
    }

}