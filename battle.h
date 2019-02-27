#ifndef BATTLE_H
#define BATTLE_H

#include <QWidget>
#include <QPoint>
#include <QPainter>
#include "ChessBoard.h"
#include "const.h"
#include <QTimer>
#include "chess.h"
#include "computerevaluate.h"
#include "onlineoption.h"
#include "networkmodule.h"
/*
 * 对战的一些事件
*/

namespace Ui {
class Battle;
}

class Battle : public QWidget
{
    Q_OBJECT

public:
    explicit Battle(QWidget *parent = 0);
    ~Battle();

    void setGameMode(GAMEMODE g);
    GAMEMODE getGameMode();

    void addChessCount();
    void subChessCount();

    bool getTurnYouPlay() const;
    void setTurnYouPlay(bool value);

    void showOnlineOption();

    int getChessCount() const;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void closeEvent(QCloseEvent *event);

    void computerGo();
    void putChess(int tx,int ty);

    void GetDrawChessMsg(QString TextString);
    void GetUndoChessMsg(QString TextString);

    void UndoChess();
signals:
    void ExitWin();//退出当前窗口

    //发送给联网玩家的数据
    void InToNetworkMsg(QString);

    void GameOver();//游戏结束
private slots:
    void on_BtnStart_clicked();

    void on_BtnFail_clicked();

    void on_BtnBackStep_clicked();

    void on_BtnDraw_clicked();

    //发送消息给联网玩家
    void OnToNetworkMsg(QString Text);

    //处理从联机玩家来的消息
    void OnFromNetworkMsg(QString Text);

    //在联机对战中，突然断开连接
    void OnDisconnected(void);
private:
    Ui::Battle *ui;
    QPoint mouseOffset;
    ChessBoard *chessCore;
    QTimer *timer;
    std::vector<Chess> chessStep;
    GAMEMODE gameMode;
    bool start;
    int time;
    int chessCount;//当前棋盘上棋子的数目
    ComputerEvaluate *computerEvaluate;

    bool turnYouPlay;//联网对战和人机对战时用到的，决定是否为你下棋
    OnlineOption* option;
    NetworkModule* networkModule;


};

#endif // BATTLE_H
