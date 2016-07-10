#include "chatserver.h"
#include "chatnetwork.h"
ChatServer::ChatServer(QObject *parent) : QTcpServer(parent)
{
//    _serverSock = new QTcpServer(parent);
    connect(this,  &QTcpServer::newConnection,
            this, &ChatServer::on_new_connection);
    _clientNW = new ClientNetWork(this);
    if(listen() == 0)
        qDebug() << "listen fail";
    else
        qDebug() << "jianting " << serverAddress() <<  serverPort();
    _clientNW->set_port(QString::number(serverPort() , 10));
    _clientNW->connect_to("127.0.0.1",11866);
}
ChatNetWork * ChatServer::chat_to(QString host,QString port)
{
    qDebug() << "changshilianjie " << host << port;
    for(QVector<ChatNetWork *>::iterator it = _chatVct.begin();it != _chatVct.end();it++)
    {
        qDebug() << (*it)->_host  << (*it)->_port;

        if((*it)->_host == host )
            return (*it);
    }
    ChatNetWork *chat = new ChatNetWork(this);
    _chatVct.push_back(chat);
    chat->_host = host;
    chat->_port = port;
    chat->connect_to(host,port.toInt());
    return chat;
}

ChatServer::~ChatServer()
{
    delete _clientNW;
}

void ChatServer::on_new_connection()
{
    while(hasPendingConnections())
    {
        QTcpSocket *sock = nextPendingConnection();
        qDebug() << "new connection";
        ChatNetWork *chat = new ChatNetWork(sock,this);
        QStringList temp = sock->localAddress().toString().split("f:");
        chat->_host = temp[1];
        chat->_port = QString::number(sock->localPort());
        qDebug()<< chat->_host<<chat->_port;
        _chatVct.push_back(chat);
        connect(sock, &QTcpSocket::readyRead,
                chat, &ChatNetWork::on_ready_read);
//        connect(sock, &QTcpSocket::disconnected,
//                this, &ChatNetWork::delete_chat);
    }
}
//ChatNetWork::delete_chat()
void ChatServer::on_ready_read()
{
    QTcpSocket *sock = qobject_cast<QTcpSocket*>(sender());
//    echo(sock);
}
