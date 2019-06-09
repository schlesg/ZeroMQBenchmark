# ZeroMQ Benchmark

ZeroMQ ping-pong latency tester

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

#### ZeroMQ  
apt-get install libzmq3-dev  

#### Json Parser  
git clone https://github.com/nlohmann/json 
    cd json 
    cmake . 
    make 
    make install

#### Boost  
   wget -O boost_1_69_0.tar.gz https://sourceforge.net/projects/boost/files/boost/1.69.0/boost_1_69_0.tar.gz/download  
   tar xzvf boost_1_69_0.tar.gz  
   cd boost_1_69_0  
   ./bootstrap.sh  
   ./b2  

#### Python3  
apt install python3

### Installing
mkdir build  
cd build  
cmake ..  
make  

## Running the tests

* Can run the ZMQInitiator or the ZMQEchoer directly:
  - ./ZMQInitiator --PubEP tcp://127.0.0.1:4242,tcp://127.0.0.1:4243 --SubEP tcp://127.0.0.1:4241 -msgLength 1000  
* More complex test are defined within the py files (e.g. IPC-1-5Test.py).

## License

This project is licensed under the MIT License - see the [License.md](License.md) file for details

