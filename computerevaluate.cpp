#include "computerevaluate.h"
#include <stdlib.h>
#include<QFile>
#include<QTextStream>
#include<qDebug>
ComputerEvaluate::ComputerEvaluate()
{

}

ComputerEvaluate::ComputerEvaluate(ChessBoard *cb)
{
    getValueNum=0;
    this->cb=cb;
    for(int i=0;i<BOARD_SIZE;i++)
        for(int j=0;j<BOARD_SIZE;j++)
        {
            blackValue[i][j]=0;
            whiteValue[i][j]=0;
        }

    //初始化每个位置的估值，最中心点为7，其余往边缘扩散减一
    //由于棋盘对称，由左上角1/4位置的分值赋值给其余位置
    for(int i=0;i<=BOARD_SIZE/2;i++)
        for(int j=0;j<=BOARD_SIZE/2;j++)
        {
            staticValue[i][j]=i<j ? i:j;
            staticValue[BOARD_SIZE-1-i][j]=staticValue[i][j];
            staticValue[i][BOARD_SIZE-1-j]=staticValue[i][j];
            staticValue[BOARD_SIZE-1-i][BOARD_SIZE-1-j]=staticValue[i][j];
        }
//    QFile file("d:\\staticValue.txt");
//    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
//        return;
//    QTextStream out(&file);
//    for(int i=0;i<BOARD_SIZE;i++)
//    {
//        for(int j=0;j<BOARD_SIZE;j++)
//        {
//            out << " " << staticValue[i][j];
//        }
//        out<<"\n";
//    }
//    file.close();
}

int *ComputerEvaluate::getTheBestPosition()
{
//    for(int i=0; i<=BOARD_SIZE-1; i++){
//        for(int j=0; j<=BOARD_SIZE-1; j++){//对棋盘的所有点循环
//            blackValue[i][j] = 0;
//            whiteValue[i][j] = 0;
//            if(cb->getPointData(i,j)==NONE){  //如果是空位，进行估值
//                for(int m=1; m<=4; m++){ //每个点的分值为四个方向分值之和
//                    blackValue[i][j] += evaluateValue( BLACK, i, j, m );
//                    whiteValue[i][j] += evaluateValue( WHITE, i, j, m );
//                }
//            }
//        }
//    }
    getTheSpaceValues();

    int maxValue = -LARGE_NUMBER;
    int value;
    int size;
    int* position = new int[2];

    int** valuablePositions = getTheMostValuablePositions(size);

    for(int i=0; i<size; i++){
        if(valuablePositions[i][2]>=FIVE){   //已经连五
            position[0] = valuablePositions[i][0];
            position[1] = valuablePositions[i][1];
            break;
        }
        cb->setPointData(valuablePositions[i][0],valuablePositions[i][1],getComputerColor());//假设下一个棋

        int oldLeft= cb->left; //改变LEFT、TOP等值
        int oldTop= cb->top; //改变LEFT、TOP等值
        int oldRight= cb->right; //改变LEFT、TOP等值
        int oldBottom = cb->bottom; //改变LEFT、TOP等值
        if(cb->left>valuablePositions[i][0])  cb->left=valuablePositions[i][0];
        if(cb->top>valuablePositions[i][1])  cb->top = valuablePositions[i][1];
        if( cb->right<valuablePositions[i][0]) cb->right = valuablePositions[i][0];
        if( cb->bottom< valuablePositions[i][1]) cb->bottom=valuablePositions[i][1];

        value = min(SEARCH_DEPTH,-LARGE_NUMBER,LARGE_NUMBER);
        cb->setPointData(valuablePositions[i][0],valuablePositions[i][1],NONE);

        cb->left=oldLeft;
        cb->top = oldTop;
        cb->right= oldRight;
        cb->bottom=oldBottom;

        if( value > maxValue){
            maxValue = value;
            position[0] = valuablePositions[i][0];
            position[1] = valuablePositions[i][1];
        }
    }
//    int k=0;
//    //定义一个保存总估值得分的具有三个列的二维数组，第一、二列是某点的列坐标和行坐标，第三列是该点总的分值
//    //定义这样一个数组主要是为了将来要对总得分进行排序
//    //int [][] totalValue = new int[(BOARD_SIZE-1+1)*(BOARD_SIZE-1+1)][3];
//    int **totalValue=new int*[BOARD_SIZE*BOARD_SIZE];
//    for(int i=0;i<BOARD_SIZE*BOARD_SIZE;i++)
//        totalValue[i]=new int[3];

//    for(int i=0; i<=BOARD_SIZE-1; i++){
//        for(int j=0; j<=BOARD_SIZE-1; j++){
//            totalValue[k][0] = i;
//            totalValue[k][1] = j;
//            totalValue[k][2] = blackValue[i][j] + whiteValue[i][j]+staticValue[i][j];
//            k++;
//        }
//    }

    //            QFile file("d:\\before.txt");
    //            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    //                return 0;
    //            QTextStream out(&file);
    //            out<<"before\n";
    //            for(int i=0;i<BOARD_SIZE*BOARD_SIZE;i++)
    //            {

    //                    out << totalValue[i][2]<<"\n";


    //            }
    //            file.close();

    //sort(totalValue,k);  //对总分值降序排序
    //            int length=k;//sizeof(totalValue)/sizeof(totalValue[0]);
    //            qDebug()<<length<<endl;
    //            qDebug()<<k<<endl;
    //            for (int i = 0; i < length-1 ; i++) {
    //                for (int j= 0; j < length - 1-i; j++) {
    //                    int ti, tj, tvalue;
    //                    if (totalValue[j][2] <totalValue[j + 1][2])  {
    //                        tvalue = totalValue[j][2];
    //                        totalValue[j] [2]= totalValue[j + 1][2];
    //                        totalValue[j+1][2] =tvalue;

    //                        ti = totalValue[j][0];
    //                        totalValue[j][0] = totalValue[j + 1][0];
    //                        totalValue[j+1][0] =ti;

    //                        tj = totalValue[j][1];
    //                        totalValue[j][1] = totalValue[j + 1][1];
    //                        totalValue[j+1][1] =tj;

    //                    }
    //                }
    //            }

    //            QFile file1("d:\\after.txt");
    //            if (!file1.open(QIODevice::WriteOnly | QIODevice::Text))
    //                return 0;
    //            QTextStream out1(&file1);
    //            out1<<"after\n";
    //            for(int i=0;i<BOARD_SIZE*BOARD_SIZE;i++)
    //            {

    //                    out1 << totalValue[i][2]<<"\n";


    //            }
    //            file1.close();


    //qDebug()<<"估值次数:"<<getValueNum;
    return  position;
}

/*
       * 根据棋型，计算该点下一个棋子的价值
       * @param chessCount1    该空位置下一个棋子后同种颜色棋子连续的个数
       * @param spaceCount1  连续棋子一端的连续空位数
       * @param chessCount2  如果spaceCount1为1，继续连续同种颜色棋子的个数
       * @param spaceCount2  继chessCount2之后，连续空位数
       * @param spaceCount3  连续棋子另一端的连续空位数
       * @param chessCount3  如果spaceCount3为1，继续连续同种颜色棋子的个数
       * @param spaceCount4  继chessCount3之后，连续空位数
       * @return  该点放color棋子给color方带来的价值
    */
int ComputerEvaluate::getValue(int chessCount1, int chessCount2, int chessCount3, int spaceCount1, int spaceCount2, int spaceCount3, int spaceCount4)
{
    getValueNum++;
    int value = 0;
    //将棋型分为成五、活四、冲四、活三、冲三、活二、冲二。
    switch(chessCount1){
    case 5:  //如果已经可以连成5子，则赢棋
        value = FIVE;
        break;
    case 4:
        if( (spaceCount1>0) && (spaceCount3>0) ){ //活四
            value = HUO_FOUR;
        }
        else{  //冲四
            value = CHONG_FOUR;
        }
        break;
    case 3:
        if( ( (spaceCount1==1) && (chessCount2>=1) ) && ( (spaceCount3==1) && (chessCount3>=1) ) ){ // AOAAAOA
            value = HUO_FOUR;
        }
        else if( ( (spaceCount1==1) && (chessCount2>=1) ) || ( (spaceCount3==1) && (chessCount3>=1) ) ){ // AAAOA
            value = CHONG_FOUR;
        }
        else if( ((spaceCount1>1) && (spaceCount3>0)) ||( (spaceCount1>0) && (spaceCount3>1)) ){//OAAAOO
                value = HUO_THREE;
        }
        else{  // 眠三
                value = MIAN_THREE;
        }
        break;
    case 2:
        if( (spaceCount1==1) && (chessCount2>=2) && (spaceCount3==1) && (chessCount3>=2)  ){ // AAOAAOAA
            value = HUO_FOUR;
        }
        else if( ( (spaceCount1==1) && (chessCount2>=2) ) || ( (spaceCount3==1) && (chessCount3>=2) ) ){ // AAOAA
            value = CHONG_FOUR;
        }
        else if( ( (spaceCount1==1) && (chessCount2==1) &&(spaceCount2>0)&&(spaceCount3>0)) || ( (spaceCount3==1) && (chessCount3==1) )&&(spaceCount1>0)&&(spaceCount4>0) ){ // AAOA
            value = HUO_THREE;
        }
        else if( (spaceCount1>0) && (spaceCount3>0) ){ //活二
                value = HUO_TWO;
        }

        break;
    case 1:
        if( ( (spaceCount1==1) && (chessCount2>=3) ) || (spaceCount3==1) && (chessCount3>=3)  ){ // AOAAA
            value = CHONG_FOUR;
        }
        else if( ( (spaceCount1==1) && (chessCount2==2) && (spaceCount2>=1)&& (spaceCount3>=1)) || ( (spaceCount3==1) && (chessCount3==2)&& (spaceCount1>=1) && (spaceCount4>=1)) ){ // OAOAAO
            value = HUO_THREE;
        }

        else if( ( (spaceCount1==1) && (chessCount2==2) && ((spaceCount2>=1)||(spaceCount3>=1))) || ( (spaceCount3==1) && (chessCount3==2)&& ((spaceCount1>=1) || (spaceCount4>=1))) ){ // OAOAAO
            value = MIAN_THREE;
        }
        else if( ( (spaceCount1==1) && (chessCount2==1) &&(spaceCount2>1)&&(spaceCount3>0)) || ( (spaceCount3==1) && (chessCount3==1) )&&(spaceCount1>0)&&(spaceCount4>1) ){ // OAOAOO
            value = HUO_TWO;
        }
        break;
    default:
        value = 0;
        break;
    }
    return value;
}

int ComputerEvaluate::getValue(int chessCount, int spaceCount1, int spaceCount2)
{
    int value = 0;
    //将棋型分为成五、活四、冲四、活三、眠三、活二。
        switch(chessCount){
    case 5:  //如果已经可以连成5子，则赢棋
        value = FIVE;
        break;
    case 4:
        if( (spaceCount1>0) && (spaceCount2>0) ){ //活四
            value = HUO_FOUR;
        }
        else{  //冲四
            value = CHONG_FOUR;
        }
        break;
    case 3:
        if( (spaceCount1>0) && (spaceCount2>0)  ){//活三
            value = HUO_THREE;
        }
        else{  // 眠三
            value = MIAN_THREE;
        }
        break;
    case 2:
        if( (spaceCount1>0) && (spaceCount2>0) ){ //活二
            value = HUO_TWO;
        }
        break;
    default:
        value = 0;
        break;
    }
    return value;
}

/*
       * 计算棋盘上指定空位在指定方向价值
       * @param color   要计算的是哪一方的价值，1：黑方，2：白方
       * @param col   要计算位置的列坐标
       * @param row   要计算位置的行坐标
       * @param dir   要计算方向，1：水平，2：垂直，3：左上到右下，4：右上到左下
       * @return  价值
       */
int ComputerEvaluate::evaluateValue(int color, int col, int row, int dir)
{
    int k, m;
            int value = 0;
            int chessCount1 = 1;  // 指定颜色的棋子数
            int chessCount2 = 0;  // 指定颜色的棋子数
            int chessCount3 = 0;  // 指定颜色的棋子数
            int spaceCount1= 0;  //一端的空位数
            int spaceCount2 = 0; //另一端空位数
            int spaceCount3 = 0; //另一端空位数
            int spaceCount4 = 0; //另一端空位数
            switch(dir){
            case 1: //水平方向
                //向增加的方向查找相同颜色连续的棋子
                for(k=col+1; k<=BOARD_SIZE-1; k++ ){
                    if(cb->getPointData(k,row)==color){
                        chessCount1++;
                    }
                    else{
                        break;
                    }
                }
                //在棋子尽头查找连续的空格数
                while((k<=BOARD_SIZE-1) && (cb->getPointData(k,row)==0)){
                    spaceCount1++;
                    k++;
                }
                if(spaceCount1==1){
                    while((k<=BOARD_SIZE-1)  && (cb->getPointData(k,row)==color)){
                        chessCount2++;
                        k++;
                    }
                    while((k<=BOARD_SIZE-1)  && (cb->getPointData(k,row)==0)){
                        spaceCount2++;
                        k++;
                    }
                }

                //向相反方向查找相同颜色连续的棋子
                for(k=col-1; k>=0; k-- ){
                    if(cb->getPointData(k,row)==color){
                        chessCount1++;
                    }
                    else{
                        break;
                    }
                }
                //在棋子的尽头查找连续的空格数
                while(k>=0 && (cb->getPointData(k,row)==0)){
                    spaceCount3++;
                    k--;
                }
                if(spaceCount3==1){
                    while((k>=0)  && (cb->getPointData(k,row)==color)){
                        chessCount3++;
                        k--;
                    }
                    while((k>=0) && (cb->getPointData(k,row)==0)){
                        spaceCount4++;
                        k--;
                    }
                }
                break;
            case 2:  //  垂直方向
                //向增加的方向查找相同颜色连续的棋子
                for(k=row+1; k<=BOARD_SIZE-1; k++ ){
                    if(cb->getPointData(col,k)==color){
                        chessCount1++;
                    }
                    else{
                        break;
                    }
                }
                //在棋子尽头查找连续的空格数
                while((k<=BOARD_SIZE-1) && (cb->getPointData(col,k)==0)){
                    spaceCount1++;
                    k++;
                }
                if(spaceCount1==1){
                    while((k<=BOARD_SIZE-1)  && (cb->getPointData(col,k)==color)){
                        chessCount2++;
                        k++;
                    }
                    while((k<=BOARD_SIZE-1)  && (cb->getPointData(col,k)==0)){
                        spaceCount2++;
                        k++;
                    }
                }

                //向相反方向查找相同颜色连续的棋子
                for(k=row-1; k>=0; k-- ){
                    if(cb->getPointData(col,k)==color){
                        chessCount1++;
                    }
                    else{
                        break;
                    }
                }
                //在相反方向的棋子尽头查找连续的空格数
                while(k>=0 && (cb->getPointData(col,k)==0)){
                    spaceCount3++;
                    k--;
                }
                if(spaceCount3==1){
                    while((k>=0)  && (cb->getPointData(col,k)==color)){
                        chessCount3++;
                        k--;
                    }
                    while((k>=0) && (cb->getPointData(col,k)==0)){
                        spaceCount4++;
                        k--;
                    }
                }
                break;
            case 3:  //  左上到右下
                //向增加的方向查找相同颜色连续的棋子
                for(k=col+1, m=row+1; (k<=BOARD_SIZE-1) &&(m<=BOARD_SIZE-1); k++ ,m++){
                    if(cb->getPointData(k,m)==color){
                        chessCount1++;
                    }
                    else{
                        break;
                    }
                }
                //在棋子尽头查找连续的空格数
                while((k<=BOARD_SIZE-1) &&(m<=BOARD_SIZE-1)&& (cb->getPointData(k,m)==0)){
                    spaceCount1++;
                    k++;
                    m++;
                }
                if(spaceCount1==1){
                    while((k<=BOARD_SIZE-1)  &&(m<=BOARD_SIZE-1) && (cb->getPointData(k,m)==color)){
                        chessCount2++;
                        k++;
                        m++;
                    }
                    while((k<=BOARD_SIZE-1) &&(m<=BOARD_SIZE-1)  && (cb->getPointData(k,m)==0)){
                        spaceCount2++;
                        k++;
                        m++;
                    }
                }

                //向相反方向查找相同颜色连续的棋子
                for(k=col-1, m=row-1; (k>=0)&&(m>=0); k--,m-- ){
                    if(cb->getPointData(k,m)==color){
                        chessCount1++;
                    }
                    else{
                        break;
                    }
                }
                //在相反方向的棋子尽头查找连续的空格数
                while( (k>=0) && (m>=0) && (cb->getPointData(k,m)==0)){
                    spaceCount3++;
                    k--;
                    m--;
                }
                if(spaceCount3==1){
                    while((k>=0) && (m>=0) && (cb->getPointData(k,m)==color)){
                        chessCount3++;
                        k--;
                        m--;
                    }
                    while((k>=0)  && (m>=0) && (cb->getPointData(k,m)==0)){
                        spaceCount4++;
                        k--;
                        m--;
                    }
                }
                break;

            case 4:  //  右上到左下
                for(k=col+1, m=row-1; k<=BOARD_SIZE-1 && m>=0; k++,m--){  //查找连续的同色棋子
                    if(cb->getPointData(k,m)==color){
                        chessCount1++;
                    }
                    else{
                        break;
                    }
                }
                while(k<=BOARD_SIZE-1 && m>=0 && (cb->getPointData(k,m)==0)){ //统计空位数
                    spaceCount1++;
                    k++;
                    m--;
                }
                if(spaceCount1==1){
                    while((k<=BOARD_SIZE-1) &&( m>=0) && (cb->getPointData(k,m)==color)){
                        chessCount2++;
                        k++;
                        m--;
                    }
                    while((k<=BOARD_SIZE-1) &&( m>=0) && (cb->getPointData(k,m)==0)){
                        spaceCount2++;
                        k++;
                        m--;
                    }
                }

                for(k=col-1, m=row+1; k>=0 && m<=BOARD_SIZE-1; k--,m++){  //查找连续的同色棋子
                    if(cb->getPointData(k,m)==color){
                        chessCount1++;
                    }
                    else{
                        break;
                    }
                }
                while(k>=0 && m<=BOARD_SIZE-1 &&( cb->getPointData(k,m)==0)){ // 统计空位数
                    spaceCount3++;
                    k--;
                    m++;
                }
                if(spaceCount3==1){
                    while((k>=0) &&( m<=BOARD_SIZE-1) && (cb->getPointData(k,m)==color)){
                        chessCount3++;
                        k--;
                        m++;
                    }
                    while((k>=0) &&( m<=BOARD_SIZE-1)  && (cb->getPointData(k,m)==0)){
                        spaceCount4++;
                        k--;
                        m++;
                    }
                }
                break;
            }
            if(chessCount1 + chessCount2 +chessCount3 +spaceCount1 + spaceCount2  +spaceCount3 + spaceCount4>=5){ //只有同色棋子数加两端的空位数不少于5时，才有价值
                value = getValue(chessCount1,chessCount2, chessCount3,  spaceCount1, spaceCount2, spaceCount3, spaceCount4);
            }
            return value;
}


/*
     * 对数组按第三列（allValue[][2]降序排序)
     * @param allValue: 待排序的数组，二维数组的前两列是棋盘位置坐标，第3列是该位置的价值
     */
void ComputerEvaluate::sort(int **allValue,int k)
{
    int length=k;//sizeof(allValue)/sizeof(allValue[0]);
    //qDebug()<<length<<endl;
    for (int i = 0; i < length-1 ; i++) {
        for (int j= 0; j < length - 1-i; j++) {
            int ti, tj, tvalue;
            if (allValue[j][2] <allValue[j + 1][2])  {
                tvalue = allValue[j][2];
                allValue[j] [2]= allValue[j + 1][2];
                allValue[j+1][2] =tvalue;

                ti = allValue[j][0];
                allValue[j][0] = allValue[j + 1][0];
                allValue[j+1][0] =ti;

                tj = allValue[j][1];
                allValue[j][1] = allValue[j + 1][1];
                allValue[j+1][1] =tj;

            }
        }
    }
}


/*
     * 计算当前局面的估值
     * @return   当前盘面的估值
    */
int ComputerEvaluate::evaluateGame()//达到限制的搜索深度后，调用评价函数根据当前局面给出得分
{
    int value=0;
    int i,j,k;
    int* line = new int[BOARD_SIZE];
    //水平  对每一行估值
//    for(j=0; j<=BOARD_SIZE-1; j++){
//        for(i=0; i<=BOARD_SIZE-1; i++){
    for(j=cb->top; j<=cb->bottom; j++){
        for(i=0; i<=BOARD_SIZE-1; i++){
            line[i] = cb->getPointData(i,j);   //第一个下标是列下标
        }
        value += evaluateLine(line, BOARD_SIZE,BLACK);//加上黑棋的价值
        value -= evaluateLine(line, BOARD_SIZE, WHITE);//减去白棋的价值
    }
    // 对每一列估值
//    for(i=0; i<=BOARD_SIZE-1; i++){
//        for(j=0; j<=BOARD_SIZE-1; j++){
    for(i=cb->left; i<=cb->right; i++){
        for(j=0; j<=BOARD_SIZE-1; j++){
            line[j] = cb->getPointData(i,j);
        }
        value += evaluateLine(line, BOARD_SIZE, BLACK);
        value -= evaluateLine(line, BOARD_SIZE, WHITE);
    }

    // 左下到右上斜线估值
    for(j = 4; j<=BOARD_SIZE-1; j++){
        for(k=0; k<=j;  k++){
            line[k] = cb->getPointData(k,j-k);
        }
        value += evaluateLine(line, j+1, BLACK);
        value -= evaluateLine(line, j+1, WHITE);
    }
    for(j = 1; j<=BOARD_SIZE-5; j++){
        for(k=0; k<=BOARD_SIZE-1-j; k++){
            line[k] = cb->getPointData(k+j,BOARD_SIZE-1-k);
        }
        value += evaluateLine(line, BOARD_SIZE-j, BLACK);
        value -= evaluateLine(line, BOARD_SIZE-j, WHITE);
    }
    // 左上到右下斜线估值
    for(j = 0; j<=BOARD_SIZE-5; j++){
        for(k=0;k<=BOARD_SIZE-1-j; k++){
            line[k] = cb->getPointData(k,k+j);
        }
        value += evaluateLine(line,BOARD_SIZE-j, BLACK);
        value -= evaluateLine(line, BOARD_SIZE-j, WHITE);
    }
    for(i= 1; i<=BOARD_SIZE-5; i++){
        for(k=0;k<=BOARD_SIZE-1-i;k++){
            line[k] = cb->getPointData(k+i,k);
        }
        value += evaluateLine(line,BOARD_SIZE-i, BLACK);
        value -= evaluateLine(line, BOARD_SIZE-i, WHITE);
    }
    if(getComputerColor()==BLACK){
        return value;   //如果计算机执黑先行，则返回value
    }
    else{
        return -value;  //如果计算机执白，则返回-value
    }
}

int ComputerEvaluate::evaluateLine(int *lineState, int num, int color)
{
    int chess, space1, space2;
        int i,j;
        int value = 0;
        int begin,end;
        for(i=0; i<num; i++)
            if(lineState[i] == color){  //遇到要找的棋子，检查棋型，得到对应的分值
                chess = 1;
                begin = i;
                for(j=begin+1; (j<num)&&(lineState[j]==color) ;j++  ){
                    chess++;
                }
                if(chess<2){
                    continue;
                }
                end = j-1;

                space1 = 0;
                space2=0;
                for(j=begin-1; (j>=0)&&((lineState[j]==0)||(lineState[j]==color)) ;j--  ){//棋子前面的空格
                    space1++;
                }
                for(j=end+1; (j<num)&&((lineState[j]==0)||(lineState[j]==color)) ;j++  ){//棋子后面的空格
                    space2++;
                }

                if(chess+space1+space2 >=5){
                    value += getValue(chess, space1, space2);
                }
                i=end+1;
        }
        return value;
}


/*
 * 查找棋盘上价值最大的几个空位，每个空位的价值等于两种棋的价值之和。
 * @return   价值最大的几个空位（包括位置和估值）
*/
int **ComputerEvaluate::getTheMostValuablePositions(int &size)
{
    int i,j,k=0;

    int l,t,r,b;
    l=(cb->left > 2) ? cb->left-2 : 0;
    t=(cb->top > 2) ? cb->top-2 : 0;
    r=(cb->right < BOARD_SIZE-2) ? cb->right+2 : BOARD_SIZE-1;
    b=(cb->bottom < BOARD_SIZE-2) ? cb->bottom+2 : BOARD_SIZE-1;


    //allValue：保存每一空位的价值(列坐标，行坐标，价值）
    int **allValue=new int*[BOARD_SIZE*BOARD_SIZE];
    for(int i=0;i<BOARD_SIZE*BOARD_SIZE;i++)
        allValue[i]=new int[3];

//    for(i=0;i<BOARD_SIZE-1; i++){
//        for(j=0;j<BOARD_SIZE-1; j++){
        for(i=0;i<=r; i++){
            for(j=0;j<=b; j++){
            if(cb->getPointData(i,j)==NONE){
                allValue[k][0] = i;
                allValue[k][1] = j;
                allValue[k][2] = blackValue[i][j] + whiteValue[i][j]+staticValue[i][j];
                k++;
            }
        }
    }
    sort(allValue,k);   //按价值降序排序

    size = k<SAMPLE_NUMBER? k:SAMPLE_NUMBER;
    //int valuablePositions[][] = new int[size][3];
    int **valuablePositions=new int*[size];
    for(int i=0;i<size;i++)
        valuablePositions[i]=new int[3];

    //将allValue中的前size个空位赋给bestPositions
    for(i=0;i<size;i++){
        valuablePositions[i][0]=allValue[i][0];
        valuablePositions[i][1]=allValue[i][1];
        valuablePositions[i][2]=allValue[i][2];
    }
    return valuablePositions;
}

int ComputerEvaluate::min(int depth,int alpha,int beta)
{
//    for(int i=0; i<=BOARD_SIZE-1; i++){
//        for(int j=0; j<=BOARD_SIZE-1; j++){//对棋盘的所有点循环
//            blackValue[i][j] = 0;
//            whiteValue[i][j] = 0;
//            if(cb->getPointData(i,j)==NONE){  //如果是空位，进行估值
//                for(int m=1; m<=4; m++){ //每个点的分值为四个方向分值之和
//                    blackValue[i][j] += evaluateValue( BLACK, i, j, m );
//                    whiteValue[i][j] += evaluateValue( WHITE, i, j, m );
//                }
//            }
//        }
//    }
    if(depth == 0){ //如果搜索到最底层，直接返回当前的估值。
        return evaluateGame();
    }

    getTheSpaceValues();

    int bestValue = LARGE_NUMBER;

    int value;
    int size;
    int** valuablePositions=getTheMostValuablePositions(size);

    for(int i=0; i<size; i++)
    {
        cb->setPointData(valuablePositions[i][0],valuablePositions[i][1], getComputerColor()==BLACK? WHITE:BLACK);

        int oldLeft= cb->left;
        int oldTop= cb->top;
        int oldRight= cb->right;
        int oldBottom = cb->bottom;
        if(cb->left>valuablePositions[i][0])     cb->left=valuablePositions[i][0];
        if(cb->top>valuablePositions[i][1])      cb->top = valuablePositions[i][1];
        if( cb->right<valuablePositions[i][0])     cb->right = valuablePositions[i][0];
        if( cb->bottom< valuablePositions[i][1]) cb->bottom=valuablePositions[i][1];

        value = max(depth-1, alpha,  beta);
        cb->setPointData(valuablePositions[i][0],valuablePositions[i][1],NONE);

        cb->left=oldLeft;
        cb->top = oldTop;
        cb->right= oldRight;
        cb->bottom=oldBottom;

        //如果某个分支的估值不大于alpha值（当前搜索的最大估值），则不再搜索剩下的分支
        if(value < beta){//beta保留当前的最小估值
            beta = value;
            if(alpha >= beta){//余下的分支不需要搜索了，直接返回
                return alpha;
            }
        }
    }
    return beta;


//    int i,j,k=0;
//    //allValue：保存每一空位的价值(列坐标，行坐标，价值）
//    //int [][] allValue = new int[(BOARD_SIZE-1+1)*(BOARD_SIZE-1+1)][3];
//    int **allValue=new int*[BOARD_SIZE*BOARD_SIZE];
//    for(int i=0;i<BOARD_SIZE*BOARD_SIZE;i++)
//        allValue[i]=new int[3];

//    for(i=0;i<BOARD_SIZE-1; i++){
//        for(j=0;j<BOARD_SIZE-1; j++){
//            if(cb->getPointData(i,j)==NONE){
//                allValue[k][0] = i;
//                allValue[k][1] = j;
//                allValue[k][2] = blackValue[i][j] + whiteValue[i][j]+staticValue[i][j];
//                k++;
//            }
//        }
//    }
//    sort(allValue,k);   //按价值降序排序

//    int size = k<SAMPLE_NUMBER? k:SAMPLE_NUMBER;
//    //int valuablePositions[][] = new int[size][3];
//    int **valuablePositions=new int*[size];
//    for(int i=0;i<size;i++)
//        valuablePositions[i]=new int[3];

//    //将allValue中的前size个空位赋给bestPositions
//    for(i=0;i<size;i++){
//        valuablePositions[i][0]=allValue[i][0];
//        valuablePositions[i][1]=allValue[i][1];
//        valuablePositions[i][2]=allValue[i][2];
//    }



    for(int i=0; i<size; i++){
        cb->setPointData(valuablePositions[i][0],valuablePositions[i][1], getComputerColor()==BLACK? WHITE:BLACK);
        value = max(depth-1,alpha,beta);
        cb->setPointData(valuablePositions[i][0],valuablePositions[i][1], NONE);
        if( value < bestValue) {
            bestValue = value;
        }
    }
    return bestValue;
}

int ComputerEvaluate::max(int depth,int alpha,int beta)
{
//    for(int i=0; i<=BOARD_SIZE-1; i++){
//        for(int j=0; j<=BOARD_SIZE-1; j++){//对棋盘的所有点循环
//            blackValue[i][j] = 0;
//            whiteValue[i][j] = 0;
//            if(cb->getPointData(i,j)==NONE){  //如果是空位，进行估值
//                for(int m=1; m<=4; m++){ //每个点的分值为四个方向分值之和
//                    blackValue[i][j] += evaluateValue( BLACK, i, j, m );
//                    whiteValue[i][j] += evaluateValue( WHITE, i, j, m );
//                }
//            }
//        }
//    }
    if(depth == 0){ //如果搜索到最底层，直接返回当前的估值。
        return evaluateGame();
    }

    getTheSpaceValues();

    int bestValue;
    bestValue = -LARGE_NUMBER;

    int value;
    int size;
    int** valuablePositions=getTheMostValuablePositions(size);

    for(int i=0; i<size; i++)
    {
        cb->setPointData(valuablePositions[i][0],valuablePositions[i][1], getComputerColor());

        int oldLeft= cb->left;
        int oldTop= cb->top;
        int oldRight= cb->right;
        int oldBottom = cb->bottom;
        if(cb->left>valuablePositions[i][0])  cb->left=valuablePositions[i][0];
        if(cb->top>valuablePositions[i][1])  cb->top = valuablePositions[i][1];
        if( cb->right<valuablePositions[i][0]) cb->right = valuablePositions[i][0];
        if( cb->bottom< valuablePositions[i][1]) cb->bottom=valuablePositions[i][1];

        value = min(depth-1,  alpha,  beta);
        cb->setPointData(valuablePositions[i][0],valuablePositions[i][1],NONE);

        cb->left=oldLeft;
        cb->top = oldTop;
        cb->right= oldRight;
        cb->bottom=oldBottom;

        if( value > alpha) {//如果某个分支的估值不小于beta值（最小估值），则不再搜索剩下的分支
            alpha = value;  //保留当前的最大估值
            if(alpha >=beta){
                return beta;
            }
        }
    }
    return alpha;

//    int i,j,k=0;
//    //allValue：保存每一空位的价值(列坐标，行坐标，价值）
//    //int [][] allValue = new int[(BOARD_SIZE-1+1)*(BOARD_SIZE-1+1)][3];
//    int **allValue=new int*[BOARD_SIZE*BOARD_SIZE];
//    for(int i=0;i<BOARD_SIZE*BOARD_SIZE;i++)
//        allValue[i]=new int[3];

//    for(i=0;i<BOARD_SIZE-1; i++){
//        for(j=0;j<BOARD_SIZE-1; j++){
//            if(cb->getPointData(i,j)==NONE){
//                allValue[k][0] = i;
//                allValue[k][1] = j;
//                allValue[k][2] = blackValue[i][j] + whiteValue[i][j]+staticValue[i][j];
//                k++;
//            }
//        }
//    }
//    sort(allValue,k);   //按价值降序排序

//    int size = k<SAMPLE_NUMBER? k:SAMPLE_NUMBER;
//    //int valuablePositions[][] = new int[size][3];
//    int **valuablePositions=new int*[size];
//    for(int i=0;i<size;i++)
//        valuablePositions[i]=new int[3];

//    //将allValue中的前size个空位赋给bestPositions
//    for(i=0;i<size;i++){
//        valuablePositions[i][0]=allValue[i][0];
//        valuablePositions[i][1]=allValue[i][1];
//        valuablePositions[i][2]=allValue[i][2];
//    }



    for(int i=0; i<size; i++){
        cb->setPointData(valuablePositions[i][0],valuablePositions[i][1], getComputerColor());

        value = min(depth-1,alpha,beta);
        cb->setPointData(valuablePositions[i][0],valuablePositions[i][1],NONE);
        if( value > bestValue) {
            bestValue = value;
        }
    }
    return bestValue;
}

void ComputerEvaluate::getTheSpaceValues()
{
    int l, t, r, b;
    l = (cb->left >2) ? cb->left-2:0;
    t = (cb->top >2) ? cb->top-2:0;
    r = (cb->right < BOARD_SIZE-2)? cb->right+2 : BOARD_SIZE-1;
    b = (cb->bottom < BOARD_SIZE-2)? cb->bottom+2 : BOARD_SIZE-1;
    for(int i=l; i<=r; i++)
    {
        for(int j=t; j<=b; j++)
        {//对棋盘的点进行循环
            blackValue[i][j] = 0;
            whiteValue[i][j] = 0;
            if(cb->getPointData(i,j)==NONE)
            {  //如果是空位，进行估值
                for(int m=1; m<=4; m++)
                { //每个点的分值为四个方向分值之和
                    blackValue[i][j] += evaluateValue( BLACK, i, j, m );
                    whiteValue[i][j] += evaluateValue( WHITE, i, j, m );
                }
            }
        }
    }
}


CHESSTYPE ComputerEvaluate::getComputerColor() const
{
    return computerColor;
}

void ComputerEvaluate::setComputerColor(const CHESSTYPE &value)
{
    computerColor = value;
}
