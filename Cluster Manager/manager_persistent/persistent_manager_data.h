#ifndef MANAGER_PERSISTENT_MANAGER_DATA
#define MANAGER_PERSISTENT_MANAGER_DATA

#include <vector>
#include <memory>
#include <list>
#include <string>

namespace entity {
	class JobInformation;
}

namespace persistent {
	class ManagerData {
	public:
		/// (loaded flags)
		virtual void resetManagerSpecificData() const = 0;
		virtual std::list<entity::JobInformation> loadJobInformation() const = 0;
		virtual void substractWorkersDone(const std::string& jobId, const std::size_t workers) const = 0;

		virtual ~ManagerData();
	};
}

#endif
