#include "configuration_shared_client_server.h"

#include "assert_common.h"
#include "configuration_default.h"

namespace configuration {
	std::shared_ptr<configuration::SharedClientServer> SharedClientServer::makeDefault() {
		auto default = std::make_shared<configuration::SharedClientServer>(
			configuration::SERVER_PORT,
			configuration::SUCCESS_MESSAGE,
			configuration::ERROR_MESSAGE,
			configuration::CLIENT_CHOICE_SCHEDULE,
			configuration::CLIENT_CHOICE_OBTAIN,
			configuration::CLIENT_CHOICE_INFO,
			configuration::CLIENT_CHOICE_LOGOUT);
		return default;
	}

	SharedClientServer::SharedClientServer(const unsigned short port,
		const std::string& successMessage,
		const std::string& errorMessage,
		const std::string& clientChoiceSchedule,
		const std::string& clientChoiceObtain,
		const std::string& clientChoiceInfo,
		const std::string& clientChoiceLogout) :
			port(port),
			successMessage(successMessage), 
			errorMessage(errorMessage),
			clientChoiceSchedule(clientChoiceSchedule),
			clientChoiceObtain(clientChoiceObtain),
			clientChoiceInfo(clientChoiceInfo),
			clientChoiceLogout(clientChoiceLogout)
	{
		assert::stringNotEmpty(successMessage, "SharedClientServer success message must not be empty");
		assert::stringNotEmpty(errorMessage, "SharedClientServer error message must not be empty");
		assert::stringNotEmpty(clientChoiceSchedule, "SharedClientServer client choice for sending must not be empty");
		assert::stringNotEmpty(clientChoiceObtain, "SharedClientServer client choice for receiving must not be empty");
		assert::stringNotEmpty(clientChoiceInfo, "SharedClientServer client choice for info must not be empty");
		assert::stringNotEmpty(clientChoiceLogout, "SharedClientServer client choice for logout must not be empty");
	}
	
	SharedClientServer::~SharedClientServer() {}
	
	unsigned int SharedClientServer::getPort() const {
		return port;
	}

	std::string SharedClientServer::getSucessMessage() const {
		return successMessage;
	}
	
	std::string SharedClientServer::getErrorMessage() const {
		return errorMessage;
	}

	std::string SharedClientServer::getClientChoiceSchedule() const {
		return clientChoiceSchedule;
	}

	std::string SharedClientServer::getClientChoiceObtain() const {
		return clientChoiceObtain;
	}

	std::string SharedClientServer::getClientChoiceInfo() const {
		return clientChoiceInfo;
	}

	std::string SharedClientServer::getClientChoiceLogout() const {
		return clientChoiceLogout;
	}

	void SharedClientServer::setPort(unsigned int port) {
		this->port = port;
	}

	void SharedClientServer::setSucessMessage(const std::string & sucessMessage) {
		assert::stringNotEmpty(successMessage, "SharedClientServer success message must not be empty");
		this->successMessage = successMessage;
	}
	
	void SharedClientServer::setErrorMessage(const std::string & errorMessage) {
		assert::stringNotEmpty(errorMessage, "SharedClientServer error message must not be empty");
		this->errorMessage = errorMessage;
	}

	void SharedClientServer::setClientChoiceSchedule(const std::string& clientChoiceSchedule) {
		assert::stringNotEmpty(clientChoiceSchedule, "SharedClientServer client choice for sending must not be empty");
		this->clientChoiceSchedule = clientChoiceSchedule;
	}

	void SharedClientServer::setClientChoiceObtain(const std::string& clientChoiceObtain) {
		assert::stringNotEmpty(clientChoiceObtain, "SharedClientServer client choice for receiving must not be empty");
		this->clientChoiceObtain = clientChoiceObtain;
	}
	
	void SharedClientServer::setClientChoiceInfo(const std::string& clientChoiceInfo) {
		assert::stringNotEmpty(clientChoiceInfo, "SharedClientServer client choice for info must not be empty");
		this->clientChoiceInfo = clientChoiceInfo;
	}

	void SharedClientServer::setClientChoiceLogout(const std::string& clientChoiceLogout) {
		assert::stringNotEmpty(clientChoiceLogout, "SharedClientServer client choice for logout must not be empty");
		this->clientChoiceLogout = clientChoiceLogout;
	}
}