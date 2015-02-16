#include "thread_executed_job.h"

#include <memory>

#include "entity_job_information.h"
#include "entity_prepared_job.h"
#include "utility_process.h"

#include <iostream>

namespace thread {
	ExecutedJob::ExecutedJob(std::unique_ptr<entity::PreparedJob>&& preparedJob, const entity::JobInformation& jobInformation, const std::string& order) :
		preparedJob(std::move(preparedJob)),
		clientID(jobInformation.getClientID()),
		jobName(jobInformation.getJobName()),
		jobID(jobInformation.getJobID()),
		order(order),
		running(true),
		task ( 
			[this]() {
		std::cout << "POCINJEEEEEE" << std::endl;
								try {
									std::cout << this->order << std::endl;
									utility::createProcessAndWaitForResult(this->order.c_str());
									std::cout << this->order << std::endl;
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

	std::unique_ptr<entity::PreparedJob> ExecutedJob::release() {
		return std::move(preparedJob);
	}
}