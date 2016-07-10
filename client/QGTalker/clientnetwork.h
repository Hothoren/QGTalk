#ifndef CLIENTNETWORK_H
#define CLIENTNETWORK_H

#include <network.h>
#include <QString>
class ClientNetWork : public NetWork
{
    Q_OBJECT
public:
    ClientNetWork(QObject *parent = 0);
    int login_server();
    int reg_server();
    void set_user_id(QString id);
    void set_user_password(QString pw);
    void deal_read();
    void deal_connected();

    void set_userId(QString userId);
    void set_passWord(QString passWord);
    void set_port(QString port);
    void set_name(QString name);
signals:
     void login_res(char res) ;
     void show_chat(QString data);
     void deshow_chat(QString data);

private:
    QString _userId;
    QString _passWord;
    QString _port;
    QString _name;
    int _ifLogin;
};

#endif // CLIENTNETWORK_H
