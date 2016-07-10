#include "session.h"
#include "ui_session.h"
#include <QTabWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <QDebug>
#include <QSpacerItem>
#include <QVector>
#include <QMap>
#include "network.h"
#include "clientnetwork.h"
session::session(QWidget *parent) :
    QWidget(parent)
    //ui(new Ui::session)
{
    //ui->setupUi(this);
    _chatVct = new QVector<ChatNetWork *>;
    this->resize(1000, 800);
    this->setWindowTitle("聊天窗口");
    _tabWidget = new QTabWidget();

    QHBoxLayout *layout = new QHBoxLayout();

    layout->addWidget(_tabWidget);

    this->setLayout(layout);
    //新建第一个页面的部件


}
//int session::add_chat(ChatNetWork *chat)
//{
//    _chatVct->push_back(chat);
//    return 0;
//}
int session::chat_in_tab(ChatNetWork *chat)
{
    for(QVector<ChatNetWork *>::iterator it = _chatVct->begin();it != _chatVct->end();it++)
    {
        if((*it) == chat)
        {
            qDebug()<<"chat in tab";
            return -1;
        }
    }
    return 0;
}

int session::add_tab(QString name,ChatNetWork *chat)
{
    if(chat_in_tab(chat) == -1)
        return -1;
    QWidget *widget = new QWidget(this);
    QVBoxLayout *vLayout = new QVBoxLayout(widget);
    QTextBrowser *textBrowser = new QTextBrowser(widget);
    QLineEdit *lineEdit = new QLineEdit(widget);
    vLayout->addWidget(textBrowser);
    vLayout->addWidget(lineEdit);
    QPushButton *sendButton = new QPushButton("send");
    QObject::connect(sendButton, &QPushButton::clicked,
                         this,    &session::send_message);
    QPushButton *closeButton = new QPushButton("close");
    QObject::connect(closeButton, &QPushButton::clicked,
                         this,    &session::close_tab);
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QWidget *buttonWidget = new QWidget();
    buttonLayout->addWidget(sendButton);
    buttonLayout->addWidget(closeButton);
    buttonWidget->setLayout(buttonLayout);
    vLayout->addWidget(buttonWidget);
    widget->setLayout(vLayout);

    _tabWidget->addTab(widget, name);
    _chatVct->push_back(chat);
    for(QMap<quint32,QString>::iterator it = chat->_messageMap.begin();it != chat->_messageMap.end();it++)
    {
        qDebug() << "benlaidexinxi";
        textBrowser->insertPlainText(*it+"\n");
    }
    qDebug() << "con display" << chat->_host << chat->_port;
    connect(chat,&ChatNetWork::display_message,textBrowser,&QTextBrowser::insertPlainText);
    return 0;
}
void session::send_message()
{
    QLineEdit *temp = (QLineEdit *)_tabWidget->currentWidget()->layout()->itemAt(1)->widget();
    (*_chatVct)[_tabWidget->currentIndex()]->send_message(temp->displayText());
    QTextBrowser *tb = (QTextBrowser *)_tabWidget->currentWidget()->layout()->itemAt(0)->widget();
    tb->insertPlainText("\t\t\t\t\t");
    tb->insertPlainText(temp->displayText() + "\n");
    temp->clear();
}

void session::close_tab()
{
    disconnect((*_chatVct)[_tabWidget->currentIndex()],0,0,0);
    _chatVct->remove(_tabWidget->currentIndex());
    _tabWidget->removeTab(_tabWidget->currentIndex());
}

session::~session()
{
    //delete ui;
    delete _tabWidget;
    delete _chatVct;
}
