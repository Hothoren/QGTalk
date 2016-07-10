#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpSocket>
#include <QString>

class NetWork : public QObject
{
    Q_OBJECT
public:
    explicit NetWork(QObject *parent = 0);
    explicit NetWork(QTcpSocket* sock ,QObject *parent = 0);
    ~NetWork();
    void connect_to(QString host,int port);
    virtual void deal_read(){}
    virtual void deal_connected(){}

    int _ifConnected;

    QTcpSocket* _sock;
public slots:
    void on_connected();
    void on_ready_read();
signals:

public slots:
};

#endif // NETWORK_H
