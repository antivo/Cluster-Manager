#include "server_detailed_server.h"

#include "common_assert.h"
#include "configuration_shared_client_server.h"
#include "configuration_shared_server_manager.h"
#include "configuration_constants.h"
#include "entity_contract.h"
#include "entity_client_record.h"
#include "filesystem_job_directory.h"
#include "persistent_server_data.h"
#include "utility_serialization.h"
#include "utility_string.h"
#include "server_instructed_server.h"

namespace server {
	
	namespace {
		constexpr auto USERNAME_PROPERTY = "username";
		constexpr auto CLIENTID_PROPERTY = "clientId";
		constexpr auto TO_SEND_PROPERY = "contractVector";
		constexpr auto POINTER_PROPERTY = "pointer";
		constexpr auto JOB_NAME_PROPERTY = "jobName";
		
		constexpr auto POINTER_INITIAL_NUM = 0;
		const auto POINTER_INIT = std::to_string(POINTER_INITIAL_NUM);
	
		constexpr auto PASSWORD_STATE = "password";
		constexpr auto IDLE_STATE = "idle";
		constexpr auto RECEIVING_STATE = "receiving";
		constexpr auto RECV_FILES_STATE = "recvFiles";
		constexpr auto SENDING_STATE = "sending";
		constexpr auto SEND_PATH_STATE = "sendPath";
		constexpr auto SEND_FILES_STATE = "sendFiles";
	}

	DetailedServer::DetailedServer(unsigned short listenerPort,
																	 std::shared_ptr<configuration::SharedClientServer> clientServerConf,
																	 std::shared_ptr<configuration::SharedServerManager> serverManagerConf) :
		clientServerConf(clientServerConf),
		serverManagerConf(serverManagerConf),
		transitionMap(nullptr), 
		instructedServer(std::make_unique<server::InstructedServer>(listenerPort)),
		persistentData(nullptr)
	{}

	DetailedServer::~DetailedServer() {}

	void DetailedServer::stop() {
		instructedServer->stop();
	}

	bool DetailedServer::run(const std::shared_ptr<persistent::ServerData>& persistentServerData) {
		using std::placeholders::_1;
		using std::placeholders::_2;
		this->persistentData = persistentServerData;
		transitionMap = std::make_unique<std::map<std::string, const std::function<std::string(entity::ClientRecord&, const std::string&)>>>();
		transitionMap->emplace(entity::ClientRecord::getStartingState(), std::bind(&server::DetailedServer::username, this, _1, _2));
		transitionMap->emplace(PASSWORD_STATE,		std::bind(&server::DetailedServer::password, this, _1, _2));
		transitionMap->emplace(IDLE_STATE,				std::bind(&server::DetailedServer::idle, this, _1, _2));
		transitionMap->emplace(RECEIVING_STATE,	  std::bind(&server::DetailedServer::receiving, this, _1, _2));
		transitionMap->emplace(RECV_FILES_STATE,	std::bind(&server::DetailedServer::recvFiles, this, _1, _2));
		transitionMap->emplace(SENDING_STATE,			std::bind(&server::DetailedServer::sending, this, _1, _2));
		transitionMap->emplace(SEND_FILES_STATE,  std::bind(&server::DetailedServer::sendFiles, this, _1, _2));
		return instructedServer->run(*transitionMap);
	}

  std::string DetailedServer::username(entity::ClientRecord& data, const std::string& message) const {
		assert::stringNotEmpty(message, "Client sended empty message");
		data.setProperty(USERNAME_PROPERTY, message);
		data.setState(PASSWORD_STATE);
		return clientServerConf->getSucessMessage();
   }

  std::string DetailedServer::password(entity::ClientRecord& data, const std::string& message) const {
		assert::stringNotEmpty(message, "Client sended empty message");
    const auto username = data.getProperty(USERNAME_PROPERTY);
    const auto clientId = persistentData->getClientID(username, message);
		data.setProperty(CLIENTID_PROPERTY, clientId);
    data.setState(IDLE_STATE);
		return clientServerConf->getSucessMessage();
  }

  std::string DetailedServer::idle(entity::ClientRecord& data, const std::string& message) const {
		assert::stringNotEmpty(message, "Client sended empty message");
    if (!message.compare(configuration::CLIENT_CHOICE_RECEIVING)) {
      data.setState(RECEIVING_STATE);
    } else if (!message.compare(configuration::CLIENT_CHOICE_SENDING)) {
      data.setState(SENDING_STATE);
    } else if (!message.compare(configuration::CLIENT_CHOICE_INFO)) {
      const auto clientId = data.getProperty(CLIENTID_PROPERTY);
      return persistentData->getInfo(clientId);
    } else {
      throw std::runtime_error("Client demanded unsupported action");
    }
    return clientServerConf->getSucessMessage();
  }

  std::string DetailedServer::receiving(entity::ClientRecord& data, const std::string& message) const {
		assert::stringNotEmpty(message, "Client sended empty message");
		const auto contractVector = utility::deserialize(message);
		auto contract = std::make_shared<entity::Contract>(contractVector);
		const auto clientID = data.getProperty(CLIENTID_PROPERTY);
    const auto jobName = contract->getJobName();
    if(!persistentData->jobExists(clientID, jobName)) {
			data.setContract(contract);
			data.setProperty(POINTER_PROPERTY, POINTER_INIT);
			data.setState(RECV_FILES_STATE);
			return clientServerConf->getSucessMessage();
		} else {
			return clientServerConf->getErrorMessage();
		}
  }

  std::string DetailedServer::recvFiles(entity::ClientRecord& data, const std::string& message) const {
    const auto clientId = data.getProperty(CLIENTID_PROPERTY);
		const auto contract = data.getContract();
		const auto jobName = contract->getJobName();
		auto pathListPointer = std::stoi(data.getProperty(POINTER_PROPERTY));
		const auto filePath = contract->getPathList().at(pathListPointer);
    const auto jobDirectory = std::make_unique<filesystem::JobDirectory>(serverManagerConf->getRootPath(), clientId, jobName);
		try {
			jobDirectory->prepareForDataIncome(serverManagerConf->getDataDirectoryName());
      jobDirectory->storeData(filePath, message);
			++pathListPointer;
			if(pathListPointer == data.getContract()->getPathList().size()) {
				data.setContract(nullptr);
				data.eraseProperty(POINTER_PROPERTY);
				persistentData->storeJob(clientId, jobName, contract->getJobType(), contract->getNeededWorkers(), contract->getArgs(), contract->getExecutable());
				data.setState(IDLE_STATE);
      } else {
        data.setProperty(POINTER_PROPERTY, std::to_string(pathListPointer));
      }
    } catch(...) {
      jobDirectory->remove();
      throw;
    }
		return clientServerConf->getSucessMessage();
  }

	/// receives job name
	/// sends string vector (vector of strings)
  std::string DetailedServer::sending(entity::ClientRecord& data, const std::string& message) const {
		assert::stringNotEmpty(message, "Client sended empty message");
		const auto clientId = data.getProperty(CLIENTID_PROPERTY);
    const auto& jobName = message;
    if(persistentData->jobFinished(clientId, jobName)) {
			data.setProperty(JOB_NAME_PROPERTY, jobName);
			const auto jobDirectory = std::make_unique<filesystem::JobDirectory>(serverManagerConf->getRootPath(), clientId, jobName);
			const auto topology = std::make_shared<std::vector<std::string>>(jobDirectory->getResultTopology());
			data.setResultTopology(topology);
			data.setProperty(POINTER_PROPERTY, POINTER_INIT);
			data.setState(SEND_FILES_STATE);
			return utility::serialise(*topology);
		} else {
			return clientServerConf->getErrorMessage();
		}
  }

  std::string DetailedServer::sendFiles(entity::ClientRecord& data, const std::string& message) const {
		auto pathListPointer = std::stoi(data.getProperty(POINTER_PROPERTY));
		std::string filepath = data.getResultTopology()->at(pathListPointer);
		std::string content = utility::fileToString(filepath);
		++pathListPointer;
		if (pathListPointer == data.getResultTopology()->size()) {
			data.setResultTopology(nullptr);
			data.eraseProperty(POINTER_PROPERTY);
			const auto clientId = data.getProperty(CLIENTID_PROPERTY);
			const auto jobName = data.getProperty(JOB_NAME_PROPERTY);
			data.eraseProperty(JOB_NAME_PROPERTY);
			persistentData->removeJob(clientId, jobName);
			filesystem::JobDirectory jobDirectory(serverManagerConf->getRootPath(), clientId, jobName);
			jobDirectory.remove();
			data.setState(IDLE_STATE);
		}
		return content;
  }
}