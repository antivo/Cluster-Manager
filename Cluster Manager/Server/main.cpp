#include <algorithm>
#include <iostream>
#include <memory>

#include "configuration_shared_client_server.h"
#include "configuration_shared_server_manager.h"
#include "database_mysql_real_connection.h"
#include "persistent_server_database.h"
#include "server_detailed_server.h"

int main() {
	try {
		const auto sharedWithClient = configuration::SharedClientServer::makeDefault();
		const auto sharedWithManager = configuration::SharedServerManager::makeDefault();
		auto conn = std::make_unique<const database::MySQLRealConnection>(
			sharedWithManager->getDatabaseHostname().c_str(), 
			sharedWithManager->getDatabaseUsername().c_str(),
			sharedWithManager->getDatabasePassword().c_str(),
			sharedWithManager->getDatabaseName().c_str(),
			sharedWithManager->getDatabsePort());
		const auto serverDatabase = std::make_shared<persistent::ServerDatabase>(std::move(conn));
		const auto server = std::make_unique<server::DetailedServer>(sharedWithClient, sharedWithManager);

		std::cout << "What are your orders. (start/stop/quit)" << std::endl;
		std::string consoleInput;
		for (;;) {
			std::cin >> consoleInput;
			std::transform(consoleInput.begin(), consoleInput.end(), consoleInput.begin(), ::toupper);
			if (!consoleInput.compare("START")) {
				server->run(serverDatabase);
				std::cout << "Server is running" << std::endl;
			} else if (!consoleInput.compare("STOP")) {
				server->stop();
				std::cout << "Server stopped" << std::endl;
			} else if (!consoleInput.compare("QUIT")) {
				break;
			}
		}
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}