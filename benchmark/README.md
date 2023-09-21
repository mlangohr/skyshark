# SKYSHARK BENCHMARK

## Installing Dependencies

0. **Install libssl-dev if not already installed**
   1. Install libssl-dev using ```sudo apt-get install libssl-dev```
1. **Install C++ Compiler**
   1. Install architecture specific compiler
2. **Install CMAKE**
   1. If you haven't installed CMake yet, you can download and install it from the official CMake [homepage](https://cmake.org/download/)
3. **Install Boost**
   1. Boost is a collection of libraries that C++ projects often use. You can download Boost from the official website [homepage](https://www.boost.org/users/download/)
      e.g. on Ubuntu ```sudo apt-get install libboost-dev-all```
4. **Install ZeroMQ**
   1. Install [libzmq](https://github.com/zeromq/libzmq) to support ZeroMQ
      1. It is recommended to build the library from source.
   2. cppzmq is the C++ binding for ZeroMQ. You can install it using the package manager for your system, or you can build it from source following the instructions on the GitHub [repository](https://github.com/zeromq/cppzmq)
5. **Install CppKafka**
   1. Install [librdkafka](https://github.com/confluentinc/librdkafka) as requirement for CppKafka.
   2. CppKafka is a C++ wrapper for the Apache Kafka C library. You can install it using a package manager for your system or build it from source. The GitHub [repository](https://github.com/mfontanini/cppkafka) contains instructions for both options.


Note: The specific installation instructions may vary depending on your operating system and package manager.
Be sure to refer to the official documentation or repositories of each library for detailed and up-to-date installation instructions.
Additionally, some libraries may require specific configurations or settings during the installation process, so be sure to follow any guidelines provided by the library maintainers.


## Build and Compile

#### Create build directory
`mkdir build && cd build`

#### Run CMake

`cmake ..`

#### Build

`make`

## Create Dataset / Converter

The basis for this benchmark is the `states.csv` created using our SKYSHARK Tooling.
The `converter` tool allows the user to convert the `states.csv` to `states.bin`.
To speed up the reading speed when running the benchmark, we store the data as binary data. 
This binary file is hardware and architecture specific.

The `converter` tool can also convert the result file `result.bin` back to `result.csv` and the timestamps from binary format to csv.

### How to Use

```
./converter -i [input file states.csv] -o [output file states.bin] -m [mode states/result_only/all] -q [query]
```

The default mode is `states` which converts the `states.csv` into the binary format.

- ```result_onyly```: only converts the result binary, if no latency is measured.
- ```all```: converts the result file and timestamp files, if latency is measured.

## Run and Benchmark

Before starting the benchmark you have to copy the configuration file `config.json.example` to the build folder, rename it to 'config.json' 
and adjust it to your needs.

### Configuration

We support multiple communication protocols like TCP, UDP, KAFKA and ZeroMQ.


#### Base 

| **attribute**   | **description**                             | **allowed values**                 |
|-----------------|---------------------------------------------|------------------------------------|
| adapter         | Type of adapter to communicate with the SPS | tcp,udp,kafka,zeromq               |
| latency         | If **true** the latency is calculated       | true, false                        |
| queryId         | ID of the query that is benchmarked         | 1 - 10                             |
| input_file      | Name of the file containing the states      | e.g. states.bin                    |
| data_frame_size | Size of the datapackage sent over network   | e.g. 11952 for TCP or 7901 for UDP |
| bind_ip         | IP to bind the benchmark to                 | e.g. 127.0.0.1                     |
|                 | if multiple NICs are available              |                                    |
| bind_ port      | Port to bind the benchmark to               | e.g. 8081                          |       


#### TCP

| **attribute** | **description**                   | **allowed values** |
|---------------|-----------------------------------|--------------------|
| ip            | IP of the SPS to sent the data to | e.g. 127.0.0.1     |
| port          | Port of the SPS                   | e.g. 8081          |


#### UDP 

| **attribute** | **description**                   | **allowed values** |
|---------------|-----------------------------------|--------------------|
| ip            | IP of the SPS to sent the data to | e.g. 127.0.0.1     |
| port          | Port of the SPS                   | e.g. 8081          |


#### Kafka

| **attribute**  | **description**                     | **allowed values**  |
|----------------|-------------------------------------|---------------------|
| broker         | IP:PORT of the Kafka broker         | e.g. 127.0.0.1:9093 |
| producer_topic | Topic to send tuples to             | e.g. testtopic      |
| consumer_topic | Topic to receive result tuples from | e.g. testtopic      |


#### ZeroMQ

| **attribute** | **description**                   | **allowed values**   |
|---------------|-----------------------------------|----------------------|
| ip            | IP of the SPS to sent the data to | e.g. tcp://127.0.0.1 |
| port          | Port of the SPS                   | e.g. 5555            |



### Run Benchmark


```
./server -q [ID of the query you want to benchmark]
```

The tool will now load the data from the drive and send them to the SPS.
At the same time a port is opened on the machine to listen for the results produced by the SPS.
You can now stop the benchmark at any time using ctrl + c or wait for it to finish. 
The tool will generate a new folder ```results/query[id]```.
This folder contains 3 files in binary format:

- The results
- Outgoing timestamps (if latency measurement is configured)
- Incoming timestamps (if latency measurement is configured)

### Convert Results

After running the benchmark you need to convert the results in order to analyse them. 
Using the default values running the benchmark you can use the converter program using the ``-m`` option set to ``results``.

