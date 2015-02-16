#ifndef ENTITY_PREPARED_JOB
#define ENTITY_PREPARED_JOB

#include <stack>
#include <string>
#include <memory>

namespace entity {
	class JobInformation;
	class Worker;
	enum class JobType;

	class PreparedJob {
	public:
		PreparedJob();
		~PreparedJob();

		std::string getAliases() const;
		std::string getExePaths() const;
		std::string getOutputFile() const;
		std::string getWorkingDirectory() const;
	
		void setAliases(const std::string& aliases);
		void setExePaths(const std::string& exePaths);
		void setOutputFile(const std::string& outputFIle);
		void setWorkingDirectory(const std::string& workingDirectory);
		
		std::size_t getWorkerSize() const;
		std::unique_ptr<entity::Worker> popWorker();
		void addWorker(std::unique_ptr<entity::Worker>&& worker);

	private:
		std::string aliases;
		std::string exePaths;
		std::string outputFile;
		std::string workingDirectory;

		std::stack<std::unique_ptr<entity::Worker>> workers;
		
		PreparedJob(PreparedJob&) = delete;
		PreparedJob& operator=(PreparedJob&) = delete;
	};
}

#endif