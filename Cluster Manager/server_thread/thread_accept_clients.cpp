#include "thread_accept_clients.h"

#include <ws2tcpip.h>

#include "assert_common.h"
#include "network_listener.h"

namespace thread {
  AcceptClients::AcceptClients(const std::function<void(const SOCKET)>& addClient, 
                               const std::weak_ptr<const network::Listener>& listener) : 
    addClient(addClient), 
    listener(listener), 
    running(true), 
    task([this](){  try {
                        this->acceptClients();
                      } catch(...) { 
                        //TODO log  
                      }
                      running = false;
                 }) 
  {}
   
  AcceptClients::~AcceptClients() {
    running = false;
    if(auto listenerSolid = listener.lock()) {
      try {
        listenerSolid->pingSelf();
      } catch(...) {
        //swallow exceptin
      }
    }
    task.join();
  }

  bool AcceptClients::isActive() const {
    return running;
  }

  void AcceptClients::acceptClients() const {
    while(running) {
      auto listenerSolid = listener.lock();
			assert::condition(nullptr != listenerSolid, []() { return std::runtime_error("Listener is not available for thread::AcceptClient::acceptClients"); });
      auto client = listenerSolid->acceptClient();
      if(running) {
        addClient(client);
      }
    }
  }
}