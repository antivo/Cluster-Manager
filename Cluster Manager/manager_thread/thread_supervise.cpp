#include "thread_supervise.h"

#include "configuration_shared_server_manager.h"
#include "filesystem_job_directory.h"
#include "persistent_manager_data.h"
#include "utility_periodic_sleeper.h"
#include "thread_execute.h"

namespace thread {
	Supervise::Supervise(std::unique_ptr<utility::PeriodicSleeper>&& periodicSleeper,
		const std::shared_ptr<configuration::SharedServerManager>& sharedData,
 	  const std::shared_ptr<persistent::ManagerData>& persistentData,
		const std::shared_ptr<locked::WorkerQueue>& workers) :
			executedJobs{ std::make_unique<locked::ExecutedJobQueue>() },
			periodicSleeper{ std::move(periodicSleeper) },
			sharedData{ sharedData },
			workers{ workers }, 
			persistentData{ persistentData },
			running{ true },
			task([this]() { try {
												this->supervise();
											} catch (...) {
												//TODO log  
											}
											running = false;
										})
	{}

	Supervise::~Supervise() {
		running = false;
		task.join();
	}

	bool Supervise::isRunning() const {
		return running;
	}

	void Supervise::insertExecutedJob(std::unique_ptr<entity::ExecutedJob>&& executedJob) {
		this->executedJobs->push_back(std::move(executedJob));
	}

	void Supervise::supervise() const {
		while(running) {
			if (executedJobs->size()) {
				auto executedJob = executedJobs->pop_front();
				auto processId = executedJob->getProcessID();
				auto finished = true; // !utility::processIsRunning(processId);
				if (finished) {
					finishJobCycle(std::move(executedJob));
				} else {
					executedJobs->push_back(std::move(executedJob));
				}
			}
			if(running)	this->periodicSleeper->trySleeping();
		}
	}

	void Supervise::finishJobCycle(std::unique_ptr<entity::ExecutedJob> executedJob) const {
		/*const auto workersDone = executedJob->getWorkerSize();
		auto jobDirectory = executedJob->getResultDirectory();
		jobDirectory->prepareForResultIncome(sharedData->getResultDirectoryName());
		while(executedJob->getWorkerSize()) {
			auto worker = executedJob->popWorker();
			auto resultDir = worker->getWorkingDirectory();
			jobDirectory->fetchResult(*worker->getWorkingDirectory());
			workers->push_back(std::move(worker));
		}
		persistentData->substractWorkersDone(executedJob->getJobID(), workersDone);*/
	}

}