#ifndef NETWORK_SOCKET
#define NETWORK_SOCKET

#include <winsock2.h>

#include <memory>

namespace network {
  class Device;
  
  /// RAII class to contain file descriptor of socket
  class Socket {
  public:
    /// <exception cref="network::Exception">Thrown when fails to create socket.</exception>
    Socket(const int adressFamily, const int type, const int protocol);
    ~Socket();
    
    /// get handle
    SOCKET get() const;
  private:
    std::unique_ptr<Device> device;
    SOCKET descriptor;
    
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
  };
}

#endif