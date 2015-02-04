#include "persistent_server_database.h"

#include "database_mysql_real_connection.h"
#include "database_mysql_store_result.h"

namespace persistent {
	ServerDatabase::ServerDatabase(std::unique_ptr<const database::MySQLRealConnection>&& conn) : conn(std::move(conn))	{}
	ServerDatabase::~ServerDatabase() {}
	
	std::string ServerDatabase::getClientID(const std::string & username, const std::string & password) const {
		return std::string();
	}
	
	std::string ServerDatabase::getInfo(const std::string & clientId) const {
		return std::string();
	}
	
	bool ServerDatabase::jobExists(const std::string & id, const std::string & job) const {
		return false;
	}
	
	bool ServerDatabase::jobFinished(const std::string & id, const std::string & job) const {
		return false;
	}
	
	void ServerDatabase::storeJob(const std::string & id, const std::string & jobName, const std::string & jobType, const std::string & neededWorkers, const std::string & args, const std::string & executable) const {}
	
	void ServerDatabase::removeJob(const std::string & id, const std::string & job) const {}
}

/*

#include "database_controler_server_side.h"

#include <stdexcept>
#include <sstream>

#include "database_mysql_real_connection.h"
#include "database_mysql_store_result.h"



#include "utility_string.h"



namespace database {
ControllerServer::ControllerServer(const char* hostname,
const char* userName,
const char* password,
const char* databaseName,
const unsigned long& portNumber) :
conn(std::make_unique<const database::MySQLRealConnection>(hostname, userName, password, databaseName, portNumber)) {}

ControllerServer::~ControllerServer() {}

std::string ControllerServer::getClientID(const std::string& username, const std::string& pass) const {
std::stringstream statement;
statement << "SELECT `id` FROM `clients` WHERE `username` ='" << username <<+ "' AND `password` ='" << pass + "'";
return conn->select(statement.str().c_str(), [&username, &pass]() {return "No such Client, user|pass: " + username + "|" + pass; });
}

bool ControllerServer::jobExists(const std::string& idClient, const std::string& jobName) {
std::stringstream statement;
statement << "SELECT COUNT(`id`) FROM `jobs` WHERE `idClient` =" << idClient << " AND `name` = '" << jobName << "'";
const auto result = conn->select(statement.str().c_str(), [&idClient, &jobName]() {return "No such job, idClient|jobName"; });
return result.compare("0");
}

void ControllerServer::insertJob(const std::string& idClient, const std::string& name, const std::string& parallel, const std::string& neededWorkers, const std::string& path) {

}


//std::string ControlerServerSide::getFullInformation(const std::string& idClient) const {

//}

//std::string ControlerServerSide::getBasicInformation(const std::string& idClient) const {

//}



// TODO

}



*/