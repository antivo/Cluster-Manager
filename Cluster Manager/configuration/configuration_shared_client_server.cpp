#include "configuration_shared_client_server.h"

#include "common_assert.h"
#include "configuration_messages.h"

namespace configuration {

	std::shared_ptr<configuration::SharedClientServer> SharedClientServer::makeDefault() {
		auto default = std::make_shared<configuration::SharedClientServer>(
			configuration::successMessage,
			configuration::errorMessage);
		return default;
	}

	SharedClientServer::SharedClientServer(std::string successMessage, std::string errorMessage) :
		successMessage(successMessage), errorMessage(errorMessage)
	{
		assert::stringNotEmpty(successMessage, "SharedClientServer success message must not be empty");
		assert::stringNotEmpty(errorMessage, "SharedClientServer error message must not be empty");
	}
	
	SharedClientServer::~SharedClientServer() {}
	
	std::string SharedClientServer::getSucessMessage() const {
		return successMessage;
	}
	
	std::string SharedClientServer::getErrorMessage() const {
		return errorMessage;
	}

	void SharedClientServer::setSucessMessage(const std::string & sucessMessage) {
		assert::stringNotEmpty(successMessage, "SharedClientServer success message must not be empty");
		this->successMessage = successMessage;
	}
	
	void SharedClientServer::setErrorMessage(const std::string & errorMessage) {
		assert::stringNotEmpty(errorMessage, "SharedClientServer error message must not be empty");
		this->errorMessage = errorMessage;
	}

}