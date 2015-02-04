#include "server_instructed_server.h"

#include <memory>

#include "entity_client_record.h"
#include "entity_deterministic_finite_automata.h"
#include "server_logging_server.h"

namespace server {
  InstructedServer::InstructedServer(const unsigned short listenerPort) :
      automata(nullptr),
      loggingServer(std::make_unique<server::LoggingServer>(listenerPort))
  {}

  InstructedServer::~InstructedServer() {}

  bool InstructedServer::run(const std::map<std::string, const std::function<std::string(entity::ClientRecord&, const std::string&)>>& transitionFunction) {
		bool started{ false };
    if(!automata) {
      automata = std::make_unique<const entity::DeterministicFiniteAutomata>(transitionFunction);
      const auto howToServe = [this](entity::ClientRecord& client, const std::string& recieved) -> std::string {return automata->transit(client, recieved);};
      started = loggingServer->run(howToServe); 
    } 
    return started;
  }

  void InstructedServer::stop() {
    loggingServer->stop();
    automata.reset(nullptr);
  }
}