#include "help.h"
#include "ui_help.h"

Help::Help(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Help)
{
    ui->setupUi(this);
    ui->statusbar->hide();
    //ui->menubar->hide();
    //禁止窗口最大化和修改窗口尺寸
    this->setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);
    this->setFixedSize(this->width(),this->height());

    //ui->frame->hide();
    QObject::connect(ui->actionluozi, &QAction::triggered, this,Help::fnLuoZi);
    QObject::connect(ui->actionChongFour, &QAction::triggered, this,Help::fnChongFour);
    QObject::connect(ui->actionHuoFour, &QAction::triggered, this,Help::fnHuoFour);
    QObject::connect(ui->actionHuoThree, &QAction::triggered, this,Help::fnHuoThree);
    QObject::connect(ui->actionMianTwo, &QAction::triggered, this,Help::fnHuoTwo);
    QObject::connect(ui->actionMianThree, &QAction::triggered, this,Help::fnMianThree);
    QObject::connect(ui->actionwuzilianzhu, &QAction::triggered, this,Help::fnLianWu);
    QObject::connect(ui->menuAbout,  &QMenu::aboutToHide, this,Help::fnAbout);
    QObject::connect(ui->menuAbout,  &QMenu::aboutToShow, this,Help::fnAbout);
}

Help::~Help()
{
    delete ui;
}

void Help::fnLuoZi()
{
   // ui->frame->show();
    ui->frame->setStyleSheet(QStringLiteral("border-image: url(:/images/res/luozi.png);"));
}

void Help::fnLianWu()
{
    //ui->frame->show();
    ui->frame->setStyleSheet(QStringLiteral("border-image: url(:/images/res/wuzilianzhu.png);"));
}

void Help::fnHuoFour()
{
    //ui->frame->show();
    ui->frame->setStyleSheet(QStringLiteral("border-image: url(:/images/res/huofour.png);"));
}

void Help::fnChongFour()
{
    //ui->frame->show();
    ui->frame->setStyleSheet(QStringLiteral("border-image: url(:/images/res/chongfour.png);"));
}

void Help::fnHuoThree()
{
    //ui->frame->show();
    ui->frame->setStyleSheet(QStringLiteral("border-image: url(:/images/res/huothree.png);"));
}

void Help::fnMianThree()
{
    //ui->frame->show();
    ui->frame->setStyleSheet(QStringLiteral("border-image: url(:/images/res/mianthree.png);"));
}

void Help::fnHuoTwo()
{
    //ui->frame->show();
    ui->frame->setStyleSheet(QStringLiteral("border-image: url(:/images/res/huotwo.png);"));
}

void Help::fnAbout()
{
    ui->frame->setStyleSheet(QStringLiteral("border-image: url(:/images/res/tim.png);"));
}
