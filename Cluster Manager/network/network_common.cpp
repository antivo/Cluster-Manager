#include "network_common.h"

#include <winsock2.h>

#include "assert_common.h"
#include "exception_network.h"
#include "memory_common.h"

#pragma comment(lib, "ws2_32.lib")

namespace network {
  void send(const SOCKET socket, const char* message, const message_size& messageSize) {
		message_size sentData { 0 };
    do {
      int status = ::send(socket, message + sentData, messageSize - sentData, 0);
			assert::condition(SOCKET_ERROR != status, exception::make_network_exception);
      sentData += status;
    } while (sentData < messageSize);
  }

  void recieve(const SOCKET socket, char* destination, const message_size& messageSize) {
		message_size recievedData{ 0 };
    do {
      auto status = ::recv(socket, destination + recievedData, messageSize - recievedData, 0);
      assert::condition(!(SOCKET_ERROR == status || 0 == status), exception::make_network_exception);
      recievedData += status;
    } while(recievedData < messageSize);
  }
  
  void sendMessage(const SOCKET socket, const std::string& message) {
		message_size messageSize{ message.length() };
    messageSize = htonl(messageSize); // The htonl function converts a u_long from host to TCP/IP network byte order (which is big-endian).
    network::send(socket, reinterpret_cast<const char*>(&messageSize), sizeof(messageSize));
    messageSize = ntohl(messageSize); // The ntohl function converts a u_long from TCP/IP network order to host byte order (which is little-endian on Intel processors)
    network::send(socket, message.c_str(), messageSize);
  }
  
  std::string recieveMessage(const SOCKET socket) { // OBRISI MESSAGECHUNK, ostalo radi kompatibilnosti
    message_size messageSize;
    recieve(socket, reinterpret_cast<char*>(&messageSize), sizeof(messageSize));
    messageSize = ntohl(messageSize);
    memory::DynamicArray<char> buffer(messageSize + 1);
    recieve(socket, buffer.get(), messageSize);
    (buffer.get())[messageSize] = '\0';
    return std::string(buffer.get());
  }
}