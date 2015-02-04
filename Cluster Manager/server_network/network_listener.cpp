#include "network_listener.h"

#include <winsock2.h>

#include <cstdio>

#include "exception_network.h"
#include "network_connection.h"
#include "network_common.h"
#include "network_socket.h"

#pragma comment(lib, "ws2_32.lib")

namespace network {
  Listener::Listener(const unsigned short port) : port{port}, socket{(std::make_unique<const Socket>(AF_INET, SOCK_STREAM, IPPROTO_TCP))} {
    sockaddr_in server{AF_INET, htons(port), {INADDR_ANY}};
    if (0 != ::bind(socket->get(), reinterpret_cast<sockaddr*>(&server), sizeof(server))) {
      throw exception::make_network_exception();
    }
    if (0 != listen(socket->get(), SOMAXCONN)) {
      throw exception::make_network_exception();
    }
  }

  Listener::~Listener() {
    ::shutdown(socket->get(), SD_BOTH);
  }

  SOCKET Listener::acceptClient() const {
    SOCKET clientSocket = ::accept(socket->get(), nullptr, nullptr);
    if (INVALID_SOCKET == clientSocket) {
      throw exception::make_network_exception();
    } 
    return clientSocket;
  }

  void Listener::closeClient(const SOCKET client) const {
    auto result = ::closesocket(client);
    if (0 != result) {
      throw exception::make_network_exception();
    } 
  }

   void Listener::pingSelf() const {
    char buffer [10];
    sprintf_s (buffer, "%d", port);
    network::Connection("localhost", buffer);
  }
    
  void Listener::selectClients(fd_set& clientSet) const {
    timeval waitTime{0, 0}; 
    if (0 != clientSet.fd_count) {
      auto result = select(clientSet.fd_count, &clientSet, nullptr, nullptr, &waitTime); // checks readability
      if (SOCKET_ERROR == result) { 
        throw exception::make_network_exception();
      }
    }
  }

  void Listener::selectBroken(fd_set& clientSet) const {
    timeval waitTime{0, 0}; 
    if (0 != clientSet.fd_count) {
      auto result = select(clientSet.fd_count, nullptr, nullptr, &clientSet, &waitTime); // checks readability
      if (SOCKET_ERROR == result) { 
        throw exception::make_network_exception();
      }
    }
  }

  void Listener::send(const SOCKET socket, const std::string& message) const {
    network::sendMessage(socket, message);
  }

  std::string Listener::recieve(const SOCKET socket) const {
    return network::recieveMessage(socket);
  }
}

