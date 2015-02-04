#include "network_device.h"

#include <winsock2.h>

#include "exception_network.h"

#pragma comment(lib, "ws2_32.lib")

namespace network {
  static std::mutex mutex;
	unsigned int Device::userCount{ 0 };
	std::unique_ptr<WSAData> Device::lpwsadata{ nullptr };
  
  Device::Device() {
		std::lock_guard<std::mutex> lock{ mutex };
    if(0 == userCount) {
      auto data = std::make_unique<WSAData>();
      if(0 != ::WSAStartup(MAKEWORD(2, 2), data.get())) {
        throw exception::make_network_exception();
      }
      lpwsadata = std::move(data);
    }
    ++userCount;
  }

  Device::~Device() {
		std::lock_guard<std::mutex> lock{ mutex };
    --userCount;
    if(0 == userCount) {
      lpwsadata.reset(nullptr);
      ::WSACleanup();
    }
  }
}