#include "clientwindow.h"
#include "ui_clientwindow.h"
#include "userbutton.h"
ClientWindow::ClientWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientWindow)
{
    ui->setupUi(this);
    _chatServer = new ChatServer(this);
    connect(_chatServer->_clientNW,&ClientNetWork::login_res,this,&ClientWindow::login_back);
    ss = new session;
    sa = new QScrollArea(this);
    wg = new QWidget(sa);
    lo = new QVBoxLayout(wg);
    wg->setGeometry(80,60,400,12);
    wg->setLayout(lo);
    sa->setWidget(wg);
    sa->setGeometry(70,50,420,400);
    ss->hide();
    sa->hide();

//    ui->scrollArea->setWidget(wg);

}

ClientWindow::~ClientWindow()
{
    delete ui;
}


void ClientWindow::chat(QString user)
{
    QStringList temp = user.split('+');
    temp = temp[2].split(';');
    temp = temp[0].split(':');
    qDebug() << temp[0] << temp[1];
    ChatNetWork* chat =  _chatServer->chat_to(temp[0],temp[1]);
    if(chat != 0)
    {
         ss->add_tab(user,chat);
    }
    ss->show();
}

void ClientWindow::add_chat_user(QString chat)
{
    QPushButton *btt = wg->findChild<QPushButton *>(chat);
    if(btt != NULL )
        return ;
    int x = wg->height();
    wg->setGeometry(80,60,400,x + 72);
    UserButton *bt = new UserButton(chat,wg);
    bt->setObjectName(chat);
    bt->setMinimumHeight(60);
    lo->addWidget(bt);

    connect(bt,&UserButton::signal_button_name,this,&ClientWindow::chat);
}
void ClientWindow::del_chat_user(QString chat)
{
    QPushButton *bt = wg->findChild<QPushButton *>(chat);
    if(bt != NULL )
    {
        int x = wg->height();
        wg->setGeometry(80,60,400,x - 72);
        lo->removeWidget(bt);
        delete bt;
    }
}
void ClientWindow::login_back(char res)
{
    if(res == 'L' || res == 'R')
    {
        ui->id->hide();
        ui->login->hide();
        ui->password->hide();
        ui->reg->hide();
        disconnect(_chatServer->_clientNW,&ClientNetWork::login_res,this,&ClientWindow::login_back);

        connect(_chatServer->_clientNW,&ClientNetWork::show_chat,
                this,&ClientWindow::add_chat_user);
        connect(_chatServer->_clientNW,&ClientNetWork::deshow_chat,
                this,&ClientWindow::del_chat_user);
        sa->show();
    }
}

void ClientWindow::on_login_clicked()
{
    qDebug() <<ui->id->displayText()<<ui->password->displayText();
    _chatServer->_clientNW->set_userId(ui->id->displayText());
    _chatServer->_clientNW->set_passWord(ui->password->displayText());
//    _chatServer->_clientNW->
    if(_chatServer->_clientNW->login_server() == -1)
        qDebug() << "login fail";
}
