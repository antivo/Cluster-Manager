#ifndef CONFIGURATION_SHARED_SERVER_MANAGER
#define CONFIGURATION_SHARED_SERVER_MANAGER

#include <string>
#include <memory>

namespace configuration {
	class SharedServerManager {
	public:
		static std::shared_ptr<configuration::SharedServerManager> makeDefault();

		SharedServerManager(const int databasePort,
												const std::string& databaseHostname,
												const std::string& databaseUsername,
												const std::string& databasePassword,
												const std::string& databaseName,
												const std::string& rootPath,
												const std::string& dataDirectoryName, 
												const std::string& resultDirectoryName);
		~SharedServerManager();

		
		int getDatabsePort() const;
		std::string getDatabaseHostname() const;;
		std::string getDatabaseUsername() const;
		std::string getDatabasePassword() const;
		std::string getDatabaseName() const;
		std::string getRootPath() const;
		std::string getDataDirectoryName() const;
		std::string getResultDirectoryName() const;

		void setDatabsePort(const int databasePort);
		void setDatabaseHostname(const std::string& databaseHostname);
		void getDatabaseUsername(const std::string& databaseUsername);
		void getDatabasePassword(const std::string& databasePassword);
		void getDatabaseName(const std::string& databaseName);
		void setRootPath(const std::string& rootPath);
		void setDataDirectoryName(const std::string& dataDirectoryName);
		void setResultDirectoryName(const std::string& recvDirectoryName);
		
	private:
		int databasePort;
		std::string databaseHostname;
		std::string databaseUsername;
		std::string databasePassword;
		std::string databaseName;
		std::string rootPath;
		std::string dataDirectoryName;
		std::string resultDirectoryName;
	};

}
#endif