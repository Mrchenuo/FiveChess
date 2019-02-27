#ifndef CHESSBOARDCORE_H
#define CHESSBOARDCORE_H

#include "const.h"
#include "chess.h"
/*
  棋盘的一些核心操作

 */
class ChessBoard
{
public:
    ChessBoard();

    //设置当前点棋子类型
    bool setPointData(int col,int row,CHESSTYPE d);

    //判断当前点是否有棋子，若有，是白还是黑
    CHESSTYPE getPointData(int col,int row){return m_data[row][col];}

    void setFlag(int f){m_flag=f;}

    int getFlag(){ return m_flag;}

    void clearData();

    PAINTOUTPUTTYPE getPaintOutput(){return m_output;}

    void setOutput(PAINTOUTPUTTYPE o){m_output=o;}

    bool searchWin(CHESSTYPE chess);

    //修改搜索范围时的四角
    int left ;
    int top ;
    int right ;
    int bottom ;

private:
    PAINTOUTPUTTYPE m_output;//棋盘要进行的操作类型
    CHESSTYPE m_data[BOARD_SIZE][BOARD_SIZE];//保存的棋盘数据
    //Chess[] chessList;//记录已经下在棋盘上的棋子的数组
    int m_flag;//当前下棋的是白棋还是黑棋,默认开始是黑棋先，默认值为0
};

#endif // CHESSBOARDCORE_H
