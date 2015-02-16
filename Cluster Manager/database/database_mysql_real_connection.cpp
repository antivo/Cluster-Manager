#include "database_mysql_real_connection.h"

#include "assert_common.h"
#include "database_mysql_store_result.h"

#pragma comment(lib, R"(_dependencies\mysql\lib\libmysql.lib)")

namespace database {

	MySQLRealConnection::MySQLRealConnection(const char* hostName, const char* userName, const char* password, const char* dbName, const unsigned long portNum) : connection{ nullptr, nullptr } {
    struct st_mysql* pMySQL{ mysql_init(nullptr) };
		assert::runtime(nullptr != pMySQL, "Could not allocate and initialize MYSQL object in  MySQLRealConnection::MySQLRealConnection");
    auto deleter = [](st_mysql* pointer){};
    connection = std::unique_ptr<st_mysql, void(*)(st_mysql*)>(pMySQL, deleter);
    auto result = mysql_real_connect(connection.get(), hostName, userName, password, dbName, portNum, nullptr, 0);
		assert::runtime(nullptr != result, "Could not allocate connect to MySQL database");
  }

  MySQLRealConnection::~MySQLRealConnection() {}

  void MySQLRealConnection::querry(const char* statement) const{
    auto result = mysql_query(connection.get(), statement);
		assert::runtime(0 == result, "Could not execute querry in MySQLRealConnection::querry");
  }

  std::unique_ptr<MySQLStoreResult> MySQLRealConnection::select(const char* statement) const {
    querry(statement);
    return std::make_unique<MySQLStoreResult>(connection.get());
  }

	std::string MySQLRealConnection::selectUnit(const std::string & statement, const std::function<std::string(void)>& generateExceptionReport) const {
		auto result = select(statement.c_str());
		assert::runtime(result->mysqlFetchRow(), generateExceptionReport());
		return result->getCurrentAttribute();
	}

}