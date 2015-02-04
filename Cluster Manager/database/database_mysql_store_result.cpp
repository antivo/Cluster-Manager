#include "database_mysql_store_result.h"

#pragma comment(lib, R"(_dependencies\mysql\lib\libmysql.lib)")

namespace database{

  MySQLStoreResult::MySQLStoreResult(st_mysql* const connection) {
    result = mysql_store_result(connection);
    if(nullptr == result) {
      throw std::runtime_error("Could not retrieve result from previous querry in  MySQLStoreResult::MySQLStoreResult");
    }
    numFields = mysql_num_fields(result);
  }

  MySQLStoreResult::~MySQLStoreResult() {
    mysql_free_result(result);
  }

  bool MySQLStoreResult::mysqlFetchRow() {
    attributeIndex = 0;
    row = mysql_fetch_row(result);
    return (nullptr != row);
  }

  bool MySQLStoreResult::setAtNextAttribute() {
		bool exists{ false };
    if ((attributeIndex + 1) < numFields) {
      ++attributeIndex; 
      exists = true;
    }
    return exists;
  }

  std::string MySQLStoreResult::getCurrentAttribute() {
    return std::string(row[attributeIndex]);
  }

}