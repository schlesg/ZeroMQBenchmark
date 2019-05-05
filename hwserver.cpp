
/**
* Example of ZeroMQ pub/sub usage for C++11.
*/

#include <zmq.hpp>
#include <iostream>
#include <chrono>
#include <string>
#include "zhelpers.hpp"
#include "nlohmann/json.hpp"
#include "BenchmarkLogger.hpp"

using namespace std;
using namespace std::chrono;
using json = nlohmann::json;

static const string PUBLISHER_ENDPOINT = "tcp://localhost:4242";
static const string IPC_ENDPOINT = "ipc:///tmp/0";

int main(int argc, char *argv[]) {

  // Create a subscriber socket
  zmq::context_t context (1);
  zmq::socket_t subscriber  (context, ZMQ_SUB);
  subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);

  // Connect to the publisher
  cout << "Connecting to " << IPC_ENDPOINT << "..." << endl;
  subscriber.connect(IPC_ENDPOINT.c_str());


  while(true) {
    zmq::message_t message;
    subscriber.recv(&message);
    high_resolution_clock::time_point p = high_resolution_clock::now();
    microseconds nowTime = duration_cast<microseconds>(p.time_since_epoch());

    auto msg = std::string(static_cast<char*>(message.data()), message.size());
    // cout<<msg<<endl;
    //char *msg = static_cast<char*>(message.data());
    json json_msg = json::parse(msg);
    

   auto srcTimestamp = std::string(json_msg["time_stamp"]);
        if (srcTimestamp.compare("0") == 0)
        {
            cout << "END OF TEST" << endl;
            BenchmarkLogger::DumpResultsToFile();
            break;
        }
        else
        {
            auto microsecLatency = nowTime.count() - std::stoll(srcTimestamp);
            //cout << "One way latency (millisec) = " << static_cast<double>(microsecLatency) / 1000 << endl;
            BenchmarkLogger::latencyResults.emplace_back(
                json_msg["src"],
                json_msg["seq_num"],
                microsecLatency);
        }

}
  subscriber.disconnect(IPC_ENDPOINT.c_str());
  return 0;
}


// //
// //  Hello World server in C++
// //  Binds REP socket to tcp://*:5555
// //  Expects "Hello" from client, replies with "World"
// //
// #include <zmq.hpp>
// #include <string>
// #include <iostream>
// #ifndef _WIN32
// #include <unistd.h>
// #else
// #include <windows.h>

// #define sleep(n)	Sleep(n)
// #endif

// int main () {
//     //  Prepare our context and socket
//     zmq::context_t context (1);
//     zmq::socket_t socket (context, ZMQ_REP);
//     socket.bind ("tcp://*:5555");

//     while (true) {
//         zmq::message_t request;

//         //  Wait for next request from client
//         socket.recv (&request);
//         std::cout << "Received Hello" << std::endl;

//         //  Do some 'work'
//     	sleep(1);

//         //  Send reply back to client
//         zmq::message_t reply (5);
//         memcpy (reply.data (), "World", 5);
//         socket.send (reply);
//     }
//     return 0;
// }
