#include "configuration_shared_server_manager.h"

#include "common_assert.h"
#include "configuration_filesystem.h"

namespace configuration {
	SharedServerManager::SharedServerManager(std::string rootPath, std::string dataDirectoryName, std::string resultDirectoryName) :
		rootPath(rootPath), dataDirectoryName(dataDirectoryName), resultDirectoryName(resultDirectoryName)
	{
		assert::stringNotEmpty(rootPath, "ServerShared::rootPath can not be empty");
		assert::stringNotEmpty(dataDirectoryName, "ServerShared::dataDirectoryName can not be empty");
		assert::stringNotEmpty(resultDirectoryName, "ServerShared::recvDirectoryName can not be empty");
	}

	SharedServerManager::~SharedServerManager() {}

	std::shared_ptr<configuration::SharedServerManager> SharedServerManager::makeDefault() {
		auto default = std::make_shared<configuration::SharedServerManager>(
			configuration::filesystemRootDirectory, 
			configuration::filesystemDataDirectory,
			configuration::filesystemResultDirectory);
		return default;
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