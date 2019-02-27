#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <battle.h>
#include "help.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void fnOnline();//联网模式
    void fnManMachine();//人机模式
    void fnDoubleGame();//双人博弈
    void fnHelp();//帮助
    void fnExit();//退出
private:
    Ui::MainWindow *ui;
    Battle* battle;
    Help* help;
};

#endif // MAINWINDOW_H
