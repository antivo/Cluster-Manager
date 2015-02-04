#ifndef DATABASE_MYSQL_STORE_RESULT
#define DATABASE_MYSQL_STORE_RESULT

#include <windows.h>

#include <string>

#include "_dependencies\mysql\include\mysql.h"

namespace database {
  /// RAII, represents result retrieved from database, enables traversing result
  class MySQLStoreResult {
  public:
    /// <exception cref="std::runtime_error">Thrown when cannot obtain results from the last querry.</exception>
    explicit MySQLStoreResult(st_mysql* const connection);
    ~MySQLStoreResult();

		/// _NOEXCEPT
		bool mysqlFetchRow(); 
		/// _NOEXCEPT
		bool setAtNextAttribute(); 
		/// _NOEXCEPT
		std::string getCurrentAttribute(); 
  
	private:
    st_mysql_res* result;
    std::size_t numFields;
    std::size_t attributeIndex;
    MYSQL_ROW row;

		MySQLStoreResult(const MySQLStoreResult&) = delete;
		MySQLStoreResult& operator=(const MySQLStoreResult&) = delete;
  };
}
#endif