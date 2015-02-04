#include "persistent_manager_database.h"

#include "entity_job_information.h"
#include "database_mysql_real_connection.h"
#include "database_mysql_store_result.h"

namespace persistent {
	ManagerDatabase::ManagerDatabase(std::unique_ptr<const database::MySQLRealConnection>&& conn) : conn(std::move(conn)) {}
	ManagerDatabase::~ManagerDatabase() {}
	
	void ManagerDatabase::resetManagerSpecificData() const {}
				
	std::list<entity::JobInformation> ManagerDatabase::loadJobInformation() const {		
		return std::list<entity::JobInformation>();
	}
				
	void ManagerDatabase::substractWorkersDone(const std::string & jobId, const std::size_t workers) const {}
}