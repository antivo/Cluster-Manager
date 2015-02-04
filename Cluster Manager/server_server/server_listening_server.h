#ifndef SERVER_SERVER_LISTENING_SERVER
#define SERVER_SERVER_LISTENING_SERVER

#include <ws2tcpip.h>
#include <memory>
#include <string>
#include <functional>

namespace network {
 class Listener;
}

namespace thread {
  class AcceptClients;
  class RespondeClients;
}

namespace server {
  class ListeningServer {
  public:
    ListeningServer(const unsigned short listener_port);
    ~ListeningServer();

    bool run(const std::function<void(const SOCKET)>& addClient, 
             const std::function<void(const SOCKET)>& closeClient,
             const std::function<void(const SOCKET)>& serveClient,
             const std::function<fd_set(void)>& getClients);
    void stop();

    // throws
    void sendToClient(const SOCKET client, const std::string& message) const;

    //throws
    std::string recieveFromClient(const SOCKET client) const;
  private:
    const std::shared_ptr<const network::Listener> listener;
    std::unique_ptr<const thread::RespondeClients> respondeClients;
    std::unique_ptr<const thread::AcceptClients> acceptClients;
    
    

    ListeningServer(const ListeningServer&) = delete;
    ListeningServer& operator=(const ListeningServer&) = delete;
  };
}
#endif