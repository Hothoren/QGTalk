#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QVector>
#include "chatnetwork.h"
#include "clientnetwork.h"

class ChatServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = 0);
    ~ChatServer();
    QVector< ChatNetWork *> _chatVct;
    ClientNetWork* _clientNW;
    ChatNetWork * chat_to(QString host,QString port);
protected slots:
    void on_new_connection();
    void on_ready_read();
protected:
private:
};

#endif // CHATSERVER_H
