#ifndef CLIENT_NETWORK_CONNECTION
#define CLIENT_NETWORK_CONNECTION

#include <ws2tcpip.h>

#include <memory>
#include <string>

namespace network {
  class Socket;

  /// RAII class to represent network connection
  class Connection {
  public:
    /// <exception cref="exception::Exception">Thrown when fails to connect to destination.</exception>
    Connection(const char* hostname, const char* service);
    ~Connection();
    
    /// <exception cref="exception::Exception">Thrown when fails to send message to destination.</exception>
    void send(const std::string& message) const;

    /// <exception cref="exception::Exception">Thrown when fails to receive message to destination.</exception>
    std::string receive() const;

  private:
    std::unique_ptr<Socket> socket;
    
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;
  };
}

#endif