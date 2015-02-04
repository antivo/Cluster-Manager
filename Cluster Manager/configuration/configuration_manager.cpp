#include "configuration_manager.h"

#include "configuration_master_node.h"

namespace configuration {
	std::shared_ptr<configuration::Manager> Manager::makeDefault() {
		return std::make_shared<configuration::Manager>(
			configuration::mpiexecPath,
			configuration::detachedExecutorPath,
			configuration::attachedInspectorPath,
			configuration::resultFilePath);
	}

	Manager::Manager(const std::string& mpiexecPath,
		const std::string& detachedExecutorPath, 
		const std::string& attachedInspectorPath,
		const std::string& resultFilePath) :
			mpiexecPath(mpiexecPath),
			detachedExecutorPath(detachedExecutorPath),
			attachedInspectorPath(attachedInspectorPath),
			resultFilePath(resultFilePath)
	{}

	Manager::~Manager() {}

	std::string Manager::getMpiexecPath() const {
		return mpiexecPath;
	}

	std::string Manager::getDetachedExecutorPath() const {
		return detachedExecutorPath;
	}

	std::string Manager::getAttachedInspector() const {
		return attachedInspectorPath;
	}

	std::string Manager::getResultFilePath() const {
		return resultFilePath;
	}

	void Manager::setMpiexecPath(const std::string & mpiexecPath) {
		this->mpiexecPath = mpiexecPath;
	}

	void Manager::setDetachedExecutorPath(const std::string & detachedExecutorPath) {
		this->detachedExecutorPath = detachedExecutorPath;
	}

	void Manager::setAttachedInspector(const std::string & attachedInspectorPath) {
		this->attachedInspectorPath = attachedInspectorPath;
	}
	void Manager::setResultFilePath(const std::string & resultFilePath) {
		this->resultFilePath = resultFilePath;
	}
}
