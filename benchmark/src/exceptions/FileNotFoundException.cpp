#ifndef FILE_NOT_FOUND_EXCEPTION
#define FILE_NOT_FOUND_EXCEPTION

#include <iostream>
#include <exception>

namespace Benchmark {
    class FileNotFoundException: public std::exception {
    private:
        std::string filename;
    public:
        FileNotFoundException(std::string filename) {
            this->filename = filename;
        }

        const char* what() const noexcept override {
            static std::string message = "Could not find file: " + filename;
            return message.c_str();
        }
    };
}

#endif