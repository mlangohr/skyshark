#ifndef UDP_ADAPTER
#define UDP_ADAPTER

#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include "AdapterInterface.h"
#include "../exceptions/SocketErrorException.cpp"
using namespace std;



namespace Benchmark {
class UDPAdapter: public Benchmark::AdapterInterface {
private:
    int server_socket, recv_len, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    socklen_t server_addr_len = sizeof(server_addr);
    char* buffer;

public:
    void init_sender(Benchmark::Config config) override {
        client_socket = socket(AF_INET,SOCK_DGRAM,0);
        if (client_socket == -1){
            close(client_socket);
            throw Benchmark::SocketErrorException("UDP","Could not create socket for sender");
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(config.get_udp_port());
        if(inet_pton(AF_INET, config.get_udp_ip().c_str(), &server_addr.sin_addr) <= 0){
            close(client_socket);
            throw Benchmark::SocketErrorException("UDP","Invalid server address");
        }
    }

    void init_receiver(Benchmark::Config config) override {
        server_socket = socket(AF_INET, SOCK_DGRAM,0);
        if (server_socket == -1) {
            close(server_socket);
            throw Benchmark::SocketErrorException("UDP", "Could not create socket");
        }
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(config.get_udp_port());

         if (::bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
            close(server_socket);
            throw Benchmark::SocketErrorException("UDP","Failed to bin socket");
        }

        this->buffer = new char[config.get_dataframe_size()];

    }

    void send_data(char* data, int message_size) override {
        sendto(client_socket,data,message_size,0,(struct sockaddr *)&server_addr, server_addr_len);
    }

    int receive_data(char &data) override {
        recv_len = recvfrom(server_socket, buffer, 11952,0,(struct sockaddr *)&client_addr, &client_addr_len);
        if (recv_len == -1){
            close(server_socket);
            throw Benchmark::SocketErrorException("UDP","Failed to receive data from SPS");
        }
        ::memcpy(&data,buffer,recv_len);
        return recv_len;
    }
};
}

#endif