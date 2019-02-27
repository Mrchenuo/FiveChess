#include "onlineoption.h"

OnlineOption::OnlineOption(QWidget *parent) : QWidget(parent)
{
    //设置为一个窗口
    this->setWindowFlags(Qt::Window);
    //设置窗口大小
    this->resize(440, 240);
    //设置模态窗口
    this->setWindowModality(Qt::ApplicationModal);
    //设置关闭窗口后自动删除
    //this->setAttribute(Qt::WA_DeleteOnClose);
    //设置禁止最大化按钮
    this->setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    //设置禁止拉伸窗口
    this->setFixedSize(width(), height());

    ServerButton = new QPushButton(tr("创建"));
    ClientButton = new QPushButton(tr("加入"));

    ServerIP = new QLabel(QString(tr("您当前的内网IP") + QString("\n%1").arg(NetworkModule::InsideIPAddress())));
    ClientConnectIP = new QLineEdit();
    LogOutput = new QTextBrowser(this);

    //更改输入框灰色的占位符信息
    ClientConnectIP->setPlaceholderText(tr("请输入对方的内网IP"));

    ServerVBox = new QVBoxLayout();
    ClientVBox = new QVBoxLayout();
    GroupVBox = new QVBoxLayout();

    WidgetHBox = new QHBoxLayout();

    ServerGroup = new QGroupBox(tr("创建对局"));
    ClientGroup = new QGroupBox(tr("加入对局"));

    QFont Font;
    Font.setFamily("Arial");
    Font.setBold(true);
    Font.setPixelSize(20);

    ServerButton->setFont(Font);
    ClientButton->setFont(Font);
    ServerIP->setFont(Font);

    ServerIP->setAlignment(Qt::AlignHCenter);

    ServerVBox->addWidget(ServerIP);
    ServerVBox->addWidget(ServerButton);

    ClientVBox->addWidget(ClientConnectIP);
    ClientVBox->addWidget(ClientButton);

    ServerGroup->setLayout(ServerVBox);
    ClientGroup->setLayout(ClientVBox);

    GroupVBox->addWidget(ServerGroup);
    GroupVBox->addWidget(ClientGroup);
    WidgetHBox->addLayout(GroupVBox);
    WidgetHBox->addWidget(LogOutput);

    setLayout(WidgetHBox);

    Font.setPixelSize(18);
    LogOutput->setFont(Font);

    connect(ServerButton, &QAbstractButton::clicked, this, &OnlineOption::OnCreateOnlineGame);
    connect(ClientButton, &QAbstractButton::clicked, this, &OnlineOption::OnConnectOnlineGame);
    connect(this, &OnlineOption::InAppendLog, this, &OnlineOption::OnAppendLog);

    emit InAppendLog(PLAYERMSG_ENUM + tr("如果您要创建对局，请点击左上的“创建”按钮,并将按钮上方标签显示的IP地址告知于您同内网的朋友（例如，连接同一个WiFi），以便连接您的对局"));
    emit InAppendLog(PLAYERMSG_ENUM + QString("------------------------------------"));
}

OnlineOption::~OnlineOption()
{

}

void OnlineOption::OnCreateOnlineGame()
{
    ServerButton->setText(tr("取消创建"));
    disconnect(ServerButton, &QAbstractButton::clicked, this, &OnlineOption::OnCreateOnlineGame);
    connect(ServerButton, &QAbstractButton::clicked, this, &OnlineOption::OnCancelCreateOnlineGame);

    emit InServerListenning();
}

void OnlineOption::OnConnectOnlineGame()
{
    emit InClientConnect(ClientConnectIP->text());
}

void OnlineOption::OnCancelCreateOnlineGame()
{
    emit InCancelListenning();

    emit InAppendLog(SYSMSG_ENUM + tr("您已取消创建对局！"));

    ServerButton->setText(tr("创建"));
    disconnect(ServerButton, &QAbstractButton::clicked, this, &OnlineOption::OnCancelCreateOnlineGame);
    connect(ServerButton, &QAbstractButton::clicked, this, &OnlineOption::OnCreateOnlineGame);
}

void OnlineOption::OnAppendLog(QString Log)
{
    //将Log切片
    QString EnumString = Log.mid(0, ENUM_SIZE);
    QString TextString = Log.mid(ENUM_SIZE, -1);
    //在这里，PLAYERMSG_ENUM表示的是黑色的字
    //SYSMSG_ENUM表示的是红色的字
    if (EnumString == PLAYERMSG_ENUM)
    {
        LogOutput->append(TextString);
    }
    else if (EnumString == SYSMSG_ENUM)
    {
        QString RedString = "<font color=red>%1</font>";
        RedString = RedString.arg(TextString);
        LogOutput->append(RedString);
    }

    //将TextBrowser输出窗口的滚动条滑到底部
    //LogOutput->verticalScrollBar()->setSliderPosition(LogOutput->verticalScrollBar()->maximum());
    LogOutput->moveCursor(QTextCursor::End);
}

void OnlineOption::OnHideWidget()
{
    if (ServerButton->text() == "取消创建")
    {
        //因为原本是ServerListenning的状态，但在游戏结束后会CancelListenning
        //所有直接重置按钮就好了
        ServerButton->setText(tr("创建"));
        disconnect(ServerButton, &QAbstractButton::clicked, this, &OnlineOption::OnCancelCreateOnlineGame);
        connect(ServerButton, &QAbstractButton::clicked, this, &OnlineOption::OnCreateOnlineGame);
    }
    //并且把TextBrowser里的文字全部清空掉
    LogOutput->clear();
    OnAppendLog(QString(PLAYERMSG_ENUM) + tr("已清空所有消息"));
    hide();
}
