#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "configuration_client.h"
#include "configuration_shared_client_server.h"
#include "entity_contract.h"
#include "entity_raw_contract.h"
#include "exception_network.h"
#include "filesystem_directory.h"
#include "network_connection.h"
#include "console_color_manipulator.h"
#include "console_input_disabler.h"
#include "utility_serialization.h"
#include "utility_string.h"

constexpr auto HELLO_MESSAGE = "\nWelcome to Cluster Manager Client, your gateway to Cluster Manager Service.\n(please visit https://github.com/antivo/Cluster-Manager)\n";
constexpr auto INPUT_CREDIENTIALS_MESSAGE = "\nNetwork - OK\nYou have connected to Cluster Manager. Please authenticate with username and password\n";
constexpr auto ACTIONS_DISPLAY = "\nYou have access to Cluster Manager Service, these are your options:\n1) Choose to send a job and then provide path to ContractFile\n2) Choose to receive job results and then provide a job's name\n3) Choose to retrieve info\n4) Quit\n";

int main() {
	try {
		// START
		std::cout << HELLO_MESSAGE;
		const auto clientConf = configuration::Client::makeDefault();
		const auto sharedData = configuration::SharedClientServer::makeDefault();
		const auto port = std::to_string(sharedData->getPort());
    auto connection = std::make_unique<network::Connection>(clientConf->getServerAddress().c_str(), port.c_str());
		// AUTHENTICATE
		std::cout << INPUT_CREDIENTIALS_MESSAGE;
		console::ColorManipulator colorManipulator(clientConf->getConsoleColor()); 
		std::string consoleInput;
		// send username
		std::cin >> consoleInput;
		connection->send(consoleInput); 
		std::cout << connection->receive() << std::endl;
		// send password
		{
			console::InputDisabler inputDisabler;
			std::cin >> consoleInput;
		}
		connection->send(consoleInput);
		std::cout << connection->receive() << std::endl;
		// ACTION
		for (;;) {
			std::cout << std::endl << ACTIONS_DISPLAY << std::endl;
			std::cin >> consoleInput;
			connection->send(consoleInput);
			std::cout << connection->receive() << std::endl;
			if (!consoleInput.compare(sharedData->getClientChoiceSchedule())) { // SCHEDULE JOB
				std::cout << "Provide a path to your Contract - File" << std::endl;
				std::cin >> consoleInput;
				auto fileStringVector = utility::fileToStringVector(consoleInput);
				auto rawContract = std::make_unique<entity::RawContract>(std::move(fileStringVector));
				const auto contract = rawContract->getContract();
				const auto pathList = rawContract->getLocalPathList();
				connection->send(utility::serialise(contract->getContractVector()));
				for (const auto& ss : *pathList) {
					connection->receive();
					connection->send(utility::fileToString(ss));
				}
				std::cout << connection->receive() << std::endl;
			} else if (!consoleInput.compare(sharedData->getClientChoiceObtain())) { // OBTAIN JOB
				std::cout << "Provide a path to folder where you will store data" << std::endl;
				std::cin >> consoleInput;
				filesystem::Directory dest(consoleInput);
				std::cout << "Provide a name of desired job" << std::endl;
				std::cin >> consoleInput;
				connection->send(consoleInput);
				const auto files = utility::deserialize(connection->receive());
				for (const auto& ss : files) {
					connection->send(sharedData->getSucessMessage());
					const auto sadrzaj = connection->receive();
					dest.store(ss, sadrzaj);
				}
				std::cout << "FINISHED" << std::endl;
			} else if (!consoleInput.compare(sharedData->getClientChoiceLogout())) { // QUIT
				break;
			}
		}
  } catch(const std::exception& e) {
     std::cout << e.what() << std::endl;
		 std::cout << "You have been disconnected from the server." << std::endl;
	} 
  return 0;
}