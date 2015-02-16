#include "filesystem_job_directory.h"

#include <sstream>

#include "filesystem_common.h"
#include "filesystem_directory.h"
#include "filesystem_directory_common.h"
#include "filesystem_file_common.h"

namespace filesystem {
	const std::string JobDirectory::deployedDirectory("Worker_");
	const std::string JobDirectory::resultFile("_out.txt");

  JobDirectory::JobDirectory(const std::string& rootPath, const std::string& clientId, const std::string& jobName) : 
		jobDirectory(std::make_unique<Directory>(filesystem::addToPath(filesystem::addToPath(rootPath, clientId), jobName)))
	{}
  
  JobDirectory:: ~JobDirectory() {}

	void JobDirectory::prepareForDataIncome(const std::string & dataDirectory) {
		this->data = std::make_unique<filesystem::Directory>(jobDirectory->makeSubdirectory(dataDirectory));
	}

	void JobDirectory::prepareForResultIncome(const std::string& resultDirectory) {
		this->result = std::make_unique<filesystem::Directory>(jobDirectory->makeSubdirectory(resultDirectory));
	}

  void JobDirectory::remove() {
    jobDirectory->remove();
  }

	void JobDirectory::storeData(const std::string & relativePath, const std::string & content) const {
		this->data->store(relativePath, content);
	}

	Directory JobDirectory::deployData(const Directory & dst, const int uuid) const {
		const auto destinationDir = dst.makeSubdirectory(JobDirectory::deployedDirectory + std::to_string(uuid));
		this->data->copy(destinationDir);
		return destinationDir;
	}

	void JobDirectory::fetchResult(Directory & src) const {
		src.copyAndRemove(*result.get());
	}
	
	std::string JobDirectory::getResultFilePath(const int uuid) const {
		std::string filename( std::to_string(uuid) + JobDirectory::resultFile );
		return this->result->appendToPath(filename);
	}


	std::vector<std::string> JobDirectory::getResultTopology() const {
		return this->result->createTopology();
	}

	std::vector<std::string> JobDirectory::getRelativeResultTopology() const {
		auto topology = this->result->createTopology();
		const auto prefixSize = result->getPath().size() + 1;
		for (auto& ss : topology) {
			ss = ss.substr(prefixSize);
		}
		return topology;
	}
}