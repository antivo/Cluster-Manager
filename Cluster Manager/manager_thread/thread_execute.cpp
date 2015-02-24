#include "thread_execute.h"

#include <memory>
#include <queue>
#include <vector>
#include <sstream>

#include "assert_common.h"
#include "configuration_manager.h"
#include "configuration_shared_server_manager.h"
#include "entity_job_type.h"
#include "entity_job_information.h"
#include "entity_prepared_job.h"
#include "filesystem_directory.h"
#include "filesystem_job_directory.h"
#include "persistent_manager_data.h"
#include "thread_executed_job.h"
#include "utility_periodic_sleeper.h"
#include "utility_process.h"

namespace thread {
	Execute::Execute(std::unique_ptr<utility::PeriodicSleeper>&& periodicSleeper, 
		const std::shared_ptr<entity::WorkerQueue>& workers,
		const std::shared_ptr<persistent::ManagerData>& persistentData, 
		const std::shared_ptr<configuration::Manager>& managerConfiguration,
		const std::shared_ptr<configuration::SharedServerManager>& sharedData,
		const std::function<void(std::unique_ptr<thread::ExecutedJob>&&)>& forwardExecutedJob) :
			periodicSleeper{ std::move(periodicSleeper) },
			workers{ workers },
			persistentData{ persistentData },
			managerConfiguration{ managerConfiguration },
			sharedData{ sharedData },
			forwardExecutedJob{ forwardExecutedJob },
			running(true),
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

	bool Execute::isRunning() const {
		return running;
	}

	void Execute::execute() const {
		std::list<entity::JobInformation> jobList;
		while (running) {
			auto newJobs = persistentData->loadJobInformation();
			jobList.splice(jobList.end(), newJobs);
			auto jobListIterator = jobList.begin();
			while (running && (jobList.end() != jobListIterator) && workers->size()) {
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
		std::shared_ptr<filesystem::JobDirectory> jobDirectory = nullptr; 
		while (workerGroupSize && (workerGroupSize <= workers->size())) {
			if (nullptr == jobDirectory) {
				jobDirectory = std::make_shared<filesystem::JobDirectory>(sharedData->getRootPath(), jobInformation.getClientID(), jobInformation.getJobName());
				jobDirectory->prepareForDataIncome(sharedData->getDataDirectoryName());
				jobDirectory->prepareForResultIncome(sharedData->getResultDirectoryName());
			}
			depositExecutedJob(jobInformation, *jobDirectory.get());
			jobInformation.substractNeededWorkers(workerGroupSize);
			workerGroupSize = jobInformation.sizeOfNextWorkerGroup();
		}
	}

	void Execute::depositExecutedJob(const entity::JobInformation& jobInformation, const filesystem::JobDirectory& jobDirectory) const {
		std::stringstream aliases, exePaths;
		auto preparedJob = std::move(prepare(jobInformation, jobDirectory));
		auto executedJob = makeExecutedJob(jobInformation, std::move(preparedJob));
		forwardExecutedJob(std::move(executedJob));
	}

	std::unique_ptr<entity::PreparedJob> Execute::prepare(const entity::JobInformation& jobInformation, const filesystem::JobDirectory& jobDirectory) const {
		auto preparedJob = std::make_unique<entity::PreparedJob>();
		auto workerGroupSize = jobInformation.sizeOfNextWorkerGroup();
		std::stringstream aliases, exePaths;
		for (int i = 0; i < workerGroupSize; ++i) {
			const auto orderNum = jobInformation.getNeededWorkers() - i;
			auto worker = std::move(workers->pop_front());
			const auto workingDirectory = jobDirectory.deployData(*worker->getRootDirectory().get(), orderNum);
			worker->setWorkingDirectory(std::make_shared<filesystem::Directory>(workingDirectory));
			aliases << " " << worker->getAlias();
			exePaths << " " << R"(")" << workingDirectory.appendToPath(jobInformation.getExecutablePath()) << R"(")";
			preparedJob->addWorker(std::move(worker));
			if (!(preparedJob->getWorkingDirectory().size())) {
				preparedJob->setWorkingDirectory(workingDirectory.getPath());
			} 
		}
		preparedJob->setAliases(aliases.str());
		preparedJob->setExePaths(exePaths.str());
		const auto outputFile = jobDirectory.getResultFilePath(jobInformation.getNeededWorkers());
		preparedJob->setOutputFile(outputFile);
		return std::move(preparedJob);
	}

	std::unique_ptr<thread::ExecutedJob> Execute::makeExecutedJob(const entity::JobInformation& jobInformation, std::unique_ptr<entity::PreparedJob>&& preparedJob) const {
		std::stringstream order;
		order << R"(")" << this->managerConfiguration->getMpiexecPath() << R"(")"
			<< " -wdir " << preparedJob->getWorkingDirectory()
			<< " -hosts "
			<< jobInformation.sizeOfNextWorkerGroup()
			<< preparedJob->getAliases()
			<< " -noprompt "
			<< preparedJob->getExePaths()
			<< " " << jobInformation.getArgs();
		const auto toExec = order.str();
		return std::make_unique<thread::ExecutedJob>(std::move(preparedJob), jobInformation, toExec, preparedJob->getOutputFile());
	}
}