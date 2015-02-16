#include "configuration_manager.h"

#include "assert_common.h"
#include "configuration_default.h"

namespace configuration {
	std::shared_ptr<configuration::Manager> Manager::makeDefault() {
		return std::make_shared<configuration::Manager>(
			configuration::MPIEXEC_PATH,
			configuration::DETACHED_EXECUTOR_PATH,
			configuration::ATTACHED_INSPECTOR_PATH);
	}

	Manager::Manager(const std::string& mpiexecPath,
		const std::string& detachedExecutorPath, 
		const std::string& attachedInspectorPath) :
			mpiexecPath(mpiexecPath),
			detachedExecutorPath(detachedExecutorPath),
			attachedInspectorPath(attachedInspectorPath)
	{
		assert::stringNotEmpty(mpiexecPath, "SharedClientServer mpiexec path must not be empty");
		assert::stringNotEmpty(detachedExecutorPath, "SharedClientServer detached executor path must not be empty");
		assert::stringNotEmpty(attachedInspectorPath, "SharedClientServer attached inspector path must not be empty");
	}

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

	void Manager::setMpiexecPath(const std::string & mpiexecPath) {
		assert::stringNotEmpty(mpiexecPath, "SharedClientServer mpiexec path must not be empty");
		this->mpiexecPath = mpiexecPath;
	}

	void Manager::setDetachedExecutorPath(const std::string & detachedExecutorPath) {
		assert::stringNotEmpty(detachedExecutorPath, "SharedClientServer detached executor path must not be empty");
		this->detachedExecutorPath = detachedExecutorPath;
	}

	void Manager::setAttachedInspector(const std::string & attachedInspectorPath) {
		assert::stringNotEmpty(attachedInspectorPath, "SharedClientServer attached inspector path must not be empty");
		this->attachedInspectorPath = attachedInspectorPath;
	}
}
