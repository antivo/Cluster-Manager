#ifndef SERVER_PERSISTENT_DATA
#define SERVER_PERSISTENT_DATA

#include <string>

namespace persistent {
	class ServerData {
	public:
		virtual std::string getClientID(const std::string& username, const std::string& password) const = 0;
		virtual std::string getInfo(const std::string& clientId) const = 0;
		
		virtual bool jobExists(const std::string& id, const std::string& job) const  = 0;
		virtual bool jobFinished(const std::string& id, const std::string& job) const = 0;
		
		virtual void storeJob(const std::string& id, const std::string& jobName, const std::string& jobType, const std::string& neededWorkers, const std::string& args, const std::string& executable) const = 0;
		virtual void removeJob(const std::string& id, const std::string& job) const = 0;
	
		virtual ~ServerData();
	};

}

#endif