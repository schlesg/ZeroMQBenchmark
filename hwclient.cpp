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

int main(int argc, char *argv[])
{
  
  json msg;
  msg["src"] = "client"; //TODO according to params
  msg["seq_num"] = 0;
  msg["time_stamp"] = 0;
  std::string s(10* 1000 /*1KB*/, '0'); //TODO according to params
  msg["payload"] = s.c_str();

  //http://zeromq.org/blog:zero-copy
  // Create a publisher socket
  zmq::context_t context(1);
  zmq::socket_t publisher(context, ZMQ_PUB);
  
  // Open the connection
  cout << "Binding to " << IPC_ENDPOINT << "..." << endl;
  publisher.bind(IPC_ENDPOINT.c_str());

  // Pause to connect
  this_thread::sleep_for(chrono::milliseconds(1000));

  for (size_t seq = 1; seq < 10000; seq++) //TODO according to params
  {
    // Current time in ms
    high_resolution_clock::time_point p = high_resolution_clock::now();
    microseconds us = duration_cast<microseconds>(p.time_since_epoch());
    msg["time_stamp"] = to_string(us.count());
    msg["seq_num"] = seq;


    // Create a message and feed data into it
    //  zmq::message_t message(1000);
    //  memcpy (message.data(), text.c_str(), text.length());
    // message << text;

    // Send it off to any subscribers
    //publisher.send(message);
    auto msg_str = msg.dump();
    zmq::message_t message(msg_str.size());
    memcpy(message.data(), msg_str.data(), msg_str.size());
    publisher.send(message);

    //s_send(publisher, text);
    cout << "[SENT] at " << us.count() << endl;

    this_thread::sleep_for(chrono::milliseconds(100)); //TODO according to params
  }

  publisher.disconnect(PUBLISH_ENDPOINT.c_str());
  return 0;
}