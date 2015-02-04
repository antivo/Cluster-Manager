#include "exception_network.h"

#include <winsock2.h>

#include "exception_common.h"

#pragma comment(lib, "ws2_32.lib")

namespace exception {
  // Creates exception that holds information from last available network error
  Exception make_network_exception() {
		return Exception{ WSAGetLastError() };
  }
}