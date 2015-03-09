#ifndef THREAD_EXECUTED_JOB
#define THREAD_EXECUTED_JOB

#include <atomic>
#include <memory>
#include <thread>

namespace entity {
	class JobInformation;
	class PreparedJob;
}

namespace thread {
	class ExecutedJob {
	public:
		ExecutedJob(std::unique_ptr<entity::PreparedJob>&& preparedJob, const entity::JobInformation& jobInformation, const std::string& order, const std::string& outFile);
		~ExecutedJob();

		std::string getClientID() const;
		std::string getJobName() const;
		std::string getJobID() const;

		bool isRunning() const;
		bool isReady() const;

		std::unique_ptr<entity::PreparedJob> release();

	private:
		std::unique_ptr<entity::PreparedJob> preparedJob;
		std::string clientID;
		std::string jobName;
		std::string jobID;
		std::string order;
		std::string outFile;
		std::atomic<bool> running;
		std::thread task;

		ExecutedJob(ExecutedJob&) = delete;
		ExecutedJob& operator=(ExecutedJob&) = delete;
	};
}

#endif