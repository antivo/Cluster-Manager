#include "entity_executed_job.h"

#include "entity_worker.h"
#include "filesystem_job_directory.h"

namespace entity {
	ExecutedJob::ExecutedJob() {}
	ExecutedJob::ExecutedJob(const std::string& clientID,
		const std::string& jobName,
		const std::string& jobId,
		const std::string& zeroNode,
		const int processID) :
			clientID(clientID),
			jobName(jobName),
			jobID(jobID), 
			zeroNode(zeroNode),
			processID(processID)
	{}

	ExecutedJob::~ExecutedJob() {}
	
	std::string ExecutedJob::getJobID() const {
		return jobID;
	}

	std::string ExecutedJob::getZeroNode() const {
		return zeroNode;
	}

	int ExecutedJob::getProcessID() const {
		return processID;
	}

	std::size_t ExecutedJob::getWorkerSize() const {
		return workers.size();
	}

	std::unique_ptr<entity::Worker> ExecutedJob::popWorker() {
		auto worker = std::move(workers.top());
		workers.pop();
		return worker;
	}

	void ExecutedJob::addWorker(std::unique_ptr<entity::Worker>&& worker) {
		this->workers.push(std::move(worker));
	}

	void ExecutedJob::setClientID(const std::string& clientID) {
		this->clientID = clientID;
	}

	void ExecutedJob::setJobName(const std::string & jobName) {
		this->jobName = jobName;
	}

	void ExecutedJob::setJobID(const std::string & jobID) {
		this->jobID = jobID;
	}

	void ExecutedJob::setZeroNode(const std::string & zeroNode) {
		this->zeroNode = zeroNode;
	}
	
	void ExecutedJob::setProcessId(const int processID) {
		this->processID = processID;
	}
}