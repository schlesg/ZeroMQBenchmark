
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
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <string>

using json = nlohmann::json;
using namespace std;
using namespace std::chrono;
namespace po = boost::program_options;

class Config
{
public:
  static std::string pubEndpoint;
  static std::string subEndpoint;
};

std::string Config::pubEndpoint;
std::string Config::subEndpoint;


int main(int argc, char *argv[])
{
  po::options_description description("Options");
  description.add_options()
  ("help", "produce help message")
  ("PubEP", po::value(&Config::pubEndpoint)->default_value("ipc:///tmp/pongTopic"), "Publish ping (connect) to endpoint(s) (e.g. ipc:///tmp/pongTopic or tcp://127.0.0.1:4241")
  ("SubEP", po::value(&Config::subEndpoint)->default_value("ipc:///tmp/pingTopic"), "Subscribe (bind) ping from endpoint (e.g. ipc:///tmp/pingTopic or tcp://127.0.0.1:4242");

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

  cout << "Sub bind to " << Config::subEndpoint << "..." << endl;
  try {
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    subscriber.bind(Config::subEndpoint.c_str());
  }
  catch (const zmq::error_t& err){
    cout<<"subscriber.bind error - "<<err.what() <<endl;
    return 0 ;
  } 


  vector<string> pubEndpoints;
  boost::split(pubEndpoints,Config::pubEndpoint,boost::is_any_of(","));
  uint64_t numSubscribers = pubEndpoints.size();
  for (size_t i = 0; i < pubEndpoints.size(); i++){
    cout << "Pub connecting to " << pubEndpoints[i] << endl;
    try{
      publisher.connect(pubEndpoints[i].c_str());
    }
    catch (const zmq::error_t& err){
      cout<<"publisher.bind error - "<<err.what() <<endl;
      return 0 ;
    }
  }

  size_t i = 0;
  cout << "Echoer initialized" << endl;

  while (true)
  {
    //No callbacks in ZMQ
    i++;
    zmq::message_t message;
    subscriber.recv(&message);
    if (strcmp((char*)message.data(), "Dummy") == 0){
      //std::cout << "Dummy message #" << i << std::endl;
      continue; //skipping echo case dummy publisher for load purposes
    }
    else{
      publisher.send(message, ZMQ_DONTWAIT);
      //std::cout << "message #" << i << std::endl;
    }
  }

  try {
    //subscriber.disconnect(Config::subEndpoint.c_str());
    //publisher.unbind(Config::pubEndpoint.c_str());
  }
  catch (const zmq::error_t& err){
    cout<<"subscriber.disconnect error - "<<err.what() <<endl;
    return 0 ;
  } 


  return 0;
}
