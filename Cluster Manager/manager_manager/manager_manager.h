#ifndef MANAGER_MANAGER
#define MANAGER_MANAGER

#include <string>
#include <memory>
#include <vector>

#include "entity_worker_queue.h"

namespace entity {
	class Worker;
}

namespace configuration {
	class Manager;
	class SharedServerManager;
}

namespace persistent {
	class ManagerData;
}

namespace thread {
	class Supervise;
	class Execute;
}

namespace manager {
	class Manager {
	public:
		Manager(const std::shared_ptr<persistent::ManagerData>& persistentData,
						const std::shared_ptr<configuration::Manager>& managerConfiguration,
						const std::shared_ptr<configuration::SharedServerManager>& sharedData);
		~Manager();

		void addWorker(std::unique_ptr<entity::Worker>&& worker);
		std::size_t getSizeOfWorkers() const;

		bool run();
		void stop();

	private:
		std::shared_ptr<entity::WorkerQueue> workers;
		std::shared_ptr<persistent::ManagerData> persistentData;
		std::shared_ptr<configuration::Manager> managerConfiguration;
		std::shared_ptr<configuration::SharedServerManager> sharedData;

		std::unique_ptr<const thread::Execute> execute;
		std::unique_ptr<thread::Supervise> supervise;
	};
}

#endif