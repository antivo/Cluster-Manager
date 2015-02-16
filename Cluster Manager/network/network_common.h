#ifndef NETWORK_COMMON
#define NETWORK_COMMON

#include <winsock2.h>

#include <istream>
#include <string>
#include <memory>

/// To be used for determining message size or message chunk values
typedef std::size_t message_size;

namespace network {
  /// low level function to send message pointed by message of size messageSize
  /// <exception cref="network::Exception">Thrown when fails to send message.</exception>
  void send(const SOCKET socket, const char* message, const message_size& messageSize);

  /// low level function to recieve message pointed by message of size messageSize
  /// <exception cref="network::Exception">Thrown when fails to recieve message.</exception>
 void recieve(const SOCKET socket, char* destination, const message_size& messageSize);
  
  /// Function to send entire message
  /// socket  - representation of socket on which to write message
  /// message - message container
  /// <exception cref="network::Exception">Thrown when fails to send message.</exception>
  void sendMessage(const SOCKET socket, const std::string& message);

  /// Function to receive entire messages 
  /// socket  - representation of socket on which to write message
  /// messageChunk - determine maximum size of buffers per read on reading the message
  /// <returns>Returns message container.</returns>
  /// <exception cref="network::Exception">Thrown when fails to send message.</exception>
  /// <exception cref="bad_alloc">Thrown when fails to allocate space for incomming message.</exception>
  std::string recieveMessage(const SOCKET socket);
}

#endif