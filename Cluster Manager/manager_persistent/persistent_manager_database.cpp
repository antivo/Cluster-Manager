#include "persistent_manager_database.h"

#include <vector>
#include <sstream>

#include "entity_job_information.h"
#include "database_mysql_real_connection.h"
#include "database_mysql_store_result.h"

namespace persistent {
	ManagerDatabase::ManagerDatabase(std::unique_ptr<const database::MySQLRealConnection>&& conn) : conn(std::move(conn)) {}
	ManagerDatabase::~ManagerDatabase() {}
	
	void ManagerDatabase::resetManagerSpecificData() const {
		std::stringstream statement;
		statement << "UPDATE `jobs` SET `loaded`='0'";
		conn->querry(statement.str().c_str());
	}
				
	std::list<entity::JobInformation> ManagerDatabase::loadJobInformation() const {		
		auto result = conn->select("SELECT `idClient`, `name`, `id`, `jobType`, `neededWorkers`, `args`, `path` FROM `jobs` WHERE `loaded`='0'");
		std::list<entity::JobInformation> jobInformationList;
		while (result->mysqlFetchRow()) {
			std::vector<std::string> stringVector;
			do {
				stringVector.push_back(result->getCurrentAttribute());
			} while (result->setAtNextAttribute());
			jobInformationList.emplace_back(std::move(stringVector));
		}
		for (const auto& jobInformation : jobInformationList) {
			markAsLoaded(jobInformation.getJobID());
		}
		return jobInformationList;
	}
				
	void ManagerDatabase::substractWorkersDone(const std::string & jobId, const std::size_t workers) const {
		std::stringstream statement;
		statement << "UPDATE `clusterdb`.`jobs` SET `neededWorkers`= (`neededWorkers` - " << workers << ") WHERE `id`=" << jobId;
		conn->querry(statement.str().c_str());
	}

	void ManagerDatabase::markAsLoaded(const std::string & jobId) const {
		std::stringstream statement;
		statement << "UPDATE `jobs` SET `loaded`='1' WHERE `id`=" << jobId;
		conn->querry(statement.str().c_str());
	}
}