#include "chatnetwork.h"
#include <QDateTime>
int ChatNetWork::send_message(QString data)
{
    quint16 len;
    if(_ifConnected)
    {
        len = data.size();
        if(len == 0) return -1;
        qDebug() << "send" << data;
        _sock->write("M");
        _sock->write((char *)&len,2);
        _sock->write(data.toLocal8Bit());
        _sock->flush();
    }
}

QString ChatNetWork::get_name()
{
    return _name;
}

void ChatNetWork::set_name(QString name)
{
    _name = name;
}
//void ChatNetWork::deal_connected()
//{

//}

void ChatNetWork::deal_read()
{
    QDateTime time = QDateTime::currentDateTime();
    quint32 time_stamp = time.toTime_t();
    quint16 len;
    QByteArray buf;
    buf = _sock->read(1);
    if(buf[0] == 'M')
    {
        _sock->read((char*)&len,2);
        buf = _sock->read(len);
        QString temp(buf);
        _messageMap[time_stamp] = temp;
        qDebug() << temp;
        emit display_message(time.toString() + ":");
        emit display_message(temp + "\n");
    }
}
