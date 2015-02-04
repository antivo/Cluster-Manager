#ifndef MANAGER_RESOURCE_WORKER
#define MANAGER_RESOURCE_WORKER

#include <memory>
#include <string>

namespace filesystem {
	class Directory;
}

namespace entity {
	class Worker {
	public:
		Worker(const std::string& alias, const std::string& rootDirectory);

		std::string getAlias() const;
		std::shared_ptr<filesystem::Directory> getRootDirectory() const;
		std::shared_ptr<filesystem::Directory> getWorkingDirectory() const;

		void setWorkingDirectory(std::shared_ptr<filesystem::Directory> workingDirectory);

	private:
		std::string alias;
		std::shared_ptr<filesystem::Directory> rootDirectory;
		std::shared_ptr<filesystem::Directory> workingDirectory;
	};
}

#endif