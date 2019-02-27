#include "chessboard.h"
#include <QDebug>

ChessBoard::ChessBoard()
{
    //初始化棋盘
    m_output=ONLYCHESSBOARD;
    m_flag=0;
    clearData();

    left=7 ;
    top=7 ;
    right=7 ;
    bottom=7 ;
}

bool ChessBoard::setPointData(int col, int row, CHESSTYPE d)
{
    if(col<0 || row<0 || col>=BOARD_SIZE || row>=BOARD_SIZE)
        return false;
    else
        m_data[row][col]=d;
    return true;
}

void ChessBoard::clearData()
{
    for(int j=0;j<BOARD_SIZE;j++)
        for(int i=0;i<BOARD_SIZE;i++)
            m_data[i][j]=NONE;
}

bool ChessBoard::searchWin(CHESSTYPE chess)
{
    for(int i=0;i<11;i++)
        for(int j=0;j<15;j++)
            if (m_data[i][j]==chess&&m_data[i+1][j]==chess&&m_data[i+2][j]==chess&&m_data[i+3][j]==chess&&m_data[i+4][j]==chess)
                return true;

    for(int i=0;i<15;i++)
        for(int j=0;j<11;j++)
            if (m_data[i][j]==chess&&m_data[i][j+1]==chess&&m_data[i][j+2]==chess&&m_data[i][j+3]==chess&&m_data[i][j+4]==chess)
                return true;

    for(int i=0;i<11;i++)
        for(int j=0;j<11;j++)
            if (m_data[i][j]==chess&&m_data[i+1][j+1]==chess&&m_data[i+2][j+2]==chess&&m_data[i+3][j+3]==chess&&m_data[i+4][j+4]==chess)
                return true;

    for(int i=0;i<11;i++)
        for(int j=4;j<15;j++)
            if (m_data[i][j]==chess&&m_data[i+1][j-1]==chess&&m_data[i+2][j-2]==chess&&m_data[i+3][j-3]==chess&&m_data[i+4][j-4]==chess)
                return true;

    return false;
}
