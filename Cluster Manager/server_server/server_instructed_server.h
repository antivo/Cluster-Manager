#ifndef SERVER_SERVER_INSTRUCTED_SERVER
#define SERVER_SERVER_INSTRUCTED_SERVER

#include <string>
#include <functional>
#include <map>
#include <memory>

namespace entity {
  class ClientRecord;
	class DeterministicFiniteAutomata;
}

namespace server {
  class LoggingServer;

  class InstructedServer {
  public:
    InstructedServer(const unsigned short listenerPort);
    ~InstructedServer();

    bool run(const std::map<std::string, const std::function<std::string(entity::ClientRecord&, const std::string&)>>& transitionFunction);
    void stop();

  private:
    std::unique_ptr<const entity::DeterministicFiniteAutomata> automata;
    std::unique_ptr<server::LoggingServer> loggingServer;

    InstructedServer(const InstructedServer&) = delete;
    InstructedServer& operator=(const InstructedServer&) = delete;
  };
}

#endif