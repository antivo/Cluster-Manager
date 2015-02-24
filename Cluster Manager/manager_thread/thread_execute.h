#ifndef MANAGER_THREAD_EXECUTE
#define MANAGER_THREAD_EXECUTE

#include <atomic>
#include <functional>
#include <memory>
#include <sstream>
#include <thread>

#include "entity_worker_queue.h"

namespace configuration {
	class SharedServerManager;
	class Manager;
}

namespace entity {
	class JobInformation;
	class PreparedJob;
}

namespace persistent {
	class ManagerData;
}

namespace utility {
	class PeriodicSleeper;
}

namespace filesystem {
	class JobDirectory;
}

namespace thread {
	class ExecutedJob;

	class Execute {
	public:
		Execute(std::unique_ptr<utility::PeriodicSleeper>&& periodicSleeper,
						const std::shared_ptr<entity::WorkerQueue>& workers,
					  const std::shared_ptr<persistent::ManagerData>& persistentData,
						const std::shared_ptr<configuration::Manager>& managerConfiguration,
						const std::shared_ptr<configuration::SharedServerManager>& sharedData,
						const std::function<void(std::unique_ptr<thread::ExecutedJob>&&)>& forwardExecutedJob);
		~Execute();
		
		bool isRunning() const;

	private:
		std::shared_ptr<persistent::ManagerData> persistentData;
		std::shared_ptr<configuration::Manager> managerConfiguration;
		std::shared_ptr<configuration::SharedServerManager> sharedData;
		std::function<void(std::unique_ptr<thread::ExecutedJob>)> forwardExecutedJob;
		std::unique_ptr<utility::PeriodicSleeper> periodicSleeper;
		std::shared_ptr<entity::WorkerQueue> workers;
		std::atomic<bool> running;
		std::thread task;

		void execute() const;
		void executeJob(entity::JobInformation& jobInformation) const;
		void depositExecutedJob(const entity::JobInformation& jobInformation, const filesystem::JobDirectory& jobDirectory) const;
		std::unique_ptr<entity::PreparedJob> prepare(const entity::JobInformation& jobInformation, const filesystem::JobDirectory& jobDirectory) const;
		std::unique_ptr<thread::ExecutedJob> makeExecutedJob(const entity::JobInformation& jobInformation, std::unique_ptr<entity::PreparedJob>&& preparedJob) const;

		Execute(const Execute&) = delete;
		Execute& operator=(const Execute&) = delete;
	};
}

#endif