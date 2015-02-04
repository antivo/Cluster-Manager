#ifndef SERVER_SERVER_DETAILED_SERVER
#define SERVER_SERVER_DETAILED_SERVER

#include <string>
#include <functional>
#include <map>
#include <memory>

namespace entity {
  class ClientRecord;
}

namespace configuration {
	class SharedClientServer;
	class SharedServerManager;
}

namespace persistent {
	class ServerData;
}

namespace server {
  class InstructedServer;

  class DetailedServer {
  public:
    DetailedServer(unsigned short listenerPort, 
			std::shared_ptr<configuration::SharedClientServer> clientServerConf,
			std::shared_ptr<configuration::SharedServerManager> serverManagerConf);

    ~DetailedServer();

    bool run(const std::shared_ptr<persistent::ServerData>& persistentServerData);
    void stop();

  private:
		std::shared_ptr<persistent::ServerData> persistentData;
		std::shared_ptr<const configuration::SharedClientServer> clientServerConf;
		std::shared_ptr<const configuration::SharedServerManager> serverManagerConf;

		std::unique_ptr<std::map<std::string, const std::function<std::string(entity::ClientRecord& data, const std::string& message)>>> transitionMap;
    std::unique_ptr<server::InstructedServer> instructedServer;

    std::string username(entity::ClientRecord& data, const std::string& message) const;
    std::string password(entity::ClientRecord& data, const std::string& message) const;
    std::string idle(entity::ClientRecord& data, const std::string& message) const;
    std::string receiving(entity::ClientRecord& data, const std::string& message) const;
    std::string recvFiles(entity::ClientRecord& data, const std::string& message) const;
    std::string sending(entity::ClientRecord& data, const std::string& message) const;
    std::string sendFiles(entity::ClientRecord& data, const std::string& message) const;
    
    DetailedServer(const DetailedServer&) = delete;
    DetailedServer& operator=(const DetailedServer&) = delete;
  };
}

#endif