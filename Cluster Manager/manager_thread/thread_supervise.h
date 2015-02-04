#ifndef MANAGER_THREAD_SUPERVISE
#define MANAGER_THREAD_SUPERVISE

#include <atomic>
#include <memory>
#include <vector>
#include <thread>

#include "entity_worker_queue.h"
#include "entity_executed_job_queue.h"

namespace configuration {
	class SharedServerManager;
}

namespace entity {
	class ExecutedJob;
}

namespace persistent {
	class ManagerData;
}

namespace utility {
	class PeriodicSleeper;
}

namespace thread {
	class Supervise {
	public:
		Supervise(std::unique_ptr<utility::PeriodicSleeper>&& periodicSleeper,
							const std::shared_ptr<configuration::SharedServerManager>& sharedData,
							const std::shared_ptr<persistent::ManagerData>& persistentData,
							const std::shared_ptr<locked::WorkerQueue>& workers);
		~Supervise();
		
		bool isRunning() const;
		void insertExecutedJob(std::unique_ptr<entity::ExecutedJob>&& executedJob);

	private:
		std::unique_ptr<locked::ExecutedJobQueue> executedJobs;
		std::unique_ptr<utility::PeriodicSleeper> periodicSleeper;
		std::shared_ptr<configuration::SharedServerManager> sharedData;
		std::shared_ptr<persistent::ManagerData> persistentData;
		std::shared_ptr<locked::WorkerQueue> workers;
		std::atomic<bool> running;
		std::thread task;

		void supervise() const;
		void finishJobCycle(std::unique_ptr<entity::ExecutedJob> executedJob) const;

		Supervise(const Supervise&) = delete;
		Supervise& operator=(const Supervise&) = delete;
	};
}

#endif