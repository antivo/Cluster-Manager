#include "thread_supervise.h"

#include <thread>
#include <chrono>

#include "configuration_shared_server_manager.h"
#include "entity_prepared_job.h"
#include "filesystem_job_directory.h"
#include "persistent_manager_data.h"
#include "utility_periodic_sleeper.h"
#include "thread_execute.h"
#include "thread_executed_job.h"

namespace thread {
	Supervise::Supervise(std::unique_ptr<utility::PeriodicSleeper>&& periodicSleeper,
		const std::shared_ptr<configuration::SharedServerManager>& sharedData,
 	  const std::shared_ptr<persistent::ManagerData>& persistentData,
		const std::shared_ptr<entity::WorkerQueue>& workers) :
			executedJobs{ std::make_unique<thread::ExecutedJobQueue>() },
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

	void Supervise::insertExecutedJob(std::unique_ptr<thread::ExecutedJob>&& executedJob) {
		this->executedJobs->push_back(std::move(executedJob));
	}

	void Supervise::supervise() const {
		while(running) {
			auto size = executedJobs->size();
			while(size != 0) {
				auto executedJob = executedJobs->pop_front();
				if (!executedJob->isRunning()) {
					using namespace std::literals;
					std::this_thread::sleep_for(2min); // filesystem to synchronize... sometimes files are multiple or huge. FIND BETTER SOLUTION
					finishJobCycle(std::move(executedJob));
				} else {
					executedJobs->push_back(std::move(executedJob));
				}
				--size;
			}
			if(running)	this->periodicSleeper->trySleeping();
		}
	}

	void Supervise::finishJobCycle(std::unique_ptr<thread::ExecutedJob> executedJob) const {
		auto preparedJob = std::move(executedJob->release());
		const auto workersDone = preparedJob->getWorkerSize();
		auto jobDirectory = std::make_shared<filesystem::JobDirectory>(sharedData->getRootPath(), executedJob->getClientID(), executedJob->getJobName());
		jobDirectory->prepareForResultIncome(sharedData->getResultDirectoryName());
		while(preparedJob->getWorkerSize()) {
			auto worker = preparedJob->popWorker();
			jobDirectory->fetchResult(*worker->getWorkingDirectory());
			workers->push_back(std::move(worker));
		}
		persistentData->substractWorkersDone(executedJob->getJobID(), workersDone);
	}
}