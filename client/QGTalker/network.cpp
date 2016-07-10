#include "network.h"

NetWork::NetWork(QObject *parent) : QObject(parent),_ifConnected(0)
{
    _sock = new QTcpSocket(this);
    QObject::connect(_sock, &QTcpSocket::connected,
                         this,    &NetWork::on_connected);
    QObject::connect(_sock, &QTcpSocket::readyRead,
                         this,    &NetWork::on_ready_read);
    QObject::connect(_sock, &QTcpSocket::disconnected,
            _sock, &QTcpSocket::deleteLater);

}
NetWork::NetWork(QTcpSocket* sock ,QObject *parent) : QObject(parent),_ifConnected(1)
{
    _sock = sock;
    QObject::connect(_sock, &QTcpSocket::connected,
                         this,    &NetWork::on_connected);
    QObject::connect(_sock, &QTcpSocket::readyRead,
                         this,    &NetWork::on_ready_read);
    QObject::connect(_sock, &QTcpSocket::disconnected,
            _sock, &QTcpSocket::deleteLater);

}


void NetWork::connect_to(QString host,int port)
{
    _sock->connectToHost(host,port);
}

void NetWork::on_connected()
{
    qDebug() << "NetWork::on_connected()";
    _ifConnected = 1;
    this->deal_connected();
}

void NetWork::on_ready_read()
{
//    qDebug() << _sock->readAll();
    qDebug() << "NetWork::on_read()";
    this->deal_read();
}


NetWork::~NetWork()
{

}

