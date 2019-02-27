#ifndef COMPUTEREVALUATE_H
#define COMPUTEREVALUATE_H

#include "const.h"
#include "chessboard.h"

/*
 * 电脑的价值评估类
 * 估值分为位置估值和棋型估值
 */

class ComputerEvaluate
{
public:
    ComputerEvaluate();
    ComputerEvaluate(ChessBoard *cb);
    ~ComputerEvaluate();

    int* getTheBestPosition();


    CHESSTYPE getComputerColor() const;
    void setComputerColor(const CHESSTYPE &value);

private:
    int getValue(int chessCount1,int chessCount2,int chessCount3,int spaceCount1,int spaceCount2,int spaceCount3,int spaceCount4);
    int getValue(int chessCount,int spaceCount1,int spaceCount2);
    int evaluateValue(int color,int col,int row,int dir);
    void sort(int **allValue, int k);
    int evaluateGame();
    int evaluateLine(int *lineState,int num,int color);
    int **getTheMostValuablePositions(int &size);
    int min(int depth, int alpha, int beta);
    int max(int depth, int alpha, int beta);
    void getTheSpaceValues();

    static const int FIVE=50000;//五子连珠
    static const int HUO_FOUR=5000;//活四
    static const int CHONG_FOUR=1000;//冲四
    static const int HUO_THREE=500;//活三
    static const int MIAN_THREE=100;//眠三
    static const int HUO_TWO=50;//活二
    static const int LARGE_NUMBER=10000000;//一个很大的数
    static const int SEARCH_DEPTH=3;//极小极大搜索的深度
    static const int SAMPLE_NUMBER=10;//搜索时选择的样本数

    ChessBoard *cb;
    int getValueNum;

    int blackValue[BOARD_SIZE][BOARD_SIZE];//保存每一空位下黑子的价值
    int whiteValue[BOARD_SIZE][BOARD_SIZE];//保存每一空位下白子的价值
    int staticValue[BOARD_SIZE][BOARD_SIZE];//保存每一点的位置价值，越靠近中心，价值越大
    CHESSTYPE computerColor;

};

#endif // COMPUTEREVALUATE_H
