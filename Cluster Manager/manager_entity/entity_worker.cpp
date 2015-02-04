#include "entity_worker.h"

#include "filesystem_directory.h"

namespace entity {
	Worker::Worker(const std::string & alias, const std::string & rootDirectory) :
		alias(alias), rootDirectory(std::make_shared<filesystem::Directory>(rootDirectory))
	{}

	std::string Worker::getAlias() const {
		return alias;
	}

	std::shared_ptr<filesystem::Directory> Worker::getRootDirectory() const {
		return rootDirectory;
	}

	std::shared_ptr<filesystem::Directory> Worker::getWorkingDirectory() const {
		return workingDirectory;
	}

	void Worker::setWorkingDirectory(std::shared_ptr<filesystem::Directory> workingDir) {
		this->workingDirectory = workingDir;
	}

}
