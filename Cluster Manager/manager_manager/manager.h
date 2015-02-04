#ifndef MANAGER_MANAGER
#define MANAGER_MANAGER

#include <string>
#include <memory>
#include <vector>

#include "entity_worker_queue.h"

namespace configuration {
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
		Manager(std::shared_ptr<persistent::ManagerData> persistentData,
						std::shared_ptr<configuration::SharedServerManager>);

		void executeOrder(const std::string& order);

	private:
		std::unique_ptr<thread::Supervise> supervise;
		std::unique_ptr<thread::Execute> execute;
	
		std::shared_ptr<persistent::ManagerData> persistentData;

		std::shared_ptr<locked::WorkerQueue> workers;
	};
}

#endif