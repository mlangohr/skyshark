#ifndef SOCKET_ERROR_EXCEPTION
#define SOCKET_ERROR_EXCEPTION

#include <iostream>
#include <exception>

namespace Benchmark {
    class SocketErrorException: public std::exception {
    private:
        std::string adapter;
        std::string error;
    public:
        SocketErrorException(std::string adapter, std::string error){
            this->adapter = adapter;
            this->error = error;
        }

        const char* what() const noexcept override {
            static std::string message = "Adapter [" + adapter + "] Threw an exception of: " + error;
            return message.c_str();
        }
    };
}

#endif