#include "entity_job_information.h"

#include "assert_common.h"

namespace entity {
	JobInformation::JobInformation(const std::vector<std::string>& jobInformationVector) : jobInformationVector(jobInformationVector) {
		assert::runtime(jobInformationVector.size() == JobInformationVectorField::SIZE, "Invalid size for job information vector");
	}
	JobInformation::JobInformation(std::vector<std::string>&& jobInformationVector) : jobInformationVector(jobInformationVector) {
		assert::runtime(jobInformationVector.size() == JobInformationVectorField::SIZE, "Invalid size for job information vector");
	}

	JobInformation::~JobInformation() {}

	std::string JobInformation::getClientID() const {
		return jobInformationVector[JobInformationVectorField::ClientID];
	}

	std::string JobInformation::getJobName() const {
		return jobInformationVector[JobInformationVectorField::JobName];
	}
	
	std::string JobInformation::getJobID() const {
		return jobInformationVector[JobInformationVectorField::JobID];
	}

	entity::JobType JobInformation::getJobType() const {
		return entity::createJobType(jobInformationVector[JobInformationVectorField::JobType]);
	}

	int JobInformation::getNeededWorkers() const {
		return std::stoi(jobInformationVector[JobInformationVectorField::NeededWorkers]);
	}

	std::string JobInformation::getArgs() const {
		return jobInformationVector[JobInformationVectorField::Args];
	}

	std::string JobInformation::getExecutablePath() const {
		return jobInformationVector[JobInformationVectorField::ExecutablePath];
	}

	void JobInformation::substractNeededWorkers(const int workers) {
		auto neededWorkers = std::stoi(jobInformationVector[JobInformationVectorField::NeededWorkers]);
		neededWorkers -= workers;
		assert::runtime(workers >= 0, "Workers for job must always be larger than zero");
		this->jobInformationVector[JobInformationVectorField::NeededWorkers] = std::to_string(neededWorkers);
	}
	
	int JobInformation::sizeOfNextWorkerGroup() const {
		const auto jobType = getJobType();
		switch (jobType) {
		case JobType::SERIAL: return getNeededWorkers() > 0 ? 1 : 0;
		case JobType::PARALLEL: return getNeededWorkers();
		default: assert::runtime(false, "JobInformation::sizeOfNextWorkerGroup unsupported job type");
		}
	}
}