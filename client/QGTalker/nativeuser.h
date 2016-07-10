#ifndef NATIVEUSER_H
#define NATIVEUSER_H

#include <QObject>
#include<QString>
#include<sys/socket.h>
class NativeUser : public QObject
{
    Q_OBJECT
public:
    explicit NativeUser(QObject *parent = 0);
    int login(QString id,QString password);
private:
    int start_server();
    void server();
    int _serverSock;

signals:

public slots:
};

#endif // NATIVEUSER_H
