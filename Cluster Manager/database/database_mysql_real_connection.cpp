#include "database_mysql_real_connection.h"

#include <stdexcept>

#include "database_mysql_store_result.h"

//#pragma comment(lib, R"(.....\Cluster Manager\database\_dependencies\mysql\lib\libmysql.lib)")
#pragma comment(lib, R"(_dependencies\mysql\lib\libmysql.lib)")


namespace database {

	MySQLRealConnection::MySQLRealConnection(const char* hostName, const char* userName, const char* password, const char* dbName, const unsigned long portNum) : connection{ nullptr, nullptr } {
    struct st_mysql* pMySQL{ mysql_init(nullptr) };
    if(nullptr == pMySQL) {
      throw std::runtime_error("Could not allocate and initialize MYSQL object in  MySQLRealConnection::MySQLRealConnection");
    }
    auto deleter = [](st_mysql* pointer){};
    connection = std::unique_ptr<st_mysql, void(*)(st_mysql*)>(pMySQL, deleter);
    auto result = mysql_real_connect(connection.get(), hostName, userName, password, dbName, portNum, nullptr, 0);
    if(nullptr == result) {
      throw std::runtime_error("Could not allocate connect to MySQL database");
    }
  }

  MySQLRealConnection::~MySQLRealConnection() {}

  void MySQLRealConnection::querry(const char* statement) const{
    auto result = mysql_query(connection.get(), statement);
    if (0 != result) {
      throw std::runtime_error("Could not execute querry in MySQLRealConnection::querry");
    }
  }

  std::unique_ptr<MySQLStoreResult> MySQLRealConnection::retrieve(const char* statement) const {
    querry(statement);
    return std::make_unique<MySQLStoreResult>(connection.get());
  }

	std::string MySQLRealConnection::select(const std::string & statement, const std::function<std::string(void)>& generateExceptionReport) const {
		auto result = retrieve(statement.c_str());
		if (result->mysqlFetchRow()) {
			return result->getCurrentAttribute();
		}
		throw std::runtime_error(generateExceptionReport());
	}

}