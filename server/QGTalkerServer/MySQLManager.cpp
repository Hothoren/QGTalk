/*
* MySQLManager.cpp

*/
#include "MySQLManager.h"

MySQLManager::MySQLManager(std::string hosts, std::string userName,
                           std::string password, std::string dbName,
                           unsigned int port) {
  IsConnected = false;
  this->setHosts(hosts);
  this->setUserName(userName);
  this->setPassword(password);
  this->setDBName(dbName);
  this->setPort(port);
}

MySQLManager::~MySQLManager() { this->destroyConnection(); }

void MySQLManager::setDBName(std::string dbName) {
  if (dbName.empty()) { //        用户没有指定数据库名
    std::cout << "DBName is null! Used default value: mysql" << std::endl;
    this->DBNAME = new char[5];
    strcpy(this->DBNAME, "mysql");
  } else {
    this->DBNAME = new char[dbName.length()];
    strcpy(this->DBNAME, dbName.c_str());
  }
}

void MySQLManager::setHosts(std::string hosts) {
  if (hosts.empty()) { //    用户没有指定数据库IP地址
    std::cout << "Hosts is null! Used default value: localhost" << std::endl;
    this->HOSTS = new char[9];
    strcpy(this->HOSTS, "localhost");
  } else {
    this->HOSTS = new char[hosts.length()];
    strcpy(this->HOSTS, hosts.c_str());
  }
}

void MySQLManager::setPassword(std::string password) { //    用户没有指定密码
  if (password.empty()) {
    std::cout << "Password is null! Used default value: " << std::endl;
    this->PASSWORD = new char[1];
    strcpy(this->PASSWORD, "");
  } else {
    this->PASSWORD = new char[password.length()];
    strcpy(this->PASSWORD, password.c_str());
  }
}

void MySQLManager::setPort(
    unsigned int port) { //    用户没有指定端口号，使用默认端口号
  if (!port) {
    std::cout << "Port number is null! Used default value: 0" << std::endl;
    this->DEFAULTPORT = 0;
  } else {
    this->DEFAULTPORT = port;
  }
}

void MySQLManager::setUserName(
    std::string userName) { //    用户没有指定登录用户名
  if (userName.empty()) {
    std::cout << "UserName is null! Used default value: root" << std::endl;
    this->USERNAME = new char[4];
    strcpy(this->USERNAME, "root");
  } else {
    this->USERNAME = new char[userName.length()];
    strcpy(this->USERNAME, userName.c_str());
  }
}

void MySQLManager::initConnection() {
  if (IsConnected) { //    已经连接到服务器
    std::cout << "Is connected to server!" << std::endl;
    return;
  }
  mysql_init(&mySQLClient); //    初始化相关对象
  if (!mysql_real_connect(&mySQLClient, HOSTS, USERNAME, PASSWORD, DBNAME,
                          DEFAULTPORT, NULL, 0)) { //    连接到服务器
    std::cout << "Error connection to database: %s\n"
              << mysql_error(&mySQLClient) << std::endl;
    return;
  }
  IsConnected = true; //    修改连接标识
}

bool MySQLManager::runSQLCommand(std::string sql) {
  if (!IsConnected) { //    没有连接到服务器
    std::cout << "Not connect to database!" << std::endl;
    return false;
  }
  if (sql.empty()) { //    SQL语句为空
    std::cout << "SQL is null!" << std::endl;
    return false;
  }

  MYSQL_RES *res;
  MYSQL_ROW row;

  unsigned int i, j = 0;

  // StringTools stringTools;
  // sql = stringTools.filterString(sql);

  i = mysql_real_query(&mySQLClient, sql.c_str(),
                       (unsigned int)strlen(sql.c_str())); //    执行查询
  if (i) {
    std::cout << "Error query from database: %s\n"
              << mysql_error(&mySQLClient) << std::endl;
    return false;
  }
  res = mysql_store_result(&mySQLClient);
  if (res != NULL) {
    vector<std::string> objectValue;
    while ((row = mysql_fetch_row(res))) { //    遍历结果集
      objectValue.clear();
      for (j = 0; j < mysql_num_fields(res); j++) {
        objectValue.push_back(row[j]);
        std::cout << row[j] << std::endl;
      }
      this->resultList.push_back(objectValue);
    }
    mysql_free_result(res); // free result after you get the result
  }
  return true;
}

vector<VCTSTRING> &MySQLManager::getResult() { return resultList; }

void MySQLManager::destroyConnection() {
  if (!IsConnected) {
    return;
  }
  mysql_close(&mySQLClient);
  this->IsConnected = false;
}

bool MySQLManager::getConnectionStatus() { return IsConnected; }
