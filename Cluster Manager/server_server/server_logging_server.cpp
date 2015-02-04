#include "server_logging_server.h"
#include "server_listening_server.h"

#include "entity_client_record.h"

#include "locked_socket_store.h"
#include "locked_client_data.h"

namespace server {
  LoggingServer::LoggingServer(const unsigned short listenerPort) : 
		socketStore{ std::make_shared<locked::SocketStore>() },
		clientData{ std::make_shared<locked::ClientData>() },
		listeningServer{ std::make_unique<server::ListeningServer>(listenerPort) }
  {}

  LoggingServer::~LoggingServer() {}

  bool LoggingServer::run(const std::function<std::string(entity::ClientRecord&, const std::string&)>& process) {
    const auto add = std::bind(&server::LoggingServer::addClient, this, std::placeholders::_1);
    const auto close = std::bind(&server::LoggingServer::closeClient, this, std::placeholders::_1);
    const auto serve = std::bind(&server::LoggingServer::serveClient, this, std::placeholders::_1, process);
    const auto getClientSet = std::bind(&server::LoggingServer::getClients, this);
    return listeningServer->run(add, close, serve, getClientSet);
  }

  void LoggingServer::stop() {
    listeningServer->stop();
  }

  void LoggingServer::serveClient(const SOCKET client, const std::function<std::string(entity::ClientRecord&, const std::string&)>& process) const {
    const auto recievedMessage = listeningServer->recieveFromClient(client);
    auto data = clientData->getClientData(client);
    const auto respondeMessage = process(data, recievedMessage);
    listeningServer->sendToClient(client, respondeMessage);
    clientData->updateClient(client, data);
  }

  void LoggingServer::addClient(const SOCKET client) const {
    socketStore->addSocket(client);
    clientData->createNewClient(client);
  }

   void LoggingServer::closeClient(const SOCKET client) const {
    clientData->removeClient(client);
    socketStore->removeSocket(client);
  }

  fd_set LoggingServer::getClients() const {
    return socketStore->getCopy();
  } 
}