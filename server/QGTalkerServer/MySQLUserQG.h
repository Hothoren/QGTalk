/**
 *
 */

#ifndef MYSQLUSERQG_H_
#define MYSQLUSERQG_H_
#include "MySQLManager.h"
#include <string>
class MySQLUserQG : public MySQLManager {
public:
  MySQLUserQG()
      : MySQLManager("127.0.0.1", "root", "12398756", "QGTalker", 3306) {
    initConnection();
  }
  ~MySQLUserQG() {}
  int user_login(std::string id, std::string password, std::string &name);
  int user_reg(std::string name, std::string password, std::string &id);
};

#endif
