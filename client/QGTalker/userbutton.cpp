#include "userbutton.h"

UserButton::UserButton(QWidget *parent) : QPushButton(parent)
{
    connect(this,&QPushButton::clicked,this, &UserButton::send_button_name);
}
UserButton::UserButton(QString name,QWidget *parent) : QPushButton(name,parent)
{
    connect(this,&QPushButton::clicked,this, &UserButton::send_button_name);
}
void UserButton::send_button_name()
{
    emit signal_button_name(objectName());
}
