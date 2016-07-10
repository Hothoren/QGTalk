#include "clientnetwork.h"

ClientNetWork::ClientNetWork(QObject *parent):NetWork(parent),_ifLogin(0)
{

}
void ClientNetWork::set_user_id(QString id)
{
    _userId = id;
}
void ClientNetWork::set_user_password(QString pw)
{
    _passWord = pw;
}

int ClientNetWork::login_server()
{
    if(_ifConnected == 0) return -1;
    char len;
    _sock->write("#");

    len = (char) _userId.size();
    _sock->write(&len,1);
    _sock->write(_userId.toStdString().c_str());

    len = (char) _passWord.size();
    _sock->write(&len,1);
    _sock->write(_passWord.toStdString().c_str());

    len = (char) _port.size();
    _sock->write(&len,1);
    _sock->write(_port.toStdString().c_str());
    return 0;
}
int ClientNetWork::reg_server()
{
    if(_ifConnected == 0) return -1;
    char len;
    _sock->write("$");

    len = (char) _name.size();
    _sock->write(&len,1);
    _sock->write(_name.toStdString().c_str());

    len = (char) _passWord.size();
    _sock->write(&len,1);
    _sock->write(_passWord.toStdString().c_str());

    len = (char) _port.size();
    _sock->write(&len,1);
    _sock->write(_port.toStdString().c_str());
    return 0;
}
void ClientNetWork::deal_read()
{
    char sign;
    quint8 len;
    quint8 times;
    QByteArray buf;
    while(!_sock->atEnd())
    {
        _sock->read(&sign,1);
        switch (sign)
        {
        case 'L':
            emit login_res('L');
            break;
        case 'R':
            emit login_res('R');
            break;
        case 'N':
            emit login_res('N');
            break;
        case 'A':
        case 'B':
            _sock->read((char*)&times,1);
            for(int i = 0; i < times; i++)
            {
                _sock->read((char*)&len,1);
                buf = _sock->read(len);
                QString temp(buf);
                qDebug() << temp;
                if(sign == 'A')
                    emit show_chat(temp);
                else
                    emit deshow_chat(temp);
            }
            break;
        default:
            break;
        }
    }
}
void ClientNetWork::set_userId(QString userId)
{
    _userId = userId;
}


void ClientNetWork::set_passWord(QString passWord)
{
    _passWord = passWord;
}

void ClientNetWork::set_port(QString port)
{
    _port = port;
}

void ClientNetWork::set_name(QString name)
{
    _name = name;
}

void ClientNetWork::deal_connected()
{

}
