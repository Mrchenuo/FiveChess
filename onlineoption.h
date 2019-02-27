#ifndef ONLINEOPTION_H
#define ONLINEOPTION_H

#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

#include "networkmodule.h"

class OnlineOption : public QWidget
{
    Q_OBJECT
public:
    explicit OnlineOption(QWidget *parent = 0);
    ~OnlineOption();
signals:
    void InAppendLog(QString log);

    //此3个信号会被NetworkModule里的槽函数所接收
    //服务器开始监听
    void InServerListenning(void);
    //服务器断开所有连接，取消监听
    void InCancelListenning(void);
    //客户端尝试连接
    void InClientConnect(QString);

public slots:

    //点击”创建“按钮后的槽函数
    void OnCreateOnlineGame(void);
    //点击”连接“按钮后的槽函数
    void OnConnectOnlineGame(void);
    //点击”取消创建“按钮后的槽函数
    void OnCancelCreateOnlineGame(void);
    //向输出窗口输出数据
    void OnAppendLog(QString Log);
    //当游戏开始后，对此窗口的一些操作
    void OnHideWidget(void);

private:
    QPushButton * ServerButton;
    QPushButton * ClientButton;

    QLabel * ServerIP;
    QLineEdit * ClientConnectIP;
    QTextBrowser * LogOutput;

    QVBoxLayout * ServerVBox;
    QVBoxLayout * ClientVBox;
    QVBoxLayout * GroupVBox;

    QHBoxLayout * WidgetHBox;

    QGroupBox * ServerGroup;
    QGroupBox * ClientGroup;
};

#endif // ONLINEOPTION_H
