/*
* MySQLManager.h

*/

#ifndef MYSQLMANAGER_H_
#define MYSQLMANAGER_H_

//#include "../Common/CheckStringTools.h"

#include <mysql/mysql.h>

#include <iostream>
#include <string>
#include <vector>

#include <string.h>
typedef std::vector<std::string> VCTSTRING;
using namespace std;

class MySQLManager {
public:
  /*
   * Init MySQL
   * @param hosts:         Host IP address
   * @param userName:        Login UserName
   * @param password:        Login Password
   * @param dbName:        Database Name
   * @param port:                Host listen port number
   */
  MySQLManager(std::string hosts, std::string userName, std::string password,
               std::string dbName, unsigned int port);
  ~MySQLManager();
  void initConnection();
  /*
   * Making query from database
   * @param mysql:        MySQL Object
   * @param sql:                Running SQL command
   */
  bool runSQLCommand(std::string sql);
  /**
   * Destroy MySQL object
   * @param mysql                MySQL object
   */
  void destroyConnection();
  bool getConnectionStatus();
  vector<VCTSTRING> &getResult();

protected:
  void setUserName(std::string userName);
  void setHosts(std::string hosts);
  void setPassword(std::string password);
  void setDBName(std::string dbName);
  void setPort(unsigned int port);

private:
  bool IsConnected;
  vector<VCTSTRING> resultList;
  MYSQL mySQLClient;
  unsigned int DEFAULTPORT;
  char *HOSTS;
  char *USERNAME;
  char *PASSWORD;
  char *DBNAME;
};

#endif /* MYSQLMANAGER_H_ */
