#ifndef SERVER_SERVER_LOGGING_SERVER
#define SERVER_SERVER_LOGGING_SERVER

#include <ws2tcpip.h>
#include <string>
#include <memory>
#include <functional>

namespace entity {
  class ClientRecord;
}

namespace locked {
  class SocketStore;
  class ClientData;
}

namespace database {
  class ControlerServerSide;
}

namespace server {
  class ListeningServer;
  
  class LoggingServer{
  public:
    LoggingServer(const unsigned short listenerPort);
    ~LoggingServer();

    bool run(const std::function<std::string(entity::ClientRecord&, const std::string&)>& process);
    void stop();
  private:
    std::shared_ptr<locked::SocketStore> socketStore;
    std::shared_ptr<locked::ClientData> clientData;
    std::unique_ptr<server::ListeningServer> listeningServer;
    
    void addClient(const SOCKET client) const;
    void closeClient(const SOCKET client) const;
    fd_set getClients() const;
    void serveClient(const SOCKET client, const std::function<std::string(entity::ClientRecord&, const std::string&)>& process) const;

    LoggingServer(const LoggingServer&) = delete;
    LoggingServer& operator=(const LoggingServer&) = delete;
  };
}
#endif