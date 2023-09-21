#ifndef CONFIG_EXCEPTION
#define CONFIG_EXCEPTION

#include <exception>
#include <iostream>

namespace Benchmark{
class ConfigException: public std::exception {
    private:
        std::string message;
    public:
        ConfigException(std::string message){
            this->message = message;
        }

        const char* what() const noexcept override {
            static std::string msg = "ConfigException: " + this->message;
            return msg.c_str();
        }
    };
}

#endif