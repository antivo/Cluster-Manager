#ifndef NETWORK_DEVICE
#define NETWORK_DEVICE

#include <memory>
#include <mutex>

struct WSAData;

namespace network {
  /// RAII class to initiate use of the Winsock DLL
  class Device {
  public:
    /// <exception cref="Exception::Exception">Thrown when fails to initiate use of Winsock DLL.</exception>
    Device();
    
    ~Device();
  private:
    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;

    static unsigned int userCount;
    static std::unique_ptr<WSAData> lpwsadata;
  }; 
}

#endif