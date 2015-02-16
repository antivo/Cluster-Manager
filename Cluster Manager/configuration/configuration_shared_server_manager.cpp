#include "configuration_shared_server_manager.h"

#include "assert_common.h"
#include "configuration_default.h"

namespace configuration {
	SharedServerManager::SharedServerManager(const int databasePort,
		const std::string& databaseHostname,
		const std::string& databaseUsername,
		const std::string& databasePassword,
		const std::string& databaseName,
		const std::string& rootPath,
		const std::string& dataDirectoryName,
		const std::string& resultDirectoryName) :
			databasePort(databasePort),
			databaseHostname(databaseHostname),
			databaseUsername(databaseUsername),
			databasePassword(databasePassword),
			databaseName(databaseName),
			rootPath(rootPath), 
			dataDirectoryName(dataDirectoryName), 
			resultDirectoryName(resultDirectoryName)
	{
		assert::stringNotEmpty(rootPath, "ServerShared::rootPath can not be empty");
		assert::stringNotEmpty(dataDirectoryName, "ServerShared::dataDirectoryName can not be empty");
		assert::stringNotEmpty(resultDirectoryName, "ServerShared::recvDirectoryName can not be empty");
	}

	SharedServerManager::~SharedServerManager() {}

	std::shared_ptr<configuration::SharedServerManager> SharedServerManager::makeDefault() {
		auto default = std::make_shared<configuration::SharedServerManager>(
			configuration::DATABASE_PORT_NUMBER,
			configuration::DATABASE_HOST_NAME,
			configuration::DATABASE_USER_NAME,
			configuration::DATABASE_PASSWORD,
			configuration::DATABASE_NAME,
			configuration::FILESYSTEM_ROOT_DIRECTORY, 
			configuration::FILESYSTEM_DATA_DIRECTORY,
			configuration::FILESYSTEM_RESULT_DIRECTORY);
		return default;
	}

	int SharedServerManager::getDatabsePort() const {
		return databasePort;
	}

	std::string SharedServerManager::getDatabaseHostname() const {
		return databaseHostname;
	}

	std::string SharedServerManager::getDatabaseUsername() const {
		return databaseUsername;
	}

	std::string SharedServerManager::getDatabasePassword() const {
		return databasePassword;
	}

	std::string SharedServerManager::getDatabaseName() const {
		return databaseName;
	}

	std::string SharedServerManager::getRootPath() const {
		return rootPath;
	}

	std::string SharedServerManager::getDataDirectoryName() const {
		return dataDirectoryName;
	}

	std::string SharedServerManager::getResultDirectoryName() const {
		return resultDirectoryName;
	}
	
	void SharedServerManager::setDatabsePort(const int databasePort) {
		this->databasePort = databasePort;
	}

	void SharedServerManager::setDatabaseHostname(const std::string& databaseHostname) {
		this->databaseHostname = databaseHostname;
	}

	void SharedServerManager::getDatabaseUsername(const std::string& databaseUsername) {
		this->databaseUsername = databaseUsername;
	}

	void SharedServerManager::getDatabasePassword(const std::string& databasePassword) {
		this->databasePassword = databasePassword;
	}

	void SharedServerManager::getDatabaseName(const std::string& databaseName) {
		this->databaseName = databaseName;
	}

	void SharedServerManager::setRootPath(const std::string & rootPath) {
		assert::stringNotEmpty(rootPath, "ServerShared::rootPath can not be empty");
		this->rootPath = rootPath;
	}

	void SharedServerManager::setDataDirectoryName(const std::string & dataDirectoryName) {
		assert::stringNotEmpty(dataDirectoryName, "ServerShared::dataDirectoryName can not be empty");
		this->dataDirectoryName = dataDirectoryName;
	}

	void SharedServerManager::setResultDirectoryName(const std::string & resultDirectoryName) {
		assert::stringNotEmpty(resultDirectoryName, "ServerShared::recvDirectoryName can not be empty");
		this->resultDirectoryName = resultDirectoryName;
	}
}