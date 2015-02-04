#include "thread_execute.h"

#include <memory>
#include <queue>
#include <vector>
#include <sstream>

#include "configuration_manager.h"
#include "configuration_shared_server_manager.h"
#include "entity_job_type.h"
#include "entity_job_information.h"
#include "entity_executed_job.h"
#include "filesystem_directory.h"
#include "filesystem_job_directory.h"
#include "persistent_manager_data.h"
#include "utility_periodic_sleeper.h"

namespace thread {
	Execute::Execute(std::unique_ptr<utility::PeriodicSleeper>&& periodicSleeper, 
		const std::shared_ptr<locked::WorkerQueue>& workers, 
		const std::shared_ptr<persistent::ManagerData>& persistentData, 
		const std::shared_ptr<configuration::Manager>& managerConfiguration,
		const std::shared_ptr<configuration::SharedServerManager>& sharedData,
		const std::function<void(std::unique_ptr<entity::ExecutedJob>)>& forwardExecutedJob) :
			periodicSleeper{ std::move(periodicSleeper) },
			workers{ workers },
			persistentData{ persistentData },
			managerConfiguration{ managerConfiguration },
			sharedData{ sharedData },
			forwardExecutedJob{ forwardExecutedJob },
			task{ [this]() { try {
												this->execute();
											} catch (...) {
												//TODO log  
											}
											running = false;
											} }
	{}

	Execute::~Execute() {
		running = false;
		task.join();
	}

	void Execute::execute() const {
		std::list<entity::JobInformation> jobList;
		while (running) {
			auto newJobs = persistentData->loadJobInformation();
			jobList.splice(jobList.end(), newJobs);
	
			auto jobListIterator = jobList.begin();
			while (running && workers->size() && jobList.size()) {
				auto& jobInformation = *jobListIterator;
				executeJob(jobInformation);
				if (jobInformation.sizeOfNextWorkerGroup() > 0) {
					jobListIterator = std::next(jobListIterator);
				} else {
					jobListIterator = jobList.erase(jobListIterator);
				}
			}
			
			if(running) periodicSleeper->trySleeping();
		}
	}

	void Execute::executeJob(entity::JobInformation & jobInformation) const {
		auto workerGroupSize = jobInformation.sizeOfNextWorkerGroup();
		if (workerGroupSize && (workerGroupSize > workers->size())) {
			auto jobDirectory = std::make_shared<filesystem::JobDirectory>(sharedData->getRootPath(), jobInformation.getClientID(), jobInformation.getJobName());
			while (workerGroupSize && (workerGroupSize > workers->size())) {
				///EXECUTE JOBS
				auto executedJob = formExecutedJob(jobInformation);
				std::stringstream aliases;
				std::stringstream exePaths;
				bool first = true;
				for (int i = 0; i < workerGroupSize; ++i) {
					/// DEPLOY DATA
					auto worker = std::move(workers->pop_front());
					const auto workingDirectory = jobDirectory->deployData(*worker->getRootDirectory().get(), i);
					worker->setWorkingDirectory(std::make_shared<filesystem::Directory>(workingDirectory));
					
					if (first) {
						executedJob->setZeroNode(worker->getAlias);
					} else {
						first = false;
					}
					aliases << " " << worker->getAlias();
					exePaths << " " << R"(")" << workingDirectory.appendToPath(jobInformation.getExecutablePath()) << R"(")";

					executedJob->addWorker(std::move(worker));
				}

				const auto outputFile = jobDirectory->getResultFilePath(jobInformation.getNeededWorkers());
				auto processId = makeProcess(jobInformation, std::to_string(workerGroupSize), aliases.str(), exePaths.str(), outputFile);
				executedJob->setProcessId(processId);
				forwardExecutedJob(std::move(executedJob));

				jobInformation.substractNeededWorkers(workerGroupSize);
				workerGroupSize = jobInformation.sizeOfNextWorkerGroup();
			}
		}	
	}

	int Execute::makeProcess(const entity::JobInformation& jobInformation, const std::string& num, const std::string& aliases, const std::string& exePaths, const std::string& outputFile) const {
		switch (jobInformation.getJobType()) {
		case entity::JobType::SERIAL: makeSerialProcess(jobInformation, aliases, exePaths, outputFile);
		case entity::JobType::PARALLEL: makeParallelProcess(jobInformation, num, aliases, exePaths, outputFile);
		default: throw std::runtime_error("Execute::makeProcess unsupported JobType");;
		}
	}

	int Execute::makeSerialProcess(const entity::JobInformation & jobInformation, const std::string & aliases, const std::string & exePaths, const std::string & outputFile) const {
		std::stringstream order;
		order << R"(")" << this->managerConfiguration->getMpiexecPath() << R"(")"
			<< " -hosts 1"
			<< aliases
			<< " -noprompt "
			<< R"(")" << this->managerConfiguration->getDetachedExecutorPath() << R"(")"
			<< exePaths
			<< " " << jobInformation.getArgs()
			<< " > "
			<< outputFile;

		return 0;
	}

	int Execute::makeParallelProcess(const entity::JobInformation & jobInformation, const std::string & num, const std::string & aliases, const std::string & exePaths, const std::string& outputFile)  const {
		std::stringstream order;
		order << R"(")" << this->managerConfiguration->getMpiexecPath() << R"(")"
			<< " -hosts "
			<< num
			<< aliases
			<< " -noprompt "
			<< exePaths
			<< " " << jobInformation.getArgs()
			<< " > "
			<< outputFile;

		return 0;
	}

	std::unique_ptr<entity::ExecutedJob> Execute::formExecutedJob(const entity::JobInformation& jobInformation) {
		return std::make_unique<entity::ExecutedJob>(
			jobInformation.getClientID(),
			jobInformation.getJobName(),
			jobInformation.getJobID(),
			std::string(),
			int()
		);
	}
}