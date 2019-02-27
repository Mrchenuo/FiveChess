#include "chess.h"

Chess::Chess()
{

}

Chess::Chess(const Chess &c)
{
    chessX=c.chessX;
    chessY=c.chessY;
    chessColor=c.chessColor;
}

Chess::Chess(int x, int y, int color)
{
    chessX=x;
    chessY=y;
    chessColor=color;
}

Chess::~Chess()
{

}

int Chess::getY()
{
    return chessY;
}

int Chess::getX()
{
    return chessX;
}

int Chess::getChessColor()
{
    return chessColor;
}
