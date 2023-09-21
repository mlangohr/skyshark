#ifndef PARAMETER_NOT_FOUND
#define PARAMETER_NOT_FOUND

#include <iostream>
#include <exception>

namespace Benchmark {
    class ParameterNotFoundException : public std::exception {
    private:
        std::string adapter;
        std::string parameter;
    public:
        ParameterNotFoundException(std::string adapter, std::string parameter) {
            this->adapter = adapter;
            this->parameter = parameter;
        }

        const char *what() const noexcept override {
            static std::string message = "Could not find parameter[" + parameter + "] for adapter of type: " + adapter;
            return message.c_str();
        }
    };
}

#endif