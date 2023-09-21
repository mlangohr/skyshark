#ifndef TCP_ADAPTER
#define TCP_ADAPTER

#include <iostream>
#include <cstring>
#include <tuple>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "AdapterInterface.h"
#include "../exceptions/SocketErrorException.cpp"

using namespace std;
/**
 * Parameter:
 * bind_ip: IP address to bind the TCP server to
 * bind_port: Port to bind the TCP server to
 * server_ip: IP of the server to connect to
 * server_port: Port of the server to connect ot
 * buffer_size: Size of the internal buffer
 */
 namespace Benchmark {
     class TCPAdapter: public Benchmark::AdapterInterface {

    private:
        int tcp_buffer_size;

        socklen_t clientAddLen;
        int adapterSocket, clientSocket;
        struct sockaddr_in serverAddr, clientAddr;


    public:
        void init_sender(Benchmark::Config config) override {
            this->adapterSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (adapterSocket == -1){
                throw  Benchmark::SocketErrorException("TCP","Could not create socket for sender");
            }

            serverAddr.sin_family = AF_INET;
            serverAddr.sin_addr.s_addr = inet_addr(config.get_tcp_ip().c_str());
            serverAddr.sin_port = htons(config.get_tcp_port());

        if (::connect(adapterSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
            close(adapterSocket);
            throw Benchmark::SocketErrorException("TCP","Could not connect to the SPS");
        }

        }

        void init_receiver(Benchmark::Config config) override {
            this->tcp_buffer_size = config.get_dataframe_size();
            adapterSocket = socket(AF_INET,SOCK_STREAM,0);
            if (adapterSocket == -1){
                throw  Benchmark::SocketErrorException("TCP","Could not create socket");
            }

            serverAddr.sin_family = AF_INET;
            serverAddr.sin_addr.s_addr = INADDR_ANY;
            serverAddr.sin_port = htons(config.get_bind_port());

            if (::bind(adapterSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1){
                close(adapterSocket);
                throw  Benchmark::SocketErrorException("TCP","Could not bind to socket");
            }

            if(listen(adapterSocket,1) == -1){
                close(adapterSocket);
                throw Benchmark::SocketErrorException("TCP","Could not listen to socket");
            }

            clientAddLen = sizeof(clientAddr);
            clientSocket = accept(adapterSocket, (struct sockaddr*) &clientAddr, &clientAddLen);
            if (clientSocket == -1){
                close(clientSocket);
                close(adapterSocket);
                throw  Benchmark::SocketErrorException("TCP","Could not accept client connection");
            }

        }


        void send_data(char* data, int message_size) override{
             if (::send(adapterSocket, data, message_size, 0) == -1){
                throw new SocketErrorException("TCP","Could not send TCP package to SPS");
            }
        }

        int receive_data(char &data) override{
            ssize_t bytesRead = recv(clientSocket, &data, this->tcp_buffer_size ,0);
            if (bytesRead == 0 ){
                throw new SocketErrorException("TCP","Client disconnected");
            }
            return bytesRead;
        }

        void close_socket() override {
            if (adapterSocket != 0){
                close(adapterSocket);
            }

            if (clientSocket != 0){
                close(clientSocket);
            }
        }
    };
}

#endif