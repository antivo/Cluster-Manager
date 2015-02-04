#ifndef EXECUTED_JOB
#define EXECUTED_JOB

#include <string>
#include <stack>
#include <memory>


namespace entity {
	class Worker;

	class ExecutedJob {
	public:
		ExecutedJob();
		ExecutedJob(const std::string& clientID,
								const std::string& jobName,
								const std::string& jobId,
								const std::string& zeroNode,
								const int processID);
		~ExecutedJob();
		
		std::string getClientID() const;
		std::string getJobName() const;
		std::string getJobID() const;
		std::string getZeroNode() const;
		int getProcessID() const;
		
		std::size_t getWorkerSize() const;

		std::unique_ptr<entity::Worker> popWorker();
		void addWorker(std::unique_ptr<entity::Worker>&& worker);
		
		void setClientID(const std::string& clientID);
		void setJobName(const std::string& jobName);
		void setJobID(const std::string& jobID);
		void setZeroNode(const std::string& zeroNode);
		void setProcessId(const int processID);
		
	private:
		std::string clientID;
		std::string jobName;
		std::string jobID;
		std::string zeroNode;
		int processID;
		std::stack<std::unique_ptr<entity::Worker>> workers;
	};

}

#endif