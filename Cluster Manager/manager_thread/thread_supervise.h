#ifndef MANAGER_THREAD_SUPERVISE
#define MANAGER_THREAD_SUPERVISE

#include <atomic>
#include <memory>
#include <vector>
#include <thread>

#include "entity_worker_queue.h"
#include "thread_executed_job_queue.h"

namespace configuration {
	class SharedServerManager;
}

namespace persistent {
	class ManagerData;
}

namespace utility {
	class PeriodicSleeper;
}

namespace thread {
	class ExecutedJob;
	class Supervise {
	public:
		Supervise(std::unique_ptr<utility::PeriodicSleeper>&& periodicSleeper,
							const std::shared_ptr<configuration::SharedServerManager>& sharedData,
							const std::shared_ptr<persistent::ManagerData>& persistentData,
							const std::shared_ptr<entity::WorkerQueue>& workers);
		~Supervise();
		
		bool isRunning() const;
		void insertExecutedJob(std::unique_ptr<thread::ExecutedJob>&& executedJob);

	private:
		std::unique_ptr<thread::ExecutedJobQueue> executedJobs;
		std::unique_ptr<utility::PeriodicSleeper> periodicSleeper;
		std::shared_ptr<configuration::SharedServerManager> sharedData;
		std::shared_ptr<persistent::ManagerData> persistentData;
		std::shared_ptr<entity::WorkerQueue> workers;
		std::atomic<bool> running;
		std::thread task;

		void supervise() const;
		void finishJobCycle(std::unique_ptr<thread::ExecutedJob> executedJob) const;

		Supervise(const Supervise&) = delete;
		Supervise& operator=(const Supervise&) = delete;
	};
}

#endif