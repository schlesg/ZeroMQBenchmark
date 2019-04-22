/**
* Example of ZeroMQ pub/sub usage for C++11.
*/

#include <zmq.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include "zhelpers.hpp"

using namespace std;
using namespace std::chrono;

static const string PUBLISH_ENDPOINT = "tcp://*:4242";
static const string IPC_ENDPOINT = "ipc:///tmp/0";

int main(int argc, char *argv[]) {

  //https://github.com/nlohmann/json
  //http://zeromq.org/blog:zero-copy
  // Create a publisher socket
    zmq::context_t context (1);
    zmq::socket_t publisher (context, ZMQ_PUB);

  // Open the connection
  cout << "Binding to " << IPC_ENDPOINT << "..." << endl;
  publisher.bind(IPC_ENDPOINT.c_str());

  // Pause to connect
  this_thread::sleep_for(chrono::milliseconds(1000));

  while(true) {

    // Current time in ms
    high_resolution_clock::time_point p = high_resolution_clock::now();
    microseconds us = duration_cast<microseconds>(p.time_since_epoch());

  
    //unsigned long long us = duration_cast<microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());

    string text = "" + to_string(us.count());

    // Create a message and feed data into it
    //  zmq::message_t message(1000);
    //  memcpy (message.data(), text.c_str(), text.length());
    // message << text;

    // Send it off to any subscribers
    //publisher.send(message);
    
    zmq::message_t message(text.size());
    memcpy (message.data(), text.data(), text.size());
    publisher.send(message);

    //s_send(publisher, text);
    cout << "[SENT] at " << text << endl;

    this_thread::sleep_for(chrono::milliseconds(100));
  }

  publisher.disconnect(PUBLISH_ENDPOINT.c_str());
  return 0;
}