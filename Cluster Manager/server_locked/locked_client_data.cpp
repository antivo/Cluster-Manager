#include "locked_client_data.h"

#include "assert_common.h"
#include "entity_client_record.h"

namespace locked {
  ClientData::ClientData() {}
  ClientData::~ClientData() {}

  void ClientData::createNewClient(const SOCKET clientSocket) {
    auto newData = std::make_unique<const entity::ClientRecord>();
    mutex.lock();
    auto result = data.emplace(clientSocket, std::move(newData));
    mutex.unlock();
		assert::condition(result.second, []() { return std::out_of_range("Client Data request to insert new data failed."); });
  }

  void ClientData::removeClient(const SOCKET clientSocket) {
    mutex.lock();
    auto result = data.erase(clientSocket);
    mutex.unlock();
		assert::condition(result, []() { return std::out_of_range("Client Data request to remove client failed."); });
  }

  entity::ClientRecord ClientData::getClientData(const SOCKET clientSocket) {
    std::lock_guard<std::mutex> guard(mutex);
    return *data.at(clientSocket);
  }
   
  void ClientData::updateClient(const SOCKET clientSocket, const entity::ClientRecord& clientData) {
    auto updated = std::make_unique<const entity::ClientRecord>(clientData);
    std::lock_guard<std::mutex> guard(mutex);
    data.at(clientSocket) = std::move(updated);
  }
}