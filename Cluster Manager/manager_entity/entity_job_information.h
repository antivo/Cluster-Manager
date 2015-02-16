#ifndef MANAGER_ENTITY_JOB_INFORMATION
#define MANAGER_ENTITY_JOB_INFORMATION

#include <vector>
#include <string>

#include "entity_job_type.h"

namespace entity {
	class JobInformation {
	public:
		explicit JobInformation(const std::vector<std::string>& jobInformationVector);
		explicit JobInformation(std::vector<std::string>&& jobInformationVector);
		~JobInformation();

		std::string getClientID() const;
		std::string getJobName() const;
		std::string getJobID() const;
		entity::JobType getJobType() const;
		int getNeededWorkers() const;
		std::string getArgs() const;
		std::string getExecutablePath() const;

		void substractNeededWorkers(const int workers);
		int sizeOfNextWorkerGroup()const;

	private:
		std::vector<std::string> jobInformationVector;

		enum JobInformationVectorField {
			ClientID = 0, JobName = 1, JobID = 2, JobType = 3, NeededWorkers = 4, Args = 5, ExecutablePath = 6, SIZE = 7
		};
	};

}


#endif