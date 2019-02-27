#ifndef HELP_H
#define HELP_H

#include <QMainWindow>

namespace Ui {
class Help;
}

class Help : public QMainWindow
{
    Q_OBJECT

public:
    explicit Help(QWidget *parent = 0);
    ~Help();

    void fnLuoZi();
    void fnLianWu();
    void fnHuoFour();
    void fnChongFour();
    void fnHuoThree();
    void fnMianThree();
    void fnHuoTwo();
    void fnAbout();
private:
    Ui::Help *ui;
};

#endif // HELP_H
