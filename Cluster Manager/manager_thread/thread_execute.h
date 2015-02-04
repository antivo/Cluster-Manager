#ifndef MANAGER_THREAD_EXECUTE
#define MANAGER_THREAD_EXECUTE

#include <atomic>
#include <memory>
#include <functional>
#include <thread>

#include "entity_worker_queue.h"

namespace configuration {
	class SharedServerManager;
	class Manager;
}

namespace entity {
	class ExecutedJob;
	class JobInformation;
}

namespace persistent {
	class ManagerData;
}

namespace utility {
	class PeriodicSleeper;
}

namespace thread {
	class Execute {
	public:
		Execute(std::unique_ptr<utility::PeriodicSleeper>&& periodicSleeper,
						const std::shared_ptr<locked::WorkerQueue>& workers,
						const std::shared_ptr<persistent::ManagerData>& persistentData,
						const std::shared_ptr<configuration::Manager>& managerConfiguration,
						const std::shared_ptr<configuration::SharedServerManager>& sharedData,
						const std::function<void(std::unique_ptr<entity::ExecutedJob>)>& forwardExecutedJob);
		~Execute();
		
		bool isRunning() const;

	private:
		static std::unique_ptr<entity::ExecutedJob> formExecutedJob(const entity::JobInformation& jobInformation);
		
		std::unique_ptr<utility::PeriodicSleeper> periodicSleeper;
		std::shared_ptr<locked::WorkerQueue> workers;
		std::shared_ptr<persistent::ManagerData> persistentData;
		std::shared_ptr<configuration::Manager> managerConfiguration;
		std::shared_ptr<configuration::SharedServerManager> sharedData;
		std::function<void(std::unique_ptr<entity::ExecutedJob>)> forwardExecutedJob;
		std::atomic<bool> running;
		std::thread task;

		void execute() const;
		void executeJob(entity::JobInformation& jobInformation) const;
		int makeProcess(const entity::JobInformation& jobInformation, const std::string& num, const std::string& aliases, const std::string& exePaths, const std::string& outputFile) const;

		int makeSerialProcess(const entity::JobInformation& jobInformation, const std::string& aliases, const std::string& exePaths, const std::string& outputFile) const;
		int makeParallelProcess(const entity::JobInformation& jobInformation, const std::string& num, const std::string& aliases, const std::string& exePaths, const std::string& outputFile) const;

		//std::unique_ptr<entity::ExecutedJob> executeSerial(const entity::JobInformation& job);
		//std::unique_ptr<entity::ExecutedJob> executeParallel(const entity::JobInformation& job);
		
		Execute(const Execute&) = delete;
		Execute& operator=(const Execute&) = delete;
	};
}

#endif