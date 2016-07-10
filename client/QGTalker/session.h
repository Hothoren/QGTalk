#ifndef SESSION_H
#define SESSION_H

#include <QWidget>
#include <QTabWidget>
#include <chatnetwork.h>
namespace Ui {
class session;
}

class session : public QWidget
{
    Q_OBJECT
public:
    explicit session(QWidget *parent = 0);
    ~session();
    int add_tab(QString name, ChatNetWork *chat);
    int add_chat(ChatNetWork *chat);
    void close_tab();
    void send_message();
    int chat_in_tab(ChatNetWork *chat);
private:
    //Ui::session *ui;
    QTabWidget *_tabWidget ;
    QVector<ChatNetWork *>* _chatVct;
};

#endif // SESSION_H
