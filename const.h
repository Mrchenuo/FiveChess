#ifndef CONST_H
#define CONST_H

#include <QtNetwork>
/*
  所有的常数在此定义
*/

const int BOARD_SIZE = 15;//棋盘行列数
const int BOARD_GRID_SIZE = 28;//网格宽度
const int BOARD_OFFSET_X = 85 - (BOARD_GRID_SIZE >> 1);//x在棋盘中的偏移量
const int BOARD_OFFSET_Y = 135 - (BOARD_GRID_SIZE >> 1);//y在棋盘中的偏移量
const int BOARD_PIX_SIZE = BOARD_GRID_SIZE * BOARD_SIZE;//棋盘布的宽度

//棋子类型
enum CHESSTYPE
{
    NONE  = 0x00000000,
    WHITE = 0x00000001,
    BLACK = 0x00000002
};

//对棋盘的操作类型
enum PAINTOUTPUTTYPE
{
    ONLYCHESSBOARD = 0x00000000,
    CHESS          = 0x00000001,
    BLACKWIN       = 0x00000002,
    WHITEWIN       = 0x00000004
};

//游戏模式
enum GAMEMODE
{
    NONEGAME   = 0x00000000,
    DOUBLEGAME = 0x00000001,
    MANMACHINE = 0x00000002,
    ONLINEGAME = 0x00000004
};

const quint16 CONNECT = 1;
const quint16 JOIN = 2;
const quint16 DOWN = 3;
const quint16 UNDO = 4;
const quint16 LIST = 5;
const QString NETIP = "localhost";


#define ENUM_SIZE 2

//PlayerMsg -- 玩家发送的信息
#define PLAYERMSG_ENUM "0#"
//SysMsg -- 程序要打印出来的信息，将以红色显示
#define SYSMSG_ENUM "1#"
//ChessPosition -- 对方落下的棋子的位置,对应的位置将会接在此枚举后面 ： "0511"=>第5行第11列
#define CHESSPOSITION_ENUM "4#"
//GameGiveUp -- 认输
#define GAMEGIVEUP_ENUM "5#"
//UndoChess -- 悔棋
#define UNDOCHESS_ENUM "6#"
//DrawChess -- 和棋
#define DRAWCHESS_ENUM "7#"

#endif // CONST_H


