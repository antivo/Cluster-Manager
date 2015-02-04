#ifndef CONFIGURATION_SHARED_SERVER_MANAGER
#define CONFIGURATION_SHARED_SERVER_MANAGER

#include <string>
#include <memory>

namespace configuration {

	class SharedServerManager {
	public:
		static std::shared_ptr<configuration::SharedServerManager> makeDefault();

		SharedServerManager(std::string rootPath, std::string dataDirectoryName, std::string resultDirectoryName);
		~SharedServerManager();

		std::string getRootPath() const;
		std::string getDataDirectoryName() const;
		std::string getResultDirectoryName() const;

		void setRootPath(const std::string& rootPath);
		void setDataDirectoryName(const std::string& dataDirectoryName);
		void setResultDirectoryName(const std::string& recvDirectoryName);
		
	private:
		std::string rootPath;
		std::string dataDirectoryName;
		std::string resultDirectoryName;
	};

}
#endif