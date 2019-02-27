#include "networkmodule.h"
#include "battle.h"
NetworkModule::NetworkModule(QObject *parent) : QObject(parent)
{
    NetworkStatus = NETWORK_NONE;
    PlayerSocket = nullptr;

    Server = new QTcpServer();
    Client = new QTcpSocket();
    ByteArray = new QByteArray();

    //Server
    connect(Server, &QTcpServer::newConnection, this, &NetworkModule::OnNewConnectServer);

    //Client
    connect(Client, &QAbstractSocket::connected, this, &NetworkModule::OnNewConnectClient);

    //当断开连接时发出的信号
    connect(Client, &QAbstractSocket::disconnected, this, &NetworkModule::OnClientDisconnected);

    //客户端接收数据
    connect(Client, &QIODevice::readyRead, this, &NetworkModule::OnClientMsgReceive);


}

NetworkModule::~NetworkModule()
{
    delete Client;
    delete Server;
    delete ByteArray;
}

QString NetworkModule::InsideIPAddress()
{
    QString IP;
    foreach(QHostAddress Address, QNetworkInterface::allAddresses())
    {
        //此if判断只寻找当前的内网IPv4地址
        if(Address.protocol() == QAbstractSocket::IPv4Protocol &&
                !Address.toString().contains("127.0.0.1"))
            IP = Address.toString();
    }
    return IP;
}

void NetworkModule::ServerMsgSend(QString Msg)
{
    ByteArray->clear();
    ByteArray->append(Msg);

    PlayerSocket->write(*ByteArray);
}

void NetworkModule::ClientMsgSend(QString Msg)
{
    //只发送数据
    ByteArray->clear();
    ByteArray->append(Msg);

    Client->write(*ByteArray);
}

void NetworkModule::OnServerListenning()
{
    if (NetworkStatus != NETWORK_NONE)
    {
        if (NetworkStatus == NETWORK_CLIENT)
            emit InAppendLog(SYSMSG_ENUM + tr("您正在连接对局中，无法创建对局"));
        else
            emit InAppendLog(SYSMSG_ENUM + tr("您已经创建过对局，重复操作无效！！！"));
        return;
    }
    else
        NetworkStatus = NETWORK_SERVER_LISTEN;

    //创建监听失败的重试次数
    int FailedCount;
    for (FailedCount = 0; FailedCount < 3; FailedCount++)
    {
        if(Server->listen(QHostAddress::Any, 12345))
            break;
        else
            emit InAppendLog(SYSMSG_ENUM + tr("创建对局失败！！！正在重试...") + QString("%1/3").arg(FailedCount + 1));
    }
    //如果失败次数小于3次
    if (FailedCount < 3)
        emit InAppendLog(PLAYERMSG_ENUM + tr("对局创建成功，等待玩家连接..."));
    else
    {
        QString strError = Server->errorString();
        emit InAppendLog(SYSMSG_ENUM + strError+"\n");
        emit InAppendLog(SYSMSG_ENUM + tr("创建对局失败！！！请稍后重试"));
        NetworkStatus = NETWORK_NONE;
    }
    //Server->close();

   // close();
}

void NetworkModule::OnCancelListenning()
{
    if (NetworkStatus == NETWORK_CLIENT)
    {
        Client->disconnectFromHost();
    }
    else if (NetworkStatus == NETWORK_SERVER_CONNECT)
    {
        PlayerSocket->disconnectFromHost();
    }
    Server->close();
    NetworkStatus = NETWORK_NONE;
}

void NetworkModule::OnClientConnect(QString IP)
{
    if (NetworkStatus != NETWORK_NONE)
    {
        if (NetworkStatus == NETWORK_CLIENT)
            emit InAppendLog(SYSMSG_ENUM + tr("您已经发起连接，重复操作无效"));
        else
            emit InAppendLog(SYSMSG_ENUM + tr("您已创建对局，等待玩家连接。如需连接玩家对局，请取消创建对局"));
        return;
    }
    else
        NetworkStatus = NETWORK_CLIENT;

    emit InAppendLog(PLAYERMSG_ENUM + QString(tr("尝试连接%1...")).arg(IP));
    Client->connectToHost(IP, 12345);

    if (!Client->waitForConnected(1000))
    {
        int i = Client->error();
        switch (i)
        {
        case QAbstractSocket::ConnectionRefusedError:
            emit InAppendLog(SYSMSG_ENUM + tr("错误码:%1 连接被拒绝，请检查IP地址是否错误").arg(i));
            break;
        case QAbstractSocket::RemoteHostClosedError:
            emit InAppendLog(SYSMSG_ENUM + tr("错误码:%1 远程主机强迫关闭了一个现有的连接，请重新连接").arg(i));
            break;
        case QAbstractSocket::HostNotFoundError:
            emit InAppendLog(SYSMSG_ENUM + tr("错误码:%1 未找到主机，请检查IP地址是否错误").arg(i));
            break;
        case QAbstractSocket::SocketTimeoutError:
            emit InAppendLog(SYSMSG_ENUM + tr("错误码:%1 连接超时，请检查IP地址是否错误").arg(i));
            break;
        default:
            emit InAppendLog(SYSMSG_ENUM + tr("错误码:%1 网络模块出错").arg(i));
            break;
        }
        Client->close();
        NetworkStatus = NETWORK_NONE;
    }
}

void NetworkModule::OnNewConnectServer()
{
    NetworkStatus = NETWORK_SERVER_CONNECT;

    QTcpSocket * NewSocket;

    //有客户端连接就发射newConnection信号，调用该槽来接受这个连接，根据当前新连接创建一个QTepSocket
    NewSocket = Server->nextPendingConnection();

    connect(NewSocket, &QIODevice::readyRead, this, &NetworkModule::OnServerMsgReceive);
    connect(NewSocket, &QAbstractSocket::disconnected, this, &NetworkModule::OnServerDisconnected);

    PlayerSocket = NewSocket;
    //暂停接受任何连接
    Server->pauseAccepting();
    emit InAppendLog(PLAYERMSG_ENUM + tr("玩家已连接成功，准备开始对局..."));
    emit InStartGame();
}

void NetworkModule::OnNewConnectClient()
{
    emit InAppendLog(PLAYERMSG_ENUM + tr("对局连接成功，正在准备对局..."));
    emit InStartGame();
}

void NetworkModule::OnClientMsgReceive()
{
    emit InFromNetworkMsg(Client->readAll());
}

void NetworkModule::OnServerMsgReceive()
{
    emit InFromNetworkMsg(PlayerSocket->readAll());
}

void NetworkModule::OnClientDisconnected()
{
    emit InDisconnected();
    NetworkStatus = NETWORK_NONE;
}

void NetworkModule::OnServerDisconnected()
{
    emit InDisconnected();

    PlayerSocket->deleteLater();
    PlayerSocket = nullptr;

    if(NetworkStatus == NETWORK_SERVER_CONNECT)
    {
        NetworkStatus = NETWORK_SERVER_LISTEN;
        Server->resumeAccepting();
    }
}
