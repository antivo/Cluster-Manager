#ifndef MANAGER_PERSISTENT_MANAGER_DATA
#define MANAGER_PERSISTENT_MANAGER_DATA

#include <memory>
#include <list>
#include <string>
#include <vector>

namespace entity {
	class JobInformation;
}

namespace persistent {
	class ManagerData {
	public:
		/// reset `loaded` to false
		virtual void resetManagerSpecificData() const = 0;
		virtual std::list<entity::JobInformation> loadJobInformation() const = 0;
		virtual void substractWorkersDone(const std::string& jobId, const std::size_t workers) const = 0;

		virtual ~ManagerData();
	};
}

#endif
