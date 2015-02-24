#include "configuration_manager.h"

#include "assert_common.h"
#include "configuration_default.h"

namespace configuration {
	std::shared_ptr<configuration::Manager> Manager::makeDefault() {
		return std::make_shared<configuration::Manager>(
			configuration::MPIEXEC_PATH);
	}

	Manager::Manager(const std::string& mpiexecPath) :
			mpiexecPath(mpiexecPath)
	{
		assert::stringNotEmpty(mpiexecPath, "SharedClientServer mpiexec path must not be empty");
	}

	Manager::~Manager() {}

	std::string Manager::getMpiexecPath() const {
		return mpiexecPath;
	}

	void Manager::setMpiexecPath(const std::string & mpiexecPath) {
		assert::stringNotEmpty(mpiexecPath, "SharedClientServer mpiexec path must not be empty");
		this->mpiexecPath = mpiexecPath;
	}
}
