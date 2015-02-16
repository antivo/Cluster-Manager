#include "entity_contract.h"

#include <algorithm>
#include <string>
#include <vector>

#include "assert_common.h"
#include "filesystem_common.h"
#include "entity_job_type.h"
#include "utility_string.h"

namespace entity {	
	Contract::Contract(const std::vector<std::string>& contractVector) : contractVector(contractVector) {
		assertContract();
	}

	Contract::Contract(std::vector<std::string>&& contractVector) : contractVector(std::move(contractVector)) {
		assertContract();
	}

	std::string Contract::getJobName() const {
		return contractVector[ContractVectorField::JobName];
	}

	std::string Contract::getJobType() const {
		return contractVector[ContractVectorField::JobType];
	}

	std::string Contract::getNeededWorkers() const {
		return contractVector[ContractVectorField::NeededWorkers];
	}

	std::string Contract::getArgs() const {
		return contractVector[ContractVectorField::Args];
	}

	std::string Contract::getExecutable() const {
		return contractVector[ContractVectorField::Executable];
	}

	std::vector<std::string> Contract::getPathList() const {
		return std::vector<std::string>{ contractVector.begin() + ContractVectorField::PathListStart, contractVector.end() };
	}

	std::vector<std::string> Contract::getContractVector() const {
		return contractVector;
	}

	bool Contract::isExecutable(const std::string& filename) {
		const auto isFilepath = filesystem::isFilePath(filename);
		return isFilepath && filesystem::endsWithExe(filename);
	}

	void Contract::assertContract() { 
		assert::runtime(contractVector.size() > ContractVectorField::PathListStart, 
			"Contract Vector must contain at least " + std::to_string(ContractVectorField::PathListStart) + " elements. it has only " + std::to_string(contractVector.size()));
		
		const auto& jobName = contractVector[ContractVectorField::JobName]; 
		assert::runtime(filesystem::isDirectory(jobName), 
			"Invalid Contract. Job name is not a valid job name. Job name provided " + jobName);

		const auto& jobType = contractVector[ContractVectorField::JobType];
		entity::createJobType(jobType);

		const auto& neededWorkers = contractVector[ContractVectorField::NeededWorkers];
		assert::runtime(utility::isNonZeroNonNegativeInt(neededWorkers),
			"Invalid Contract. Needed workers must be positive. Assigned number for needed workers was " + neededWorkers);
		
		const auto& executable = contractVector[ContractVectorField::Executable];
		assert::runtime(isExecutable(executable),
			"Invalid Contract. Job exectuion point extension is not supported. Assigned execution point was " + executable);

		const auto found = 
			std::find_if_not(std::next(contractVector.begin(), ContractVectorField::PathListStart), contractVector.end(),
			[](const std::string& path) { return filesystem::isFilePath(path); });
		assert::condition(contractVector.end() == found, [&]() {return std::runtime_error("Invalid Contract. Path list must containt valid paths. Invalid assigned path is " + *found); });
	}

}