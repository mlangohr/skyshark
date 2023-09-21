#ifndef KAFKA_ADAPTER
#define KAFKA_ADAPTER

#include <iostream>
#include <map>
#include "AdapterInterface.h"
#include <cppkafka/cppkafka.h>
#include "cppkafka/utils/buffered_producer.h"


using namespace std;
using namespace cppkafka;

namespace Benchmark {
    class KafkaAdapter: public Benchmark::AdapterInterface {
    private:
        std::string topic;
        BufferedProducer<std::string> *producer;
        Consumer *consumer;

    public:
        void init_sender(Benchmark::Config config) override {
            this->topic = config.get_kafka_producer_topic();
            Configuration kafka_config = {
                    {"metadata.broker.list", config.get_kafka_broker()}
            };

            this->producer = new BufferedProducer<std::string>(kafka_config);

        }

        void init_receiver(Benchmark::Config config) override {
            this->topic = config.get_kafka_consumer_topic();
            Configuration kafka_config = {
                    {"metadata.broker.list", config.get_kafka_broker()},
                    {"group.id", "skyshark-consumer-1"}
            };
            this->consumer = new Consumer(kafka_config);
            this->consumer->subscribe({this->topic});


        }

        void send_data(char* data, int message_size) override {
            std::string payload(data,message_size);
            this->producer->produce(MessageBuilder(this->topic).partition(0).payload(payload));
            this->producer->flush();
        }

        int receive_data(char &data) override {
            while(true) {
                Message msg = consumer->poll();
                if (!msg) {
                    cerr << "Could not fetch message from Kafka" << std::endl;
                    continue;
                }

                if (msg.get_error()){
                    cerr << "Error receiving message from Kafka" << std::endl;
                    continue;
                }
                ::memcpy(&data,msg.get_payload().get_data(),msg.get_payload().get_size());
                return msg.get_payload().get_size();
            }

        }
    };
}

#endif