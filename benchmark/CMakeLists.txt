cmake_minimum_required(VERSION 3.0)
project(benchmark)

# Set the C++ standard and threading preferences
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)


# Find required packages
find_package(Threads REQUIRED)
find_package(CppKafka REQUIRED)
find_package(cppzmq CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

# Create a library for the adapter sources
add_library(adapter_lib
        src/adapter/AdapterInterface.h
        src/adapter/TCPAdapter.cpp
        src/adapter/KafkaAdapter.cpp
        src/adapter/UDPAdapter.cpp
        src/adapter/ZeroMQAdapter.cpp
        )

# Create a library for the exceptions sources
add_library(exceptions_lib
        src/exceptions/ParameterNotFoundException.cpp
        src/exceptions/SocketErrorException.cpp
        src/exceptions/ConfigException.cpp
        )


# Link any required libraries to the respective targets
target_link_libraries(adapter_lib PUBLIC CppKafka::cppkafka)
target_link_libraries(exceptions_lib PUBLIC Threads::Threads)
# add include directory


include_directories(src/include)


# SERVER
add_executable(server
        server.cpp
        src/config.h
        src/ring_buffer.cpp
        src/network_statistics.cpp
        src/data_loader.cpp
        src/query_results.cpp
        src/receiver.cpp src/sender.cpp)

target_link_libraries(server PRIVATE
        adapter_lib
        exceptions_lib
        ${Boost_LIBRARIES}
        cppzmq
        )


# CONVERTER
add_executable(converter
        converter.cpp
        src/query_results.cpp)



