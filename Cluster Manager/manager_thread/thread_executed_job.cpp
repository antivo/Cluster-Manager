#include "thread_executed_job.h"

#include <memory>
#include <chrono>

#include "configuration_default.h"
#include "entity_job_information.h"
#include "entity_prepared_job.h"
#include "utility_process.h"

namespace thread {
	ExecutedJob::ExecutedJob(std::unique_ptr<entity::PreparedJob>&& preparedJob, 
													 const entity::JobInformation& jobInformation,
													 const std::string& order, 
													 const std::string& outFile) :
		preparedJob(std::move(preparedJob)),
		clientID(jobInformation.getClientID()),
		jobName(jobInformation.getJobName()),
		jobID(jobInformation.getJobID()),
		order(order),
		outFile(outFile),
		running(true),
		task ( 
			[this]() {
								try {
									utility::createProcess(this->order.c_str(), this->outFile.c_str());
								} catch (...) {
									//TODO log  
								}
								running = false;
						 })
		{}
	
	ExecutedJob::~ExecutedJob() {
		task.join();
	}

	std::string ExecutedJob::getClientID() const {
		return clientID;
	}

	std::string ExecutedJob::getJobName() const {
		return jobName;
	}

	std::string ExecutedJob::getJobID() const {
		return jobID;
	}

	bool ExecutedJob::isRunning() const {
		return running;
	}

	bool ExecutedJob::isReady() const {
		static auto startTime = std::chrono::system_clock::now();
		auto passedTime = std::chrono::system_clock::now() - startTime;
		return passedTime > configuration::EXECUTED_WAITING_DURATION;
	}

	std::unique_ptr<entity::PreparedJob> ExecutedJob::release() {
		return std::move(preparedJob);
	}
}