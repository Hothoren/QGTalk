
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MySQLUserQG.h"
#include <arpa/inet.h>
#include <map>
#include <netinet/in.h>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define IPADDRESS "192.168.99.14"
#define PORT 11866
#define MAXSIZE 1024
#define LISTENQ 5
#define FDSIZE 1000
#define EPOLLEVENTS 100
typedef struct User {
  std::string id;
  std::string name;
  std::string address;
  int login;
  int sock;
  std::string port;
  std::string buf;
} User;

std::map<int, User *> socketMap;
//函数声明
//创建套接字并进行绑定
static int socket_bind(const char *ip, int port);
// IO多路复用epoll
static void do_epoll(int listenfd);
//事件处理函数
static void handle_events(int epollfd, struct epoll_event *events, int num,
                          int listenfd, char *buf);
//处理接收到的连接
static void handle_accpet(int epollfd, int listenfd);
//读处理
static void do_read(int epollfd, int fd, char *buf);
//写处理
static void do_write(int epollfd, int fd, char *buf);
//错误处理
static void do_error(int epollfd, int fd);
//添加事件
static void add_event(int epollfd, int fd, int state);
//修改事件
static void modify_event(int epollfd, int fd, int state);
//删除事件
static void delete_event(int epollfd, int fd);

static void read_login(int epollfd, int fd, char *buf);

static void read_reg(int epollfd, int fd, char *buf);

static void write_login(int epollfd, int fd, int loginType);

static void write_user(int epollfd, int fd, int actionType);

static int read_unit(int epollfd, int fd, char *buf, std::string &unit);
MySQLUserQG *mysqlUser;
int main(int argc, char *argv[]) {
  mysqlUser = new MySQLUserQG;
  int listenfd;
  listenfd = socket_bind(IPADDRESS, PORT);
  listen(listenfd, LISTENQ);
  do_epoll(listenfd);
  return 0;
}

static int socket_bind(const char *ip, int port) {
  int listenfd;
  struct sockaddr_in servaddr;
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd == -1) {
    perror("socket error:");
    exit(1);
  }
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  // inet_pton(AF_INET, ip, &servaddr.sin_addr);
  servaddr.sin_port = htons(port);
  // servaddr.sin_addr = 0;
  if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
    perror("bind error: ");
    exit(1);
  }
  return listenfd;
}

static void do_epoll(int listenfd) {
  int epollfd;
  struct epoll_event events[EPOLLEVENTS];
  int ret;
  char buf[MAXSIZE];
  memset(buf, 0, MAXSIZE);
  //创建一个描述符
  epollfd = epoll_create(FDSIZE);
  //添加监听描述符事件
  add_event(epollfd, listenfd, EPOLLIN);
  for (;;) {
    //获取已经准备好的描述符事件
    ret = epoll_wait(epollfd, events, EPOLLEVENTS, -1);
    handle_events(epollfd, events, ret, listenfd, buf);
  }
  close(epollfd);
}

static void handle_events(int epollfd, struct epoll_event *events, int num,
                          int listenfd, char *buf) {
  int i;
  int fd;
  //进行选好遍历
  for (i = 0; i < num; i++) {
    fd = events[i].data.fd;
    //根据描述符的类型和事件类型进行处理
    if ((fd == listenfd) && (events[i].events & EPOLLIN))
      handle_accpet(epollfd, listenfd);
    else if (events[i].events & EPOLLIN)
      do_read(epollfd, fd, buf);
    else if (events[i].events & EPOLLOUT)
      do_write(epollfd, fd, buf);
    else if (events[i].events & EPOLLERR)
      do_error(epollfd, fd);
  }
}
static void handle_accpet(int epollfd, int listenfd) {
  int clifd;
  struct sockaddr_in cliaddr;
  socklen_t cliaddrlen = sizeof(cliaddr);
  clifd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddrlen);
  if (clifd == -1) {
    perror("accpet error:");
    printf("%d\n", errno);
  } else {
    printf("accept a new client: %s:%d\n", inet_ntoa(cliaddr.sin_addr),
           cliaddr.sin_port);
    //添加一个客户描述符和事件
    add_event(epollfd, clifd, EPOLLIN);
    std::string sIP(inet_ntoa(cliaddr.sin_addr));
    User *tempUser = new User;
    tempUser->login = 0;
    tempUser->sock = clifd;
    tempUser->address = sIP;
    // tempUser->port = cliaddr.sin_port;
    socketMap[clifd] = tempUser;
  }
}

static void do_read(int epollfd, int fd, char *buf) {
  int nread;
  nread = read(fd, buf, 1);
  if (nread == -1) {
    perror("read error:");
    do_error(epollfd, fd);
  } else if (nread == 0) {
    fprintf(stderr, "client close.\n");
    do_error(epollfd, fd);
  } else {
    if (buf[0] == '$')
      read_reg(epollfd, fd, buf + 1);
    else if (buf[0] == '#')
      read_login(epollfd, fd, buf + 1);
    else
      do_error(epollfd, fd);
  }
}
static int read_unit(int epollfd, int fd, char *buf, std::string &unit) {
  int len;
  int nread;
  unit.clear();
  nread = read(fd, buf, 1);
  if (nread <= 0) {
    perror("read error:");
    do_error(epollfd, fd);
    return -1;
  } else {
    len = (int)buf[0];
    nread = read(fd, buf, len);
    if (nread <= 0) {
      perror("read error:");
      do_error(epollfd, fd);
      return -1;
    } else {
      buf[nread] = 0;
      unit = buf;
    }
  }
  return 0;
}
static void read_login(int epollfd, int fd, char *buf) {
  std::string id;
  if (read_unit(epollfd, fd, buf, id))
    return;
  std::string pw;
  if (read_unit(epollfd, fd, buf, pw))
    return;
  std::string pt;
  if (read_unit(epollfd, fd, buf, pt))
    return;

  //登陆验证
  std::string nm;
  // printf("login=>%d\n", mysqlUser->user_login(id, pw, nm));
  for (std::map<int, User *>::iterator it = socketMap.begin();
       it != socketMap.end(); it++) {
    if (it->second->id == id) {
      printf("id had logined\n");
      return;
    }
  }
  if (mysqlUser->user_login(id, pw, nm) != 0) {
    printf("login fail\n");
    write_login(epollfd, fd, 2);
    return;
  }
  socketMap[fd]->id = id;
  socketMap[fd]->name = nm;
  socketMap[fd]->port = pt;
  socketMap[fd]->login = 1;
  printf("%s-%s-%s-%s\n", id.c_str(), pw.c_str(), pt.c_str(), nm.c_str());

  write_login(epollfd, fd, 0);
  write_user(epollfd, fd, 0);
}

static void read_reg(int epollfd, int fd, char *buf) {
  std::string nm;
  if (read_unit(epollfd, fd, buf, nm))
    return;
  std::string pw;
  if (read_unit(epollfd, fd, buf, pw))
    return;
  std::string pt;
  if (read_unit(epollfd, fd, buf, pt))
    return;
  std::string id;
  if (mysqlUser->user_reg(nm, pw, id) != 0) {
    printf("reg fail\n");
    write_login(epollfd, fd, 3);
    return;
  }
  socketMap[fd]->id = id;
  socketMap[fd]->name = nm;
  socketMap[fd]->port = pt;
  socketMap[fd]->login = 1;
  printf("%s-%s-%s-%s\n", id.c_str(), pw.c_str(), pt.c_str(), nm.c_str());

  write_login(epollfd, fd, 1);
  write_user(epollfd, fd, 0);
}

static void do_write(int epollfd, int fd, char *buf) {
  int nwrite;
  if (socketMap[fd]->buf.empty()) {
    modify_event(epollfd, fd, EPOLLIN);
    return;
  }
  nwrite = write(fd, socketMap[fd]->buf.c_str(), socketMap[fd]->buf.size());
  if (nwrite == -1) {
    perror("write error:");
    do_error(epollfd, fd);
  } else
    modify_event(epollfd, fd, EPOLLIN);
  socketMap[fd]->buf.clear();
  memset(buf, 0, MAXSIZE);
}

static void write_user(int epollfd, int fd,
                       int actionType) { // 0:登陆成功1:退出
  std::string allUser, nowUser, allTemp;
  uint8_t count = 0;
  nowUser += socketMap[fd]->id + "+" + socketMap[fd]->name + "+" +
             socketMap[fd]->address + ":" + socketMap[fd]->port + ";";
  for (std::map<int, User *>::iterator it = socketMap.begin();
       it != socketMap.end(); it++) {
    if (fd != it->first && it->second->login == 1) {
      socketMap[it->first]->buf += 'A' + actionType;
      socketMap[it->first]->buf += (char)1;
      socketMap[it->first]->buf += (char)(nowUser.length()) + nowUser;
      modify_event(epollfd, it->first, EPOLLOUT);
      allTemp = it->second->id + "+" + it->second->name + "+" +
                it->second->address + ":" + it->second->port + ";";
      allUser += (char)(allTemp.length()) + allTemp;
      ++count;
    }
  }
  if (count != 0 && actionType == 0) {
    socketMap[fd]->buf += 'A';
    socketMap[fd]->buf += (char)count;
    socketMap[fd]->buf += allUser;
    modify_event(epollfd, fd, EPOLLOUT);
  }
}
static void write_login(int epollfd, int fd, int loginType) {
  switch (loginType) {
  case 0:
    socketMap[fd]->buf += 'L';
    break;
  case 1:
    socketMap[fd]->buf += 'R';
    break;
  case 2:
    socketMap[fd]->buf += 'N';
    break;
  case 3:
    socketMap[fd]->buf += 'Q';
    break;
  }
  modify_event(epollfd, fd, EPOLLOUT);
}
static void do_error(int epollfd, int fd) {
  write_user(epollfd, fd, 1);
  close(fd);
  delete_event(epollfd, fd);
  delete socketMap[fd];
  socketMap.erase(fd);
}

static void add_event(int epollfd, int fd, int state) {
  struct epoll_event ev;
  ev.events = state;
  ev.data.fd = fd;
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

static void delete_event(int epollfd, int fd) {
  struct epoll_event ev;
  ev.data.fd = fd;
  epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
}

static void modify_event(int epollfd, int fd, int state) {
  struct epoll_event ev;
  ev.events = state;
  ev.data.fd = fd;
  epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}
