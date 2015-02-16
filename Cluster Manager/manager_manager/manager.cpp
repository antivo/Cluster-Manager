#include "manager.h"

#include "entity_worker.h"
#include "entity_worker_queue.h"
#include "configuration_manager.h"
#include "database_mysql_real_connection.h"
#include "persistent_manager_database.h"
#include "thread_execute.h"
#include "thread_executed_job.h"
#include "thread_executed_job_queue.h"
#include "thread_supervise.h"
#include "utility_periodic_sleeper.h"

namespace manager {
	Manager::Manager(const std::shared_ptr<persistent::ManagerData>& persistentData,
									 const std::shared_ptr<configuration::Manager>& managerConfiguration,
									 const std::shared_ptr<configuration::SharedServerManager>& sharedData) :
									 workers(std::make_shared<entity::WorkerQueue>()),
									 persistentData(persistentData),
									 managerConfiguration(managerConfiguration),
									 sharedData(sharedData),
									 execute(nullptr),
									 supervise(nullptr) {}

	Manager::~Manager() {
		execute.reset(nullptr);
		supervise.reset(nullptr);
	}

	void Manager::addWorker(std::unique_ptr<entity::Worker>&& worker) {
		workers->push_back(std::move(worker));
	}

	bool Manager::run() {
		using std::placeholders::_1;
		bool started = false;
		if (!execute || !supervise || !execute->isRunning() || !supervise->isRunning()) {
			persistentData->resetManagerSpecificData();
			supervise = std::make_unique<thread::Supervise>(std::move(utility::PeriodicSleeper::makeDefault()),
																											sharedData,
																											persistentData,
																											workers);
			auto forwardExecutedJob = [&](std::unique_ptr<thread::ExecutedJob>&& executedJob) { 
				supervise->insertExecutedJob(std::move(executedJob)); 
			};
			execute = std::make_unique<const thread::Execute>(std::move(utility::PeriodicSleeper::makeDefault()),
																												workers,
																												persistentData,
																												managerConfiguration,
																												sharedData,
																												forwardExecutedJob);
			started = true;
		}
		return started;
	}

	void Manager::stop() {
		execute.reset(nullptr);
		supervise.reset(nullptr);
	}
}
