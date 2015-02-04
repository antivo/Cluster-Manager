#ifndef NETWORK_LISTENER
#define NETWORK_LISTENER

#include <ws2tcpip.h>
#include <string>
#include <memory>

namespace network {
  class Socket;

  /// RAII class that encapsulates listening socket, provides actions that can be done with listening socket
  class Listener {
  public:
    explicit Listener(const unsigned short port);
    ~Listener();
   
   /// <exception cref="exception::Exception">Thrown when fails to accept client.</exception>
   SOCKET acceptClient() const;

   /// <exception cref="exception::Exception">Thrown when fails to remove client.</exception>
   void closeClient(const SOCKET client) const;

   /// <exception cref="exception::Exception">Thrown when fails to accept client.</exception>
   void pingSelf() const;

   /// <exception cref="exception::Exception">Thrown when fails to select readable sockets.</exception>
   void selectClients(fd_set& clientSet) const;

   /// <exception cref="exception::Exception">Thrown when fails to select sockets with errors.</exception>
   void selectBroken(fd_set& clientSet) const;

   /// <exception cref="exception::Exception">Thrown when fails to send message.</exception>
   void send(const SOCKET socket, const std::string& message) const;

   /// <exception cref="exception::Exception">Thrown when fails to send message.</exception>
   std::string recieve(const SOCKET socket) const;
  
	private:
    const unsigned short port;
    const std::unique_ptr<const Socket> socket;

    Listener(const Listener& other) = delete;
    Listener& operator=(const Listener& other) = delete;
  };
}

#endif