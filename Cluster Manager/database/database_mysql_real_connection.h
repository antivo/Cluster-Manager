#ifndef DATABASE_MYSQL_REAL_CONNECTION
#define DATABASE_MYSQL_REAL_CONNECTION

#include <windows.h>

#include <functional>
#include <memory>

#include "_dependencies/mysql/include/mysql.h"

namespace database {
  class MySQLStoreResult;

  /// RAII, represent connection to MySQL database, encapsulate mysql_real_connect
  class MySQLRealConnection {
  public:
    /// <exception cref="std::runtime_error">Thrown when fails create MYSQL object.</exception>
    /// <exception cref="std::runtime_error">Thrown when fails to connect to MySQL database.</exception>
    MySQLRealConnection(const char* hostName, const char* userName, const char* password, const char* dbName, const unsigned long portNum);
    ~MySQLRealConnection();

    /// for querries that do not retrieve data (update, delete, insert)
    /// <exception cref="std::runtime_error">Thrown when fails to execute statement.</exception>
    void querry(const char* statement) const;

    ///for queries that retrieve data (select)
    /// <exception cref="std::runtime_error">Thrown when fails to execute statement.</exception>
    std::unique_ptr<MySQLStoreResult> select(const char* statement) const;
 
		std::string selectUnit(const std::string& statement, const std::function<std::string(void)>& generateExceptionReport) const;

	private:
    std::unique_ptr<st_mysql, void(*)(st_mysql*)> connection; 

    MySQLRealConnection(const MySQLRealConnection&) = delete;
    MySQLRealConnection& operator=(const MySQLRealConnection&) = delete;
  };
}

#endif