#include "network_connection.h"

#include <ws2tcpip.h>
#include <winsock2.h>

#include "exception_common.h"
#include "exception_network.h"
#include "memory_common.h"
#include "network_common.h"
#include "network_device.h"
#include "network_socket.h"

#pragma comment(lib, "ws2_32.lib")

namespace network {
  Connection::Connection(const char* hostname, const char* service) : socket{ nullptr } {
    addrinfo hints{0, AF_INET, SOCK_STREAM, IPPROTO_TCP};
    auto deleter = [](PADDRINFOA* pointer) { freeaddrinfo(*pointer); delete pointer; pointer = nullptr; };
    auto result = memory::make_unique_with_deleter<PADDRINFOA, decltype(deleter)>(deleter, nullptr);
    Device device;
    int status = ::getaddrinfo(hostname, service, &hints, result.get());
    if (0 != status) {
      throw exception::make_network_exception();
    }
    addrinfo* res = *result;
    socket = std::make_unique<Socket>(res->ai_family, res->ai_socktype, res->ai_protocol);
    status = ::connect(socket->get(), res->ai_addr, res->ai_addrlen);
    if (SOCKET_ERROR == status) {
      throw exception::make_network_exception();
    }
  }

  Connection::~Connection() {
    ::shutdown(socket->get(), SD_BOTH);
  }
  
  void Connection::send(const std::string& message) const {
     network::sendMessage(socket->get(), message);
  }
  
  std::string Connection::receive() const  {
    return network::recieveMessage(socket->get());
  }
}