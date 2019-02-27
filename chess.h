#ifndef CHESS_H
#define CHESS_H
#include "const.h"
/*
    棋子类，记录棋子的位置信息等
*/

class Chess
{
public:
    Chess();
    Chess(const Chess&);
    Chess(int x, int y, int color);
    ~Chess();

    int getY();
    int getX();
    int getChessColor();
private:
    int chessX;//棋子在棋盘上的x索引
    int chessY;//棋子在棋盘上的y索引
    int chessColor;//是黑棋还是白棋
};

#endif // CHESS_H
