#ifndef SERVER_LOCKED_CLIENT_DATA
#define SERVER_LOCKED_CLIENT_DATA

#include <windows.h>
#include <mutex>
#include <memory>
#include <map>

namespace entity {
  class ClientRecord;
}

namespace locked {
  
	/// Thread safe container for client data
  class ClientData {
  public:
    ClientData();
    ~ClientData();

    /// <exception cref="std::out_of_range">Thrown when fails to create data for the new client.</exception>
    void createNewClient(const SOCKET clientSocket);
    
    /// <exception cref="std::out_of_range">Thrown when fails to remove client.</exception>
    void removeClient(const SOCKET clientSocket);

    /// <exception cref="std::out_of_range">Thrown when no found client exist with given socket.</exception>
		entity::ClientRecord getClientData(const SOCKET clientSocket);
    
    /// <exception cref="std::out_of_range">Thrown when no found client exist with given socket.</exception>
    void updateClient(const SOCKET clientSocket, const entity::ClientRecord& clientData);
  
	private:
    std::mutex mutex;
		std::map<const SOCKET, std::unique_ptr<const entity::ClientRecord>> data;

		ClientData(const ClientData&) = delete;
		ClientData& operator=(const ClientData&) = delete;
  };

}

#endif