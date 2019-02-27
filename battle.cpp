#include "battle.h"
#include "ui_battle.h"
#include <QDebug>
#include <QPainter>
#include <Qt>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QMessageBox>
#include <QTime>


Battle::Battle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Battle),
    timer(new QTimer)
{

    ui->setupUi(this);
    this->setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);
    this->setFixedSize(this->width(),this->height());

    chessCore=new ChessBoard();
    computerEvaluate=new ComputerEvaluate(chessCore);

    start=false;

    time=0;
    timer->start(1000);
    chessCount=0;
    gameMode=NONEGAME;


    option=new OnlineOption();

    networkModule=new NetworkModule();

    connect(timer,&QTimer::timeout,this,[&]{
        if(start==true)
        {
            time++;
            ui->LabelTotalTime->setText(tr("%1 Second").arg(time));
        }
    });

//    connect(this,SIGNAL(GameOver()),this,[&]{
//        if(start==true)
//        {
//            time;
//            ui->LabelTotalTime->setText(tr("%1 Second").arg(time));
//        }
//    });

    connect(this, &Battle::InToNetworkMsg, this, &Battle::OnToNetworkMsg);

    connect(option, &OnlineOption::InCancelListenning, networkModule, &NetworkModule::OnCancelListenning);
    connect(option, &OnlineOption::InServerListenning, networkModule, &NetworkModule::OnServerListenning);
    connect(option, &OnlineOption::InClientConnect, networkModule, &NetworkModule::OnClientConnect);

    connect(networkModule, &NetworkModule::InAppendLog, option, &OnlineOption::OnAppendLog);
    connect(networkModule, &NetworkModule::InStartGame, option, &OnlineOption::OnHideWidget);
    connect(networkModule, &NetworkModule::InStartGame, this, &Battle::on_BtnStart_clicked);

    connect(networkModule, &NetworkModule::InFromNetworkMsg, this, &Battle::OnFromNetworkMsg);
    connect(networkModule, &NetworkModule::InDisconnected, this, &Battle::OnDisconnected);

    connect(this,&Battle::ExitWin,networkModule, &NetworkModule::OnCancelListenning);
}

Battle::~Battle()
{
    delete ui;
    delete chessCore;
    delete timer;
    delete option;
    delete networkModule;
}

void Battle::paintEvent(QPaintEvent *)
{
    if(start==true)
    {
        if(chessCore->getFlag()==0)
            ui->LabelCurChess->setPixmap(QPixmap::fromImage(QImage(":/images/res/blackChess.png")));
        else
            ui->LabelCurChess->setPixmap(QPixmap::fromImage(QImage(":/images/res/whiteChess.png")));

        ui->LabelTotalChessNum->setText(QString::number(getChessCount(), 10));
    }





    //PAINTOUTPUTTYPE opt=chessCore->getPaintOutput();
    //qDebug()<<opt<<endl;
    //ChessCore->dataPrint();
    QPainter painter;
     //QPainter * painter = new QPainter(this);
    painter.begin(this);
    painter.drawImage(QPoint(0,0),QImage(":/images/res/playscreen.png"));
//    if (opt==BLACKWIN)
//    {
//        //painter.drawImage(QRect(535/2-200,535/2-60,400,120),QImage(":/images/res/blackwin.JPG"));
//        QMessageBox::about(NULL, "结果", "黑方胜利!");
//    }
//    if (opt==WHITEWIN)
//    {
//        //painter.drawImage(QRect(535/2-200,535/2-60,400,120),QImage(":/images/res/whitewin.JPG"));
//        QMessageBox::about(NULL, "结果", "白方胜利!");
//    }


    //还没分出胜负
//    if (opt==CHESS)
//    {
        for (int i=0;i<BOARD_SIZE;i++)
            for (int j=0;j<BOARD_SIZE;j++)
            {
                //qDebug()<<"i:"<<i<<"j:"<<j<<"data:"<<chessCore->getPointData(i,j)<<endl;
                if (chessCore->getPointData(i,j)==BLACK)
                {
                    //qDebug()<<(i*BOARD_GRID_SIZE+BOARD_OFFSET_X)<<" "<<(j*BOARD_GRID_SIZE+BOARD_OFFSET_Y)<<endl;
                    painter.drawImage(i*BOARD_GRID_SIZE+BOARD_OFFSET_X,j*BOARD_GRID_SIZE+BOARD_OFFSET_Y,QImage(":/images/res/blackChess.png"));
                }
                if (chessCore->getPointData(i,j)==WHITE)
                {
                    //qDebug()<<(i*BOARD_GRID_SIZE+BOARD_OFFSET_X)<<" "<<(j*BOARD_GRID_SIZE+BOARD_OFFSET_Y)<<endl;
                    painter.drawImage(i*BOARD_GRID_SIZE+BOARD_OFFSET_X,j*BOARD_GRID_SIZE+BOARD_OFFSET_Y,QImage(":/images/res/whiteChess.png"));

                }

            }
    //}

    //突出整盘游戏中的最后一颗棋子
    if (!chessStep.empty())
    {

        painter.setPen(QPen(QBrush(Qt::red), 2, Qt::DashLine));
        painter.setBrush(Qt::NoBrush);

        std::vector<Chess>::iterator it = chessStep.end() - 1;
        Chess chess = *it;


        int LastChooseChessWidth = 25;
        int LastChooseChessX = chess.getX()*BOARD_GRID_SIZE+BOARD_OFFSET_X;
        int LastChooseChessY = chess.getY()*BOARD_GRID_SIZE+BOARD_OFFSET_Y;
        painter.drawRect(LastChooseChessX , LastChooseChessY, LastChooseChessWidth, LastChooseChessWidth);
    }// - LastChooseChessWidth / 2

    painter.end();
}

void Battle::mousePressEvent(QMouseEvent *event)
{
    if(!start)
        return ;
    //int flag=chessCore->getFlag();
    if(getGameMode()!=DOUBLEGAME)//不是双人游戏
        if(turnYouPlay==false)//不是你下棋
            return;
    int x=0,y=0;
    if (event->button() == Qt::LeftButton)
    {
        x = event->pos().x() - BOARD_OFFSET_X;
        y = event->pos().y() - BOARD_OFFSET_Y;
       // qDebug()<<event->pos().x()<<" "<<event->pos().y()<<endl;
        //qDebug()<<BOARD_OFFSET_X<<" "<<BOARD_OFFSET_Y<<endl;
        //qDebug()<<x<<" "<<y<<endl;
        if(x < 0 || x >= BOARD_PIX_SIZE)
        {
            return;
        }
        if(y < 0 || y >= BOARD_PIX_SIZE)
        {
            return;
        }
    }
    int tx = x / BOARD_GRID_SIZE;
    int ty = y / BOARD_GRID_SIZE;
    qDebug()<<"click:"<<tx<<" "<<ty<<endl;

    //给联机玩家发送信息
    if (getGameMode() == ONLINEGAME)
    {
        //发送下的棋子的位置
        //将int转化为QString
        QString Row = QString::number(ty, 10);
        if (Row.length() < 2)
            Row = '0' + Row; //使数字变成两位数，如：02 12

        QString Column = QString::number(tx, 10);
        if (Column.length() < 2)
            Column = '0' + Column; //使数字变成两位数，如：02 12
        emit InToNetworkMsg(QString(CHESSPOSITION_ENUM) + Row + Column);
    }

    putChess(tx,ty);

    //这里存在鼠标放下，但是没有释放时机器就开始下棋的情况

//    if (event->button() == Qt::LeftButton)
//    {   qDebug()<<"OK14"<<endl;
//        mouseOffset=event->globalPos() - pos()-QPoint(81,133);
//        qDebug()<<"OK15"<<endl;
//    }
//    qDebug()<<(mouseOffset.x())<<" "<<(mouseOffset.y())<<endl;

//    int tx=(mouseOffset.x()+14)/BOARD_GRID_SIZE,ty=(mouseOffset.y()+14)/BOARD_GRID_SIZE;
//    qDebug()<<tx<<" "<<ty<<endl;


}

void Battle::mouseReleaseEvent(QMouseEvent *)
{
   // QTimer::singleShot(600000,this, SLOT(quit()));
//    //延时一下，假装机器人思考了一会
//    QTime dieTime = QTime::currentTime().addMSecs(300);
//        while( QTime::currentTime() < dieTime )
//            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
//    QFile file("d:\\output.txt");
//     if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
//        return;
//     QTextStream out(&file);
//     for(int i=0;i<BOARD_SIZE;i++)
//     {
//         for(int j=0;j<BOARD_SIZE;j++)
//         {
//             out<<chessCore->getPointData(j,i)<<" ";
//         }
//         out<<"\n\n";
//     }
//     file.close();
    int flag=chessCore->getFlag();
    if(flag==0)//不轮到机器人下
        return ;
    //当人的鼠标释放后机器人开始下棋
    switch (gameMode) {
    case MANMACHINE:
        computerGo();
        break;
    case ONLINEGAME:
        setTurnYouPlay(false);
        break;
    default:
        break;
    }
}

void Battle::closeEvent(QCloseEvent *event)
{
    if (getGameMode() == ONLINEGAME)
    {
        QMessageBox MsgBox(this);
        MsgBox.setWindowTitle(tr("确定离开？"));
        MsgBox.setText(tr("您当前正在进行联机对战，离开将视为认输\n是否离开？"));
        MsgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        MsgBox.setButtonText(QMessageBox::Yes, tr("是"));
        MsgBox.setButtonText(QMessageBox::No, tr("否"));
        MsgBox.setDefaultButton(QMessageBox::No);
        MsgBox.setIcon(QMessageBox::Question);
        switch(MsgBox.exec())
        {
        case QMessageBox::Yes:
            //认输
            emit InToNetworkMsg(QString(GAMEGIVEUP_ENUM));
            event->accept();
            emit ExitWin();
            break;
        case QMessageBox::No:
            event->ignore();
            break;
        }
    }
    else
        emit ExitWin();

}

void Battle::computerGo()
{
    computerEvaluate->setComputerColor(WHITE);
    int* pos=computerEvaluate->getTheBestPosition();
//    aiEvaluate->setComputerColor(WHITE);
//    int *pos=aiEvaluate->getTheBestPosition();

    putChess(pos[0],pos[1]);
//    static int x=0,y=0;
//    putChess(x++,y++);
}

//机器和人都调用这个函数下棋
void Battle::putChess(int tx, int ty)
{

    int flag=chessCore->getFlag();
    if (chessCore->getPointData(tx,ty)==NONE)
    {
        //执黑先行
        chessCore->setPointData(tx,ty, flag ? WHITE : BLACK);
        chessCore->setOutput(CHESS);
        //addChessCount();
        chessCount++;//棋盘中棋子数加一
        chessStep.push_back(Chess(tx,ty,flag ? WHITE : BLACK));

        repaint();

        if(chessCore->left>tx) chessCore->left=tx;
        if(chessCore->top>ty) chessCore->top=ty;
        if(chessCore->right<tx) chessCore->right=tx;
        if(chessCore->bottom<ty) chessCore->bottom=ty;

        if (chessCore->searchWin(flag ? WHITE : BLACK))
        {
            if (timer->isActive() )
                timer->stop();//停止计时

            if (flag)
            {
                chessCore->setOutput(WHITEWIN);
//                QMessageBox::about(NULL, "结果", "白方赢了!");


                QMessageBox message(QMessageBox::NoIcon, "结果", "白方赢了!");
                //message.setIconPixmap(QPixmap("../../FiveChess/res/whitewin.png"));
                message.exec();
                destroy();
                //exit(0);
                emit ExitWin();
            }
            else
            {
                chessCore->setOutput(BLACKWIN);
                //QMessageBox::about(NULL, "结果", "黑方赢了!");
                QMessageBox message(QMessageBox::NoIcon, "结果", "黑方赢了!");
                //message.setIconPixmap(QPixmap("../../FiveChess/res/blackwin.png"));
                message.exec();
                destroy();
                //exit(0);
                emit ExitWin();
            }
            start=false;
            emit GameOver();
            //killTimer(timer);

        }

        chessCore->setFlag(flag^1);//对flag异或
    }
}

//处理和棋请求
void Battle::GetDrawChessMsg(QString TextString)
{
    if (TextString == "Try")
    {
        QMessageBox MsgBox(this);
        MsgBox.setWindowTitle(tr("新申请"));
        MsgBox.setText(tr("对方向你发起和棋申请，是否同意？"));
        MsgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        MsgBox.setButtonText(QMessageBox::Yes, tr("同意"));
        MsgBox.setButtonText(QMessageBox::No, tr("不同意"));
        MsgBox.setDefaultButton(QMessageBox::Yes);
        MsgBox.setIcon(QMessageBox::Question);
        switch(MsgBox.exec())
        {
        case QMessageBox::Yes:
            emit InToNetworkMsg(QString(DRAWCHESS_ENUM) + "Yes");
            QMessageBox::about(NULL, "结果", "游戏结束！平局!");
            destroy();
            //exit(0);
            emit ExitWin();
            break;
        case QMessageBox::No:
            emit InToNetworkMsg(QString(DRAWCHESS_ENUM) + "No");
            break;
        }
    }
    else if (TextString == "Yes")
    {
        QMessageBox::about(NULL, "结果", "对方同意你的和棋请求！游戏结束！平局!");
        destroy();
        //exit(0);
        emit ExitWin();
    }
    else if (TextString == "No")
    {
        QMessageBox::about(NULL, "结果", "对方拒绝了您的和棋请求！");
    }
}

//处理悔棋信息
void Battle::GetUndoChessMsg(QString TextString)
{
//    if(turnYouPlay==false)//不是你下棋
//        return;
    if (TextString == "Try")
    {
        QMessageBox MsgBox(this);
        MsgBox.setWindowTitle(tr("新申请"));
        MsgBox.setText(tr("对方向你发起悔棋申请，是否同意？"));
        MsgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        MsgBox.setButtonText(QMessageBox::Yes, tr("同意"));
        MsgBox.setButtonText(QMessageBox::No, tr("不同意"));
        MsgBox.setDefaultButton(QMessageBox::Yes);
        MsgBox.setIcon(QMessageBox::Question);
        switch(MsgBox.exec())
        {
        case QMessageBox::Yes:
            emit InToNetworkMsg(QString(UNDOCHESS_ENUM) + "Yes");
            //turnYouPlay=false;
            UndoChess();
            break;
        case QMessageBox::No:
            //turnYouPlay=false;
            emit InToNetworkMsg(QString(UNDOCHESS_ENUM) + "No");
            break;
        }
    }
    else if (TextString == "Yes")
    {
        QMessageBox::about(NULL, "结果", "对方同意你的悔棋请求！");
        //turnYouPlay=true;
        UndoChess();

    }
    else if (TextString == "No")
    {
        //turnYouPlay=true;
        QMessageBox::about(NULL, "结果", "对方拒绝了您的悔棋请求！");
    }
}

//悔棋
void Battle::UndoChess()
{
    if(chessStep.size() < 2)
    {
        return;
    }

    for(int i = 0; i < 2; i++)
    {
        std::vector<Chess>::iterator it = chessStep.end() - 1;
        Chess chess = *it;

        int x = chess.getX();
        int y = chess.getY();

        chessCore->setPointData(x,y,NONE);
        chessStep.pop_back();
        //subChessCount();
        chessCount--;
        repaint();
    }
}

void Battle::setGameMode(GAMEMODE g)
{
    gameMode=g;
}

GAMEMODE Battle::getGameMode()
{
    return gameMode;
}

void Battle::addChessCount()
{
    chessCount++;
}

void Battle::subChessCount()
{
    chessCount--;
}


//开始按钮
void Battle::on_BtnStart_clicked()
{
    start=true;
    //timer=startTimer(1000);
    if (!(timer->isActive() ))
        timer->start(1000);//开始计时


    if (networkModule->NetworkStatus == NetworkModule::NETWORK_SERVER_CONNECT)
    {//默认服务器端执黑先行
        turnYouPlay=true;
        //chessCore->setFlag(0);
    }
    else if (networkModule->NetworkStatus == NetworkModule::NETWORK_CLIENT)
     {
        {//默认客户端执白
            turnYouPlay=false;
            //chessCore->setFlag(0);
        }
    }
}

//认输按钮
void Battle::on_BtnFail_clicked()
{
    QMessageBox::about(NULL, "结果", "你输了!");
//    start = false;
//    chessCore->clearData();
//    repaint();
    if(getGameMode()==ONLINEGAME)
        emit InToNetworkMsg(QString(GAMEGIVEUP_ENUM));
    destroy();
    //exit(0);
    emit ExitWin();
}

//悔棋按钮
void Battle::on_BtnBackStep_clicked()
{
    if(getTurnYouPlay()==false)
    {
        QMessageBox::about(NULL, "结果", "你只有在自己的回合才能悔棋!");
        return;
    }
    if(getGameMode()==ONLINEGAME)
        emit InToNetworkMsg(QString(UNDOCHESS_ENUM)+ "Try");
    else
    {
        UndoChess();
    }
   // chessStep.erase(chessStep.end());
}

//和棋按钮
void Battle::on_BtnDraw_clicked()
{
    if(getGameMode()==ONLINEGAME)
        emit InToNetworkMsg(QString(DRAWCHESS_ENUM) + "Try");
    else
    {
        QMessageBox::about(NULL, "结果", "平局!");
        //exit(0);
        destroy();
        emit ExitWin();
    }
}

//向网络中发送消息
void Battle::OnToNetworkMsg(QString Text)
{
    if (networkModule->NetworkStatus == NetworkModule::NETWORK_SERVER_CONNECT)
        networkModule->ServerMsgSend(Text);
    else if (networkModule->NetworkStatus == NetworkModule::NETWORK_CLIENT)
        networkModule->ClientMsgSend(Text);
}

//接收来自网络的消息
void Battle::OnFromNetworkMsg(QString Text)
{
    //将Text切片
    QString EnumString = Text.mid(0, ENUM_SIZE);
    QString TextString = Text.mid(ENUM_SIZE, -1);

    if (EnumString == CHESSPOSITION_ENUM)
    {
        putChess(TextString.mid(2, 2).toInt(),TextString.mid(0, 2).toInt());
        setTurnYouPlay(true);
    }
    else if (EnumString == GAMEGIVEUP_ENUM)
    {
        QMessageBox::about(NULL, "结果", "对方认输，游戏结束!");
        destroy();
        //exit(0);
        emit ExitWin();
    }
    else if (EnumString == DRAWCHESS_ENUM)//和棋请求
        GetDrawChessMsg(TextString);
    else if(EnumString==UNDOCHESS_ENUM)//悔棋请求
        GetUndoChessMsg(TextString);
}

void Battle::OnDisconnected()
{
    //QMessageBox::about(NULL, "结果", "对方离开，游戏结束!");
    destroy();
    //exit(0);
    emit ExitWin();
    //QMessageBox::about(NULL, "结果", "hello!");
}

int Battle::getChessCount() const
{
    return chessCount;
}

bool Battle::getTurnYouPlay() const
{
    return turnYouPlay;
}

void Battle::setTurnYouPlay(bool value)
{
    turnYouPlay = value;
}

void Battle::showOnlineOption()
{
    turnYouPlay=false;
    option->show();
}

