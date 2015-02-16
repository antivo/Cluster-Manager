#include "entity_prepared_job.h"

#include "entity_job_information.h"
#include "entity_worker.h"

namespace entity {
	PreparedJob::PreparedJob() {}
	PreparedJob::~PreparedJob() {}

	std::string PreparedJob::getAliases() const {
		return aliases;
	}

	std::string PreparedJob::getExePaths() const {
		return exePaths;
	}

	std::string PreparedJob::getOutputFile() const {
		return outputFile;
	}

	std::string PreparedJob::getWorkingDirectory() const {
		return workingDirectory;
	}

	void PreparedJob::setAliases(const std::string& aliases) {
		this->aliases = aliases;
	}

	void PreparedJob::setExePaths(const std::string& exePaths) {
		this->exePaths = exePaths;
	}

	void PreparedJob::setOutputFile(const std::string& outputFIle) {
		this->outputFile = outputFIle;
	}

	void PreparedJob::setWorkingDirectory(const std::string& workingDirectory) {
		this->workingDirectory = workingDirectory;
	}

	std::size_t PreparedJob::getWorkerSize() const {
		return this->workers.size();
	}

	std::unique_ptr<entity::Worker> PreparedJob::popWorker() {
		auto worker = std::move(workers.top());
		this->workers.pop();
		return worker;
	}
	
	void PreparedJob::addWorker(std::unique_ptr<entity::Worker>&& worker) {
		this->workers.push(std::move(worker));
	}
}