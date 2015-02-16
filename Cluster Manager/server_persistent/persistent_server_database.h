#ifndef PERSISTENT_SERVER_DATABASE
#define PERSISTENT_SERVER_DATABASE

#include <string>
#include <memory>

#include "persistent_server_data.h"

namespace database {
	class MySQLRealConnection;
}

namespace persistent {
	class ServerDatabase : public ServerData  {
	public:
		explicit ServerDatabase(std::unique_ptr<const database::MySQLRealConnection>&& conn);
		~ServerDatabase();

		virtual std::string getClientID(const std::string& username, const std::string& password) const;
		virtual std::string getInfo(const std::string& clientId) const;

		virtual bool jobExists(const std::string& clientId, const std::string& jobName) const;
		virtual bool jobFinished(const std::string& clientId, const std::string& jobName) const;

		virtual void storeJob(const std::string& clientId, const std::string& jobName, const std::string& jobType, const std::string& neededWorkers, const std::string& args, const std::string& executable) const;
		virtual void removeJob(const std::string& clientId, const std::string& jobName) const;
	
	private:
		std::unique_ptr<const database::MySQLRealConnection> conn;
	};
}

#endif