#include "nativeuser.h"

NativeUser::NativeUser(QObject *parent) : QObject(parent)
{

}
int NativeUser::login(QString id, QString password)
{

    return start_server();
}

int NativeUser::start_server()
{

    return 0;
}

void NativeUser::server()
{


}
