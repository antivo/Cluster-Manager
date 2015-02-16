#include <iostream>


#include "configuration_manager.h"
#include "configuration_shared_server_manager.h"

#include "entity_worker.h"
#include "database_mysql_real_connection.h"
#include "manager.h"
#include "persistent_manager_database.h"

int main() {
	try {
	auto w1 = std::make_unique<entity::Worker>("C12", R"(\\C12\mpi)");
	auto w2 = std::make_unique<entity::Worker>("C14", R"(\\C14\mpi)");

	auto conn = std::make_unique<const database::MySQLRealConnection>("localhost", "root", "cmadmin", "clusterDB", 4000);
  auto persistent = std::make_shared<persistent::ManagerDatabase>(std::move(conn));

	auto manConf = configuration::Manager::makeDefault();
	auto shared = configuration::SharedServerManager::makeDefault();
	
	manager::Manager manager(persistent, manConf, shared);
	manager.addWorker(std::move(w1));
	manager.addWorker(std::move(w2));

	manager.run();


	std::string a;
	std::cin >> a;
	} catch (const std::exception& e) {
		std::cout << std::endl << "E:" << e.what();
	}
	return 0;
}