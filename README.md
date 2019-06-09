# ZeroMQ Benchmark

ZeroMQ ping-pong latency tester

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

ZeroMQ apt-get install libzmq3-dev)
Json Parser 
OpenDDS
Boost 1.69
Python3

### Installing

mkdir build
cd build
cmake ..
make

## Running the tests

python3 runFullScaleTest.py 
In case MC is not supported - you will need to add SpdpSendAddrs=192.168.1.105:8410 to the rtps_disc.ini

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

