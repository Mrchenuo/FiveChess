#ifndef NETWORKMODULE_H
#define NETWORKMODULE_H

//这就是一个网络模块，包括监听，连接，查看内网IP等等的功能

#include <QDebug>

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QByteArray>
#include <QHostAddress>
#include <QNetworkInterface>

#include "const.h"

class NetworkModule : public QObject
{
    Q_OBJECT
public:
    explicit NetworkModule(QObject *parent = 0);
    ~NetworkModule();

    //返回QString类型的内网IP
    static QString InsideIPAddress(void);

    //当前网络状态
    enum NetworkState{NETWORK_NONE, NETWORK_SERVER_LISTEN, NETWORK_SERVER_CONNECT, NETWORK_CLIENT};
    NetworkState NetworkStatus;

    //服务器发送数据到特定套接字
    void ServerMsgSend(QString Msg);
    //客户端发送数据到服务器
    void ClientMsgSend(QString Msg);

signals:
    //此信号会连接OnlineOption里的OnHideWidget()，把OnlineOptionWidget隐藏，并开始游戏
    void InStartGame(void);

    void InAppendLog(QString Log);

    void InFromNetworkMsg(QString Log);

    void InDisconnected(void);
public slots:

    //此3个槽函数接受onlineoption里的信号
    //服务器开始监听
    void OnServerListenning(void);
    //服务器断开所有连接，取消监听
    void OnCancelListenning(void);
    //客户端尝试连接
    void OnClientConnect(QString IP);
private slots:
    //以下的槽函数则会被内部调用

    //当有新连接连接至服务器时
    void OnNewConnectServer(void);

    //当客户端连接成功时
    void OnNewConnectClient(void);

    //客户端从服务器接收数据
    void OnClientMsgReceive(void);

    //服务器接收数据
    void OnServerMsgReceive(void);

    //当客户端与服务器的连接断开
    void OnClientDisconnected(void);

    //有连接与服务器断开连接（或者服务器自己断开连接）
    void OnServerDisconnected(void);

private:
    QTcpServer * Server;
    //玩家的套接字
    QTcpSocket * PlayerSocket;

    QTcpSocket * Client;
    //存数据的容器
    QByteArray * ByteArray;

};

#endif // NETWORKMODULE_H
