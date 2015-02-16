#ifndef PERSISTENT_MANAGER_DATABASE
#define PERSISTENT_MANAGER_DATABASE

#include "persistent_manager_data.h"

namespace database {
	class MySQLRealConnection;
}

namespace persistent {
	class ManagerDatabase : public ManagerData {
	public:
		explicit ManagerDatabase(std::unique_ptr<const database::MySQLRealConnection>&& conn);
		~ManagerDatabase();

		virtual void resetManagerSpecificData() const;
		virtual std::list<entity::JobInformation> loadJobInformation() const;
		virtual void substractWorkersDone(const std::string& jobId, const std::size_t workers) const;

	private:
		std::unique_ptr<const database::MySQLRealConnection> conn;

		void markAsLoaded(const std::string& jobId) const;
	};
}


#endif