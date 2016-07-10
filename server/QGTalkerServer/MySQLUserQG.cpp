
#include "MySQLUserQG.h"
#include <vector>
int MySQLUserQG::user_login(std::string id, std::string password,
                            std::string &name) {
  std::string sql("select * from user where id = ");
  sql += id;
  if (runSQLCommand(sql) == false) {
    std::cout << "false" << std::endl;
    return -1; // sql语句执行错误
  }
  std::cout << "sucessful select" << std::endl;
  if (getResult().empty())
    return -2; //结果错误
  std::vector<std::string> temp = getResult().back();
  getResult().pop_back();
  if (temp.size() != 3)
    return -2; //结果错误
  if (temp[2] != password)
    return -3; //密码错误
  name = temp[1];
  return 0; //成功
}
int MySQLUserQG::user_reg(std::string name, std::string password,
                          std::string &id) {
  std::string sql("insert into user(name,password) value('");
  sql += name + "','" + password + "');";
  std::cout << sql << std::endl;
  if (runSQLCommand(sql) == false) {
    std::cout << "false" << std::endl;
    return -1; // sql语句执行错误
  }
  std::cout << "sucessful insert" << std::endl;
  sql = "select max(id) from user";
  if (runSQLCommand(sql) == false) {
    std::cout << "false" << std::endl;
    return -1; // sql语句执行错误
  }
  std::cout << "sucessful select" << std::endl;
  if (getResult().empty())
    return -2; //结果错误
  std::vector<std::string> temp = getResult().back();
  getResult().pop_back();
  if (temp.size() != 1)
    return -2; //结果错误
  id = temp[0];
  return 0; //成功
}
// int main(int argc, char const *argv[]) {
//   MySQLManager aa("127.0.0.1", "root", "12398756", "QGTalker", 3306);
//   aa.initConnection();
//   if (aa.runSQLCommand("select * from user;") == false) {
//     std::cout << "false" << std::endl;
//     return 1;
//   } else
//     std::cout << "sucessful" << std::endl;
//   aa.getResult().back();
//   return 0;
// }
