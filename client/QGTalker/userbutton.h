#ifndef USERBUTTON_H
#define USERBUTTON_H
#include <QObject>
#include <QPushButton>
#include <QString>
class UserButton : public QPushButton
{
    Q_OBJECT
public:
    UserButton(QWidget *parent = 0);
    UserButton(QString name,QWidget *parent = 0);
    void send_button_name();
signals:
    void signal_button_name(QString name);
};

#endif // USERBUTTON_H
