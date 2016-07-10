#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include"session.h"
#include <QScrollArea>
#include <QLayout>
#include "chatserver.h"
#include "chatnetwork.h"
namespace Ui {
class ClientWindow;
}

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientWindow(QWidget *parent = 0);
    ~ClientWindow();
//public slots:
    void add_chat_user(QString chat);
    void del_chat_user(QString chat);
    void chat(QString user);
public slots:
    void on_login_clicked();
    void login_back(char);

private:
    Ui::ClientWindow *ui;
    session *ss;
    QScrollArea *sa;
    QWidget *wg;
    QVBoxLayout *lo;
    ChatServer * _chatServer;
};

#endif // CLIENTWINDOW_H
