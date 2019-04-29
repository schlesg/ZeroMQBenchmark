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
#include <boost/format.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <string>

using json = nlohmann::json;
using namespace std;
using namespace std::chrono;
namespace po = boost::program_options;

// static const std::string PUBLISH_ENDPOINT = "tcp://*:4242"; //TODO according to params
// static const std::string IPC_ENDPOINT = "ipc:///tmp/0";
void my_free (void *data, void *hint)
{
    //  We've allocated the buffer using malloc and
    //  at this point we deallocate it using free.
    //free (data);
}
class Config
{
public:
    static std::string pubEndpoint;
    static std::string publisherUniqueName;
    static uint64_t payloadSize;
    static int64_t executionTime;
    static double pubRate;
};

std::string Config::pubEndpoint;
std::string Config::publisherUniqueName;
uint64_t Config::payloadSize;
int64_t Config::executionTime;
double Config::pubRate;

int main(int argc, char *argv[])
{
    po::options_description description("Options");
    description.add_options()("help", "produce help message")
    ("PubEP", po::value(&Config::pubEndpoint)->default_value("ipc:///tmp/0"), "Publish endpoint (e.g. ipc:///tmp/0 or tcp://*:4242")
    ("pubUniqueName", po::value(&Config::publisherUniqueName)->default_value("pub1"), "Publisher Unique Name")
    ("msgLength", po::value(&Config::payloadSize)->default_value(100), "Message Length (bytes)")
    ("executionTime", po::value(&Config::executionTime)->default_value(100), "Benchmark Execution Time (sec)")
    ("pubRate", po::value(&Config::pubRate)->default_value(100), "Publishing rate");

    po::variables_map vm;

    try
    {
        po::store(po::parse_command_line(argc, argv, description), vm);
        po::notify(vm);
    }
    catch (const po::error &e)
    {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    if (vm.count("help"))
    {
        std::cout << description << "\n";
        return EXIT_SUCCESS;
    }



  //TODO update argument via config file
  json msg;
  msg["src"] = Config::publisherUniqueName;
  std::string s(Config::payloadSize, '0');
  msg["payload"] = s.c_str();

  //buffer for zeroCopy
  auto buffer = new u_char[1000];
  //http://zeromq.org/blog:zero-copy
  // Create a publisher socket
  zmq::context_t context(1);
  zmq::socket_t publisher(context, ZMQ_PUB);
  
  // Open the connection
  cout << "Binding to " << Config::pubEndpoint << "..." << endl;
  publisher.bind(Config::pubEndpoint.c_str());

  // Pause to connect
  this_thread::sleep_for(chrono::milliseconds(1000));

  std::chrono::time_point<std::chrono::high_resolution_clock> executionStartTime = high_resolution_clock::now();
  uint64_t seq = 0;
  while(1)
  {
    seq++;
    msg["seq_num"] = seq;
    
    if ((duration_cast<seconds>(high_resolution_clock::now() - executionStartTime)).count() > Config::executionTime)
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
            
            if (seq % 100 == 0)
            {
                std::cerr << "pub " << seq << endl;
            }

        }

    this_thread::sleep_for(chrono::milliseconds(u_int64_t(1000 * (1 / Config::pubRate))));
  }

  publisher.disconnect(Config::pubEndpoint.c_str());
  return 0;
}