#ifndef CONFIGURATION_MANAGER
#define CONFIGURATION_MANAGER

#include <string>
#include <memory>

namespace configuration {
	class Manager {
	public:
		static std::shared_ptr<configuration::Manager> makeDefault();
		
		Manager(const std::string& mpiexecPath, 
						const std::string& detachedExecutorPath, 
						const std::string& attachedInspector,
						const std::string& resultFilePath);
		~Manager();
		
		std::string getMpiexecPath() const;
		std::string getDetachedExecutorPath() const;
		std::string getAttachedInspector() const;
		std::string getResultFilePath() const;

		void setMpiexecPath(const std::string& mpiexecPath);
		void setDetachedExecutorPath(const std::string& detachedExecutorPath);
		void setAttachedInspector(const std::string& attachedInspectorPath);
		void setResultFilePath(const std::string& resultFilePath);

	private:
		std::string mpiexecPath;
		std::string detachedExecutorPath;
		std::string attachedInspectorPath;
		std::string resultFilePath;
	};
}

#endif