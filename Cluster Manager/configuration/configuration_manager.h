#ifndef CONFIGURATION_MANAGER
#define CONFIGURATION_MANAGER

#include <memory>
#include <string>

namespace configuration {
	class Manager {
	public:
		static std::shared_ptr<configuration::Manager> makeDefault();
		
		Manager(const std::string& mpiexecPath, 
						const std::string& detachedExecutorPath, 
						const std::string& attachedInspector);
		~Manager();
		
		std::string getMpiexecPath() const;
		std::string getDetachedExecutorPath() const;
		std::string getAttachedInspector() const;

		void setMpiexecPath(const std::string& mpiexecPath);
		void setDetachedExecutorPath(const std::string& detachedExecutorPath);
		void setAttachedInspector(const std::string& attachedInspectorPath);

	private:
		std::string mpiexecPath;
		std::string detachedExecutorPath;
		std::string attachedInspectorPath;
	};
}

#endif