#ifndef SERVER_LOCKED_SOCKET_STORE
#define SERVER_LOCKED_SOCKET_STORE

#include <ws2tcpip.h>

#include <mutex>
#include <vector>

namespace locked {
	/// Thread safe socket container
  class SocketStore {
  public:
    SocketStore();
    ~SocketStore();

    /// <exception cref="std::out_of_range">Thrown when fails to insert socket.</exception>
    void addSocket(const SOCKET socket);
    /// <exception cref="std::out_of_range">Thrown when fails to remove socket.</exception>
    void removeSocket(const SOCKET socket);
    /// <exception cref="noexcept">Does not throw</exception>
    fd_set getCopy();

  private:
    std::mutex mutex;
    fd_set store;

    /// <exception cref="noexcept">Does not throw</exception>
    void set(const SOCKET socket);
    /// <exception cref="noexcept">Does not throw</exception>
    void unset(const SOCKET socket);
    /// <exception cref="noexcept">Does not throw</exception>
    bool isSet(const SOCKET socket);

    SocketStore(const SocketStore&) = delete;
    SocketStore& operator=(const SocketStore&) = delete;
  };
}

#endif