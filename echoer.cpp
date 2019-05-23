
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

// // static const string PUBLISHER_ENDPOINT = "tcp://localhost:4242";
// static const string pubEndpoint = "ipc:///tmp/pongTopic"; //TBD by config
// static const string subEndpoint = "ipc:///tmp/pingTopic"; //TBD by config

class Config
{
public:
  static std::string pubEndpoint;
  static std::string subEndpoint;
  static std::string publisherUniqueName;
};

std::string Config::pubEndpoint;
std::string Config::subEndpoint;
std::string Config::publisherUniqueName;


int main(int argc, char *argv[])
{
  po::options_description description("Options");
  description.add_options()
  ("help", "produce help message")
  ("PubEP", po::value(&Config::pubEndpoint)->default_value("ipc:///tmp/pongTopic"), "Publish endpoint (e.g. ipc:///tmp/pongTopic or tcp://*:4241")
  ("SubEP", po::value(&Config::subEndpoint)->default_value("ipc:///tmp/pingTopic"), "Subscribe endpoint (e.g. ipc:///tmp/pingTopic or tcp://locahost:4242");

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
  // Create a subscriber socket
  zmq::context_t context(2);
  zmq::socket_t subscriber(context, ZMQ_SUB);
  zmq::socket_t publisher(context, ZMQ_PUB);

  cout << "Sub connecting to " << Config::subEndpoint << "..." << endl;
  try {
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    subscriber.connect(Config::subEndpoint.c_str());
  }
  catch (const zmq::error_t& err){
    cout<<"subscriber.connect error - "<<err.what() <<endl;
    return 0 ;
  } 
  cout << "Pub binding to " << Config::pubEndpoint << "..." << endl;
  try{
    publisher.bind(Config::pubEndpoint.c_str());
  }
  catch (const zmq::error_t& err){
    cout<<"publisher.bind error - "<<err.what() <<endl;
    return 0 ;
  }

  size_t i = 0;
  zmq::message_t message;
  while (true)
  {
    i++;
    //cout<<i<<endl;
    subscriber.recv(&message);
    publisher.send(message, ZMQ_DONTWAIT);
  }

  try {
    subscriber.disconnect(Config::subEndpoint.c_str());
    //publisher.unbind(Config::pubEndpoint.c_str());
  }
  catch (const zmq::error_t& err){
    cout<<"subscriber.disconnect error - "<<err.what() <<endl;
    return 0 ;
  } 


  return 0;
}
