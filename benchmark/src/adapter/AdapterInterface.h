#pragma once
#include <queue>
#include <tuple>
#include <map>
#include <iostream>
#include "../config.h"


using namespace  std;

namespace Benchmark {

    class AdapterInterface {
    public:
        virtual void init_sender(Benchmark::Config config){};
        virtual void send_data(char* data, int message_size){};
        virtual void init_receiver(Benchmark::Config config){};
        virtual int receive_data(char &data){return 0;};
        virtual void close_socket(){};
        virtual ~AdapterInterface() {}
    };

}