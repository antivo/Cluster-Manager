#include "configuration_client.h"

#include "configuration_default.h"

namespace configuration {
	std::unique_ptr<configuration::Client> Client::makeDefault() {
		return std::make_unique<configuration::Client>(configuration::CONSOLE_COLOR, configuration::SERVER_ADDRESS);
	}

	Client::Client(const int consoleColor, const std::string & serverAddress) : 
		consoleColor(consoleColor), serverAddress(serverAddress) {}

	Client::~Client() {}

	int Client::getConsoleColor() const {
		return consoleColor;
	}

	std::string Client::getServerAddress() const {
		return serverAddress;
	}

	void Client::setConsoleColor(const int consoleColor) {
		this->consoleColor = consoleColor;
	}

	void Client::setServerAddress(const std::string & serverAddress) {
		this->serverAddress = serverAddress;
	}
}