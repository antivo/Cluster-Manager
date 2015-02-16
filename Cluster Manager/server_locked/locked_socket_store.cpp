#include "locked_socket_store.h"

#include "assert_common.h"

#pragma comment(lib, "ws2_32.lib")

namespace locked {
  SocketStore::SocketStore() {
    FD_ZERO(&store);
  }

  SocketStore::~SocketStore() {}

  void SocketStore::addSocket(const SOCKET socket) {
    set(socket);
		assert::condition(isSet(socket), []() { return std::out_of_range("Socket Store request to insert socket failed."); });
  }

  void SocketStore::removeSocket(const SOCKET socket) {
		assert::condition(isSet(socket), []() { return std::out_of_range("Socket Store request to delete socket that does not exist."); });
    unset(socket);
  }

  fd_set SocketStore::getCopy() {
    mutex.lock();
    auto copy = store;
    mutex.unlock();
    return copy;
  }
  

  void SocketStore::set(const SOCKET socket) {
    mutex.lock();
    FD_SET(socket, &store);
    mutex.unlock();
  }

  void SocketStore::unset(const SOCKET socket) {
    mutex.lock();
    FD_CLR(socket, &store);
    mutex.unlock();
  }

  bool SocketStore::isSet(const SOCKET socket) {
    mutex.lock();
    auto isset = (0 != FD_ISSET(socket, &store));
    mutex.unlock();
    return isset;
  }
}