#include "server_listening_server.h"

#include "network_listener.h"
#include "thread_accept_clients.h"
#include "thread_responde_clients.h"

namespace server {
  ListeningServer::ListeningServer(const unsigned short listener_port) : 
    listener(std::make_shared<const network::Listener>(listener_port)),
    respondeClients(nullptr),
    acceptClients(nullptr)
  {}

  ListeningServer::~ListeningServer() {
    respondeClients.reset(nullptr);
    acceptClients.reset(nullptr);
  }

  bool ListeningServer::run(const std::function<void(const SOCKET)>& addClient, 
                            const std::function<void(const SOCKET)>& closeClient,
                            const std::function<void(const SOCKET)>& serveClient,
                            const std::function<fd_set(void)>& getClients) 
  {
    auto started = false; 
    if(!acceptClients || !respondeClients || !acceptClients->isActive() || !respondeClients->isActive()) {
      acceptClients = std::make_unique<const thread::AcceptClients>(addClient, listener);
      respondeClients = std::make_unique<const thread::RespondeClients>(serveClient, closeClient, getClients, listener);
      started = true;
    }
    return started;
  }

  void ListeningServer::stop() {
    respondeClients.reset(nullptr);
    acceptClients.reset(nullptr);
  }

  void ListeningServer::sendToClient(const SOCKET client, const std::string& message) const {
    listener->send(client, message);
  }

  std::string ListeningServer::recieveFromClient(const SOCKET client) const {
    return listener->recieve(client);
  }
}