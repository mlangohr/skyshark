#ifndef STATE_CPP
#define STATE_CPP

#include <optional>
#include <string>
#include <cstring>
#include <chrono>

namespace Benchmark {
    class State {
    public:
        uint64_t id;
        char icao24[6];
        char callsign[9];
        char origin_country[30];
        uint32_t time_position;
        uint32_t last_contact;
        double longitude;
        double latitude;
        double baro_altitude;
        char on_ground[6];
        double velocity;
        double true_track;
        double vertical_rate;
        char sensors[16];
        double geo_altitude;
        uint16_t squawk;
        char spi[6];
        uint8_t position_source;

        State(){};

        State(uint64_t id,
              std::string icao24,
              std::string callsign,
              std::string origin_country,
              uint32_t time_position,
              uint32_t last_contact,
              double longitude,
              double latitude,
              double baro_altitude,
              std::string on_ground,
              double velocity,
              double true_track,
              double vertical_rate,
              std::string sensors,
              double geo_altitude,
              uint16_t squawk,
              std::string spi,
              uint8_t position_source){

            this->id = id;
            strncpy(this->icao24, icao24.c_str(), sizeof(this->icao24));


            strncpy(this->callsign, callsign.c_str(), sizeof(this->callsign));


            strncpy(this->origin_country, origin_country.c_str(), sizeof(this->origin_country) );
            this->time_position = time_position;
            this->last_contact = last_contact;
            this->longitude = longitude;
            this->latitude = latitude;
            this->baro_altitude = baro_altitude;
            strncpy(this->on_ground, on_ground.c_str(), sizeof(this->on_ground));

            this->velocity = velocity;
            this->true_track = true_track;
            this->vertical_rate =vertical_rate;
            strncpy(this->sensors, sensors.c_str(), sizeof(this->sensors));

            this->geo_altitude = geo_altitude;
            this->squawk = squawk;
            strncpy(this->spi, spi.c_str(), sizeof(this->spi));

            this->position_source = position_source;
        };
    }__attribute__((packed,aligned(8)));
}
#endif