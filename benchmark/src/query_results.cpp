#ifndef QUERY_RESULT
#define QUERY_RESULT

#include <iostream>
#include <string>

using namespace std;

namespace Benchmark {

    class QueryResult {
    };


    class Q1: QueryResult {
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

        std::string get_header()  {
            return "id,icao24,callsign,origin_country,time_position,last_contact,longitude,latitude,baro_altitude,"
                   "on_ground,velocity,true_track,vertical_rate,sensors,geo_altitude,squawl,spi,position_source";
        }

        std::string to_csv()   {
            return std::to_string(id) + "," + string(icao24) + "," + callsign + "," + origin_country + std::to_string(time_position) + ","
            + std::to_string(last_contact) + "," + std::to_string(longitude) + "," + std::to_string(latitude) + "," +
            std::to_string(baro_altitude) + "," + on_ground + "," + std::to_string(velocity) + "," + std::to_string(true_track)
            + "," + std::to_string(vertical_rate) + "," + sensors + "," + std::to_string(geo_altitude) + "," + std::to_string(squawk) + ","
            + spi + "," + std::to_string(position_source);
        }

        ::uint64_t get_id()  {
            return this->id;
        }
    }__attribute__((packed,aligned(8)));

    class Q2 : QueryResult {
    public:
        ::uint32_t id;
        char icao24[6];
        std::uint64_t time_position;
        char callsign[9];

        std::string get_header()  {
            return "id,icao24,time_position,callsign";
        }

        std::string to_csv()  {
            return to_string(id) + "," + string(icao24) + "," + std::to_string(time_position) + "," + callsign;
        }

        ::uint32_t get_id()  {
            return this->id;
        }
    }__attribute__((packed,aligned(8)));

    class Q3: QueryResult {
    public:
        ::uint32_t id;
        char icao24[6];
        std::uint64_t time_position;
        std::uint64_t last_contact;
        double longitude;
        double latitude;
        double velocity;
        double true_track;
        double vertical_rate;
        char callsign[9];
        char on_ground[6];
        char spi[6];
        uint16_t  squawk;
        double baro_altitude;
        double geo_altitude;

        std::string get_header()  {
            return "id,icao24,time_position,last_contact,longitude,latitude,velocity,true_track,vertical_rate,callsign,on_ground,spi,squawk,baro_altitude,geo_altitude";
        }

        std::string to_csv()  {
            return to_string(id) + "," + string(icao24) + "," + std::to_string(time_position) + "," + std::to_string(last_contact) + ","
            + std::to_string(longitude)+ "," + std::to_string(latitude)+ "," + std::to_string(velocity) + "," +
            std::to_string(true_track) + "," + std::to_string(vertical_rate) + "," + callsign + "," + on_ground + ","
            + spi + "," + std::to_string(squawk)+ "," + std::to_string(baro_altitude) + "," + std::to_string(geo_altitude);
        }

        ::uint32_t get_id()  {
            return this->id;
        }
    }__attribute__((packed,aligned(8)));

    class Q4: QueryResult {
    public:
        ::uint32_t id;
        char icao24[6];
        char callsign[0];
        std::uint64_t time_position;
        double longitude;
        double latitude;
        double velocity;
        double true_track;
        double baro_altitude;

        std::string get_header()  {
            return "id,icao24,callsign,time_position,longitude,latitude,velocity,true_track,baro_altitude";
        }

        std::string to_csv()  {
            return to_string(id)+ "," + string(icao24) + "," + callsign + "," + ::to_string(time_position) + "," + to_string(longitude) + ","+
                    to_string(latitude) + "," + to_string(velocity) + "," + to_string(true_track) + "," +
                    to_string(baro_altitude);
        }

        ::uint32_t get_id()  {
            return this->id;
        }
    }__attribute__((packed, aligned(8)));

    class Q5: QueryResult {
    public:
        ::uint32_t id;
        char icao24[6];
        char callsign[9];
        std::uint64_t time_position;
        double longitude;
        double latitude;
        double baro_altitude;

        std::string get_header()  {
            return "id,icao24,callsign,time_position,longitude,latitude,baro_altitude";
        }
        std::string to_csv()  {
            return to_string(id) + "," + string(icao24) + "," + callsign + to_string(time_position)+
            "," + to_string(longitude) + "," + to_string(latitude) + "," +
                    to_string(baro_altitude);
        }

        ::uint32_t get_id()  {
            return this->id;
        }
    }__attribute__((packed,aligned(8)));

    class Q6 : QueryResult {
    public:
        ::uint32_t id;
        char icao24[6];
        char callsign[9];
        std::uint64_t time_position;
        double longitude;
        double latitude;
        double baro_altitude;

        std::string get_header()  {
            return "id,icao24,callsign,time_position,longitude,latitude,baro_altitude";
        }

        std::string to_csv()  {
            return to_string(id) + "," + string(icao24) + "," + callsign + to_string(time_position) + "," +
            to_string(longitude) + "," + to_string(latitude) + "," + to_string(baro_altitude);
        }

        ::uint32_t get_id()  {
            return this->id;
        }
    }__attribute__((packed,aligned(8)));

    class Q7 : QueryResult {
    public:
        ::uint32_t id;
        char icao24[6];
        char callsign[9];
        std::uint64_t time_position;
        double longitude;
        double latitude;
        double velocity;
        double true_track;
        double baro_altitude;

        std::string get_header()  {
            return "id,icao24,callsign,time_position,longitude,latitude,velocity,true_track,baro_altitude";
        }
        std::string to_csv()  {
            return to_string(id) + "," +string(icao24) + "," + callsign + "," + to_string(time_position) + "," + to_string(longitude) + "," +
                    to_string(latitude) + "," + to_string(velocity) + "," + to_string(true_track) + "," +
                    to_string(baro_altitude);
        }

        ::uint32_t get_id()  {
            return this->id;
        }
    }__attribute__((packed,aligned(8)));

    class Q8 : QueryResult {
    public:
        ::uint32_t id;
        char icao24[6];
        char callsign[9];
        std::uint64_t time_position;
        std::string inside_airspace;

        std::string get_header()  {
            return "id,icao24,callsign,time_position,inside_airspace";
        };

        std::string to_csv()  {
            return to_string(id) + "," +string(icao24) + "," + callsign + "," + to_string(time_position) + "," + inside_airspace;
        }

        ::uint32_t get_id()  {
            return this->id;
        }
    }__attribute__((packed,aligned(8)));

    class Q9: QueryResult {
    public:
        ::uint32_t id;
        char icao24[6];
        char callsign[9];
        std::uint64_t time_position;
        double latitude;
        double longitude;
        double baro_altitude;
        double distance_squared;

        std::string get_header()  {
            return "id,icao24,callsign,time_position,latitude,longitude,baro_altitude,distance_squared";
        }

        std::string to_csv()  {
            return to_string(id) + "," + string(icao24) + "," + callsign + "," + to_string(time_position) + "," + to_string(latitude) + "," +
            to_string(longitude) + "," + to_string(baro_altitude) + "," + to_string(distance_squared);
        }

        ::uint32_t get_id()  {
            return this->id;
        }
    }__attribute__((packed,aligned(8)));

    class Q10: QueryResult {
    public:
        ::uint32_t id;
        char icao24[6];
        char callsign[9];
        std::uint64_t time_position;
        char registration[8];
        char type[4];
        char typedesc[30];
        char owner[30];

        std::string get_header()  {
            return "id,icao24,callsign,time_position,registration,type,typedesc,owner";
        }

        std::string to_csv()  {
            return to_string(id) + "," + string(icao24) + "," + callsign + "," + to_string(time_position) + "," +
            registration + "," + type + "," + typedesc + "," + owner;
        }

        ::uint32_t get_id()  {
            return this->id;
        }
    }__attribute__((packed,aligned(8)));

}

#endif