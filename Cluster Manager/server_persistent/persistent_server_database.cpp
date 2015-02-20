#include "persistent_server_database.h"

#include <sstream>

#include "database_mysql_real_connection.h"
#include "database_mysql_store_result.h"
#include "utility_string.h"

namespace persistent {
	ServerDatabase::ServerDatabase(std::unique_ptr<const database::MySQLRealConnection>&& conn) : conn(std::move(conn))	{}
	ServerDatabase::~ServerDatabase() {}
	
	std::string ServerDatabase::getClientID(const std::string & username, const std::string & password) const {
		std::stringstream statement;
		statement << "SELECT `id` FROM `clients` WHERE `username` ='" << username << +"' AND `password` ='" << password + "'";
		return conn->selectUnit(statement.str().c_str(), [&]() {return "No such Client, user|pass: " + username + "|" + password; });
	}
	
	std::string ServerDatabase::getInfo(const std::string & clientId) const {
		std::stringstream statement;
		statement << "SELECT `name`, `neededWorkers` FROM `jobs` WHERE idClient =" << clientId << " ORDER BY `name` ASC";
		auto result = conn->select(statement.str().c_str());
		std::stringstream info;
		info << "<JobName> : <RemainingWorkers>" << std::endl;
		while (result->mysqlFetchRow()) {
			info << result->getCurrentAttribute();
			result->setAtNextAttribute();
			info << " : ";
			info << result->getCurrentAttribute();
			info << std::endl;
		}
		return info.str();
	}
	
	bool ServerDatabase::jobExists(const std::string & clientId, const std::string & jobName) const {
		std::stringstream statement;
		statement << "SELECT COUNT(`id`) FROM `jobs` WHERE `idClient` =" << clientId << " AND `name` = '" << jobName << "'";
		const auto result = conn->selectUnit(statement.str().c_str(), [&]() {return "No such job " + jobName + " exists." ; });
		return result.compare("0");
	}
	
	bool ServerDatabase::jobFinished(const std::string & clientId, const std::string & jobName) const {
		std::stringstream statement;
		statement << "SELECT COUNT(`id`) FROM `jobs` WHERE `idClient` =" << clientId << " AND `name` = '" << jobName << "' AND `neededWorkers` = 0";
		const auto result = conn->selectUnit(statement.str().c_str(), [&]() {return "Job" + jobName + " is not finished"; });
		return result.compare("0");
	}
	
	void ServerDatabase::storeJob(const std::string & clientId, const std::string & jobName, const std::string & jobType, const std::string & neededWorkers, const std::string & args, const std::string & executable) const {
		std::stringstream statement;
		statement << "INSERT INTO `jobs` (`idClient`, `name`, `jobType`, `neededWorkers`, `args`, `path`) VALUES ("
			<< clientId << R"(,")" << jobName << R"(",")" << jobType << R"(",)" << neededWorkers << R"(,")" << args << R"(",")" << executable << R"("))";
		const auto preparedStatement = utility::duplicateSlashesIn(statement.str());
		conn->querry(preparedStatement.c_str());
	}
	
	void ServerDatabase::removeJob(const std::string & clientId, const std::string & jobName) const {
		std::stringstream statement;
		statement << "DELETE FROM `jobs` WHERE `idClient` =" << clientId << " AND `name` ='" + jobName + "'";
		conn->querry(statement.str().c_str());
	}
}