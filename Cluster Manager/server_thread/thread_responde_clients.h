#ifndef SERVER_THREAD_RESPONDE_CLIENTS
#define SERVER_THREAD_RESPONDE_CLIENTS

#include <ws2tcpip.h>

#include <atomic>
#include <memory>
#include <thread>

namespace network {
  class Listener;
}

namespace thread {
  class RespondeClients {
  public:
    RespondeClients(const std::function<void(const SOCKET)>& serveClient, 
                    const std::function<void(const SOCKET)>& closeClient,
                    const std::function<fd_set(void)>& getClients,
                    const std::weak_ptr<const network::Listener>& listener);
    ~RespondeClients();

    bool isActive() const;

  private:
    const std::function<void(const SOCKET)> serveClient;
    const std::function<void(const SOCKET)> closeClient;
    const std::function<fd_set(void)> getClients;
    const std::weak_ptr<const network::Listener> listener; 
    std::atomic<bool> running;
    std::thread task;
    
    /// <exception cref="std::runtime_error">Thrown when Listener || SocketStore are not available during execution.</exception>
    /// throws: if fails to serve client and then close it
    /// throws when fails to close broken socket
    void respondeClients() const;

    /// <exception cref="std::runtime_error">Thrown when Listener || SocketStore are not available during execution.</exception>
    /// throws: if fails to serve client and then close it
    void serveIfRunning(void (network::Listener::* listenerSelectMethod)(fd_set&) const, const std::function<void(const SOCKET)>& action) const;

    /// throw when getClients throws
    /// <exception cref="std::runtime_error">Thrown when Listener SocketStore is not available during execution.</exception>
    fd_set selectSocketSet(void (network::Listener::* listenerSelectMethod)(fd_set&) const) const;

    /// throws when serveClient throws
    /// throws when closeClient throws
    void processClient(const SOCKET client) const;

    RespondeClients(const RespondeClients&) = delete;
    RespondeClients& operator=(const RespondeClients&) = delete;
  };
}

#endif