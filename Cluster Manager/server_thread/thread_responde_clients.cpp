#include "thread_responde_clients.h"

#include <iterator>

#include "network_listener.h"

namespace thread {
  RespondeClients::RespondeClients(
    const std::function<void(const SOCKET)>& serveClient, 
    const std::function<void(const SOCKET)>& closeClient, 
    const std::function<fd_set(void)>& getClients, 
    const std::weak_ptr<const network::Listener>& listener) : 
      serveClient(serveClient), 
      closeClient(closeClient), 
      getClients(getClients), 
      listener(listener), 
      running(true),
      task([this](){  try {
                        this->respondeClients();
                      } catch(...) { 
                         //TODO log  
                      }
                      running = false;
                   }) 
   {}

  RespondeClients::~RespondeClients() {
    running = false;
    task.join();
  }

  bool RespondeClients::isActive() const{
    return running;
  }

  void RespondeClients::respondeClients() const {
    const std::function<void(const SOCKET)> process = std::bind(&thread::RespondeClients::processClient, this, std::placeholders::_1);
    while (running) {
      serveIfRunning(&network::Listener::selectClients, process);
      if(running) {
        serveIfRunning(&network::Listener::selectBroken, closeClient);
      }
    }
  }

  void RespondeClients::serveIfRunning(void (network::Listener::* listenerSelectMethod)(fd_set&) const, const std::function<void(const SOCKET)>& action) const {
    auto clientSet = selectSocketSet(listenerSelectMethod);
    auto range = clientSet.fd_count;
    for(decltype(range) clientIndex = 0; running && (clientIndex < range); ++clientIndex) {
      auto client = clientSet.fd_array[clientIndex];
      action(client);
    }
  }
  
  fd_set RespondeClients::selectSocketSet(void (network::Listener::* listenerSelectMethod)(fd_set&) const) const {
    auto listenerSolid = listener.lock();
    if(listenerSolid) {
      auto clientSet = getClients();
      auto selector = std::bind(listenerSelectMethod, listenerSolid.get(), std::placeholders::_1);
      selector(clientSet);
      return clientSet;
    } else {
       throw std::runtime_error("Listener not available inside thread::RespondeClients::selectSocketSet");
    }
  }

  void RespondeClients::processClient(const SOCKET client) const {
      try {
        serveClient(client);
      } catch (...) {
        closeClient(client);
        auto listenerSolid = listener.lock();
        if(listenerSolid) {
          listenerSolid->closeClient(client);
        } else {
          throw std::runtime_error("Listener not available in RespondeClients::processClient");
        }
      }
  }
}