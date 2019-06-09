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
  static std::string publisherUniqueName;
  static uint64_t updateRate;
  static uint64_t payloadSize;
  static int64_t roundtripCount;
};

std::string Config::pubEndpoint;
std::string Config::subEndpoint;
std::string Config::publisherUniqueName;
uint64_t Config::updateRate;
uint64_t Config::payloadSize;
int64_t Config::roundtripCount;

int main(int argc, char *argv[])
{
  po::options_description description("Options");
  description.add_options()("help", "produce help message. Execution example - ./ZMQInitiator --PubEP tcp://127.0.0.1:4242,tcp://127.0.0.1:4243 --SubEP tcp://127.0.0.1:4241 -msgLength 1000")("PubEP", po::value(&Config::pubEndpoint)->default_value("ipc:///tmp/pingTopic"), "Publish ping (connect) to endpoint(s) (e.g. ipc:///tmp/pingTopic or tcp://127.0.0.1:4242")("SubEP", po::value(&Config::subEndpoint)->default_value("ipc:///tmp/pongTopic"), "Subscribe pong (bind) from endpoint (e.g. ipc:///tmp/pongTopic or tcp://127.0.0.1:4241")("msgLength", po::value(&Config::payloadSize)->default_value(100), "Message Length (bytes)")("roundtripCount", po::value(&Config::roundtripCount)->default_value(1000), "ping-pong intervals")("pubName", po::value(&Config::publisherUniqueName)->default_value("Initiator"), "publisher name. Name 'Dummy' will cause the echoer to not echo messages")("updateRate", po::value(&Config::updateRate)->default_value(0), "update rate (for general load purposes only)");

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

  // Open the connections
  zmq::context_t context(2);
  zmq::socket_t publisher(context, ZMQ_PUB);
  zmq::socket_t subscriber(context, ZMQ_SUB);

  cout << "Sub bind to " << Config::subEndpoint << "..." << endl;
  try
  {
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    subscriber.bind(Config::subEndpoint.c_str());
  }
  catch (const zmq::error_t &err)
  {
    cout << "subscriber.bind error - " << err.what() << endl;
    return 0;
  }

  vector<string> pubEndpoints;
  boost::split(pubEndpoints, Config::pubEndpoint, boost::is_any_of(","));
  uint64_t numSubscribers = pubEndpoints.size();
  for (size_t i = 0; i < pubEndpoints.size(); i++)
  {
    cout << "Pub connecting to " << pubEndpoints[i] << endl;
    try
    {
      publisher.connect(pubEndpoints[i].c_str());
    }
    catch (const zmq::error_t &err)
    {
      cout << "publisher.bind error - " << err.what() << endl;
      return 0;
    }
  }
  this_thread::sleep_for(chrono::seconds(2));
  cout << "Running scenario ..." << endl;

  std::chrono::time_point<std::chrono::high_resolution_clock> executionStartTime = high_resolution_clock::now();
  for (size_t i = 1; i != Config::roundtripCount; i++)
  {
    //cout<<i<<endl;
    zmq::message_t message(Config::payloadSize); //Must put message within loop
    memcpy(message.data(), Config::publisherUniqueName.c_str(), Config::publisherUniqueName.size() + 1);
    publisher.send(message, ZMQ_DONTWAIT);

    if (Config::updateRate != 0) //only for general load puropses
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(1000 / Config::updateRate));
      //cout<<i<<endl;
    }

    else
    {
      for (size_t jj = 0; jj < numSubscribers; ++jj)
      {
        // Wait for echoed message from each subscriber
        zmq::message_t receivedMessage;
        subscriber.recv(&receivedMessage);
        //cout<<"Received #" << jj <<endl;
        if (receivedMessage.size() != Config::payloadSize)
        {
          auto msg = std::string(static_cast<char *>(receivedMessage.data()), receivedMessage.size());
          std::cout << "Message of incorrect size received: " << msg.size()
                    << std::endl;
          return -1;
        }
      }
    }
  }

  //Export average latency
  std::chrono::time_point<std::chrono::high_resolution_clock> executionEndTime = high_resolution_clock::now();
  auto diff = std::chrono::duration_cast<std::chrono::microseconds>(executionEndTime - executionStartTime);
  cout << "Average latency = " << diff.count() / Config::roundtripCount / 2 << " microseconds with roundtrip count of " << Config::roundtripCount << endl;

  std::cout << "Shutting down..." << std::endl;
  try
  {
    //subscriber.disconnect(Config::subEndpoint.c_str());
    //publisher.unbind(Config::pubEndpoint.c_str());
  }
  catch (const zmq::error_t &err)
  {
    cout << "subscriber.disconnect error - " << err.what() << endl;
    return 0;
  }

  return 0;
}
