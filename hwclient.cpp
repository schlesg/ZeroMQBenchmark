/**
* Example of ZeroMQ pub/sub usage for C++11.
*/

#include <zmq.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include "zhelpers.hpp"
#include "nlohmann/json.hpp"

using namespace std;
using namespace std::chrono;
using json = nlohmann::json;

static const string PUBLISH_ENDPOINT = "tcp://*:4242"; //TODO according to params
static const string IPC_ENDPOINT = "ipc:///tmp/0";
void my_free (void *data, void *hint)
{
    //  We've allocated the buffer using malloc and
    //  at this point we deallocate it using free.
    //free (data);
}

int main(int argc, char *argv[])
{
  //TODO update argument via config file
  json msg;
  msg["src"] = "Pub1"; //TODO according to params
  std::string s(1* 1000 /*1KB*/, '0'); //TODO according to params
  msg["payload"] = s.c_str();

  //buffer for zeroCopy
  auto buffer = new u_char[1000];
  //http://zeromq.org/blog:zero-copy
  // Create a publisher socket
  zmq::context_t context(1);
  zmq::socket_t publisher(context, ZMQ_PUB);
  
  // Open the connection
  cout << "Binding to " << IPC_ENDPOINT << "..." << endl;
  publisher.bind(IPC_ENDPOINT.c_str());

  // Pause to connect
  this_thread::sleep_for(chrono::milliseconds(1000));

  std::chrono::time_point<std::chrono::high_resolution_clock> executionStartTime = high_resolution_clock::now();
  uint64_t seq = 0;
  while(1) //TODO according to params
  {
    seq++;
    msg["seq_num"] = seq;
    
    if ((duration_cast<seconds>(high_resolution_clock::now() - executionStartTime)).count() > 21) //TODO according to params
        {
            msg["time_stamp"] = "0"; //Notify subscriber on ending the test
            std::cerr << "Finishing test " << endl;
            auto msg_str = msg.dump();
            zmq::message_t message(msg_str.size());
            memcpy(message.data(), msg_str.data(), msg_str.size());
            publisher.send(message);
            return 1;
        }
        else
        {
            // Current time in ms
            high_resolution_clock::time_point p = high_resolution_clock::now();
            microseconds us = duration_cast<microseconds>(p.time_since_epoch());
            msg["time_stamp"] = to_string(us.count());
            auto msg_str = msg.dump();
            zmq::message_t message(msg_str.size());
            memcpy(message.data(), msg_str.data(), msg_str.size());
            publisher.send(message);

            //ZeroCopyMethod TODO
            // high_resolution_clock::time_point p = high_resolution_clock::now();
            // microseconds us = duration_cast<microseconds>(p.time_since_epoch());
            // msg["time_stamp"] = to_string(us.count());
            // auto msg_str = msg.dump();
            // zmq_msg_t msg;
            // void *hint = NULL;
            // zmq_msg_init_data (&msg, (void*)msg_str.c_str(), 1000, my_free, hint);
            // publisher.send(message);
            

            cout << "pub " << seq << endl;

        }

    this_thread::sleep_for(chrono::milliseconds(10)); //TODO according to params
  }

  publisher.disconnect(PUBLISH_ENDPOINT.c_str());
  return 0;
}