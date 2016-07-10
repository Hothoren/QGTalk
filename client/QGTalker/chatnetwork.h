#ifndef CHATNETWORK_H
#define CHATNETWORK_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QMap>
#include "network.h"
class ChatNetWork : public NetWork
{
    Q_OBJECT
public:
    ChatNetWork(QObject *parent = 0):NetWork(parent){}
    ChatNetWork(QTcpSocket* sock,QObject *parent = 0):NetWork(sock,parent){}
    virtual ~ChatNetWork(){}
    int send_message(QString data);
    QString get_name();
    void set_name(QString name);
    void deal_read();
    QString _name;
    QString _host;
    QString _port;
    QMap<quint32,QString> _messageMap;
signals:
    void display_message(const QString &data);
private:
};

#endif // CHATNETWORK_H
