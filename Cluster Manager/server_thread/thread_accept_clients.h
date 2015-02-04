#ifndef SERVER_THREAD_ACCEPT_CLIENTS
#define SERVER_THREAD_ACCEPT_CLIENTS

#include <ws2tcpip.h>

#include <atomic>
#include <thread>
#include <memory>

namespace network {
  class Listener;
}

namespace thread {
  class AcceptClients {
  public:
    AcceptClients(const std::function<void(const SOCKET)>& addClient, 
                  const std::weak_ptr<const network::Listener>& listener);
    ~AcceptClients();

    bool isActive() const;

  private:
    const std::function<void(const SOCKET)> addClient;
    const std::weak_ptr<const network::Listener> listener; 
    std::atomic<bool> running;
    std::thread task;
  
    /// throws when addClient throws
    /// <exception cref="std::runtime_error">Thrown when Listener does not exist during exection.</exception>
    void acceptClients() const;

    AcceptClients(const AcceptClients&) = delete;
    AcceptClients& operator=(const AcceptClients&) = delete;
  };
}

#endif