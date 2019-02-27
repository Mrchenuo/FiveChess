#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mainToolBar->hide();
    ui->statusBar->hide();
    //禁止窗口最大化和修改窗口尺寸
    this->setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);
    this->setFixedSize(this->width(),this->height());

    QObject::connect(ui->BtnOnline, &QPushButton::clicked, this,&MainWindow::fnOnline);
    QObject::connect(ui->BtnManMachine, &QPushButton::clicked, this, &MainWindow::fnManMachine);
    QObject::connect(ui->BtnDoubleGame, &QPushButton::clicked, this, &MainWindow::fnDoubleGame);
    QObject::connect(ui->BtnHelp, &QPushButton::clicked, this, &MainWindow::fnHelp);
    QObject::connect(ui->BtnExit, &QPushButton::clicked, this, &MainWindow::fnExit);

//    //功能实现：当子窗口触发了关闭事件后，主窗口获取的子窗口关闭信号并还原主窗口
//    connect(battle,&Battle::ExitWin,this,&QWidget::show);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete battle;
    delete help;
}

void MainWindow::fnDoubleGame()
{
    battle = new Battle();
    battle->setGameMode(DOUBLEGAME);
    battle->show();
    this->hide();
    //功能实现：当子窗口触发了关闭事件后，主窗口获取的子窗口关闭信号并还原主窗口
    connect(battle,&Battle::ExitWin,this,&QWidget::show);
}

void MainWindow::fnHelp()
{
    help=new Help();
    help->show();
}

void MainWindow::fnExit()
{
    //destroy();
    exit(0);
}

void MainWindow::fnManMachine()
{

    battle = new Battle();
    battle->setGameMode(MANMACHINE);
    battle->setTurnYouPlay(true);//人机默认人开始下棋
    battle->show();
    this->hide();
    //功能实现：当子窗口触发了关闭事件后，主窗口获取的子窗口关闭信号并还原主窗口
    connect(battle,&Battle::ExitWin,this,&QWidget::show);
}

void MainWindow::fnOnline()
{
    battle = new Battle();
    battle->setGameMode(ONLINEGAME);
    battle->show();
    battle->showOnlineOption();
    this->hide();
    //功能实现：当子窗口触发了关闭事件后，主窗口获取的子窗口关闭信号并还原主窗口
    connect(battle,&Battle::ExitWin,this,&QWidget::show);
}


