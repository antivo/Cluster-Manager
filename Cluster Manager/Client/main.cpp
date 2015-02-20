#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "assert_common.h"
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

constexpr auto HELLO_MESSAGE = "Welcome to Cluster Manager Client, your gateway to Cluster Manager Service.\n(please visit https://github.com/antivo/Cluster-Manager)";
constexpr auto INPUT_CREDIENTIALS_MESSAGE = "Network - OK\nYou have connected to Cluster Manager Service. Please authenticate with username and password";
constexpr auto ACTIONS_DISPLAY = "You have access to Cluster Manager Service, these are your options:\n1) Choose to send a job and then provide path to ContractFile\n2) Choose to receive job results and then provide a job's name\n3) Choose to retrieve info\n4) Quit";
constexpr auto PRESS_ANY_KEY = "Press any key...";

constexpr auto CONTRACT_FILE_INPUT = "Provide a path to your Contract - File";
constexpr auto LOCAL_FOLDER_INPUT = "Provide a path to folder where you will store data";
constexpr auto JOB_NAME_INPUT = "Provide a name of desired job";

constexpr auto FINISHED_EVENT = "Finished.";
constexpr auto DISCONNECTED_EVENT = "You have been disconnected from the server.";

int main() {
	try {
		// START
		std::cout << HELLO_MESSAGE << std::endl;
		const auto clientConf = configuration::Client::makeDefault();
		const auto sharedData = configuration::SharedClientServer::makeDefault();
		const auto port = std::to_string(sharedData->getPort());
    auto connection = std::make_unique<network::Connection>(clientConf->getServerAddress().c_str(), port.c_str());
		// AUTHENTICATE
		std::cout << INPUT_CREDIENTIALS_MESSAGE << std::endl;
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
				std::cout << CONTRACT_FILE_INPUT << std::endl;
				std::cin >> consoleInput;
				auto fileStringVector = utility::fileToStringVector(consoleInput);
				auto rawContract = std::make_unique<entity::RawContract>(std::move(fileStringVector));
				const auto contract = rawContract->getContract();
				const auto pathList = rawContract->getLocalPathList();
				connection->send(utility::serialise(contract->getContractVector()));
				for (const auto& ss : *pathList) {
					std::cout << connection->receive() << std::endl;
					const auto content = utility::fileToString(ss);
					assert::runtime(content.size() > 0, "Empty files can not be sent to server");
					connection->send(content);
				}
				std::cout << connection->receive() << std::endl;
			} else if (!consoleInput.compare(sharedData->getClientChoiceObtain())) { // OBTAIN JOB
				std::cout << LOCAL_FOLDER_INPUT << std::endl;
				std::cin >> consoleInput;
				filesystem::Directory dest(consoleInput);
				std::cout << JOB_NAME_INPUT << std::endl;
				std::cin >> consoleInput;
				connection->send(consoleInput);
				const auto files = utility::deserialize(connection->receive());
				for (const auto& ss : files) {
					connection->send(sharedData->getSucessMessage());
					const auto content = connection->receive();
					dest.store(ss, content);
				}
				std::cout << FINISHED_EVENT << std::endl;
			} else if (!consoleInput.compare(sharedData->getClientChoiceLogout())) { // QUIT
				break;
			}
		}
  } catch(const std::exception& e) {
     std::cout << e.what() << std::endl;
		 std::cout << DISCONNECTED_EVENT << std::endl;
		 std::cout << PRESS_ANY_KEY << std::endl;
		 std::cin.get();
		 std::cin.get();
	}
  return 0;
}