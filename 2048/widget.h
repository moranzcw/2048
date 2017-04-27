#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include "GameWidget.h"

#define LBLSTYLESHEET "QLabel {color: #000000;background: #B0E0E6;border-radius: %1px;}"
#define BTNSTYLESHEET "QPushButton {color: #FFFFFF;background: #FA8072;border-radius: %1px;} QPushButton:pressed{color: white;background: orange;border-radius: %1px;}"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    QPushButton *restartBtn;
    QLabel *scoreLbl;
    QLabel *highScoreLbl;
    GameWidget *gameWidget;
    qreal ratioW, ratioH;
    int highScore;

protected:
    void resizeEvent(QResizeEvent *);

public slots:
    void onScoreInc(int);
    void onGameOver();
    void onWin();
};

#endif // WIDGET_H
