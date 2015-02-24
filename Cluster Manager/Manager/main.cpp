#include <iostream>

#include "assert_common.h"
#include "configuration_manager.h"
#include "configuration_shared_server_manager.h"
#include "database_mysql_real_connection.h"
#include "entity_worker.h"
#include "manager_manager.h"
#include "persistent_manager_database.h"
#include "utility_string.h"

int main() {
	try {
		const auto sharedData = configuration::SharedServerManager::makeDefault();
		auto conn = std::make_unique<const database::MySQLRealConnection>(
			sharedData->getDatabaseHostname().c_str(),
			sharedData->getDatabaseUsername().c_str(),
			sharedData->getDatabasePassword().c_str(),
			sharedData->getDatabaseName().c_str(),
			sharedData->getDatabsePort());	
		auto persistent = std::make_shared<persistent::ManagerDatabase>(std::move(conn));
		persistent->resetManagerSpecificData();
		auto manConf = configuration::Manager::makeDefault();
		auto shared = configuration::SharedServerManager::makeDefault();
		manager::Manager manager(persistent, manConf, shared);
	
		std::cout << "Path to workers" << std::endl;
		std::string consoleInput;
		std::cin >> consoleInput;
		const auto workerDefLines = utility::fileToStringVector(consoleInput);
		std::cout << workerDefLines.size() << std::endl;
		std::cout << "I have read: " << std::endl;
		for (const auto workerDef : workerDefLines) {
			std::cout << workerDef << std::endl;
			auto workerDefparts = utility::stringExplode(workerDef, '|');
			assert::runtime(2 == workerDefparts.size(), "Definition of worker demands alias and working dir. Irregular line " + workerDef);
			auto& alias = workerDefparts[0];
			auto& workingDirectory = workerDefparts[1];
			utility::trim(alias);
			utility::trim(workingDirectory);
			auto worker = std::make_unique<entity::Worker>(alias, workingDirectory);
			manager.addWorker(std::move(worker));
		}
		std::cout << "There are " << manager.getSizeOfWorkers() << " workers" << std::endl;
		manager.run();
		std::cout << "is running" << std::endl;
		// commands for manager not defined, run in infinite loop in this version
		for (;;) {}
	} catch (const std::exception& e) {
		std::cout << std::endl << "E:" << e.what();
	}
	return 0;
}