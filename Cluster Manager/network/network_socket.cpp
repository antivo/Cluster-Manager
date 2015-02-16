#include "network_socket.h"
#include "network_device.h"

#include <winsock2.h>

#include "assert_common.h"
#include "exception_network.h"

#pragma comment(lib, "ws2_32.lib")

namespace network {
  Socket::Socket(const int adressFamily, const int type, const int protocol) : 
    device{ std::make_unique<Device>() }, 
    descriptor{ ::socket(adressFamily, type, protocol) } 
  {
    assert::condition(INVALID_SOCKET != descriptor, exception::make_network_exception);
  }

  Socket::~Socket() {
    ::closesocket(descriptor);
  }

  SOCKET Socket::get() const {
    return this->descriptor;
  }
}