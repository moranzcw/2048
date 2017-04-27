#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    highScore = 0;

    QFile file("score.j");
    if (file.open(QIODevice::ReadOnly))
    {
        file.read((char *)&highScore, sizeof(highScore));
        file.close();
    }

    gameWidget = new GameWidget(this);
//    gameWidget->setGeometry(2, 200, 400, 400);
//    connect(gameWidget, SIGNAL(ScoreInc(int)), this, SLOT(onScoreInc(int)));
//    connect(gameWidget, SIGNAL(GameOver()), this, SLOT(onGameOver()));
//    connect(gameWidget, SIGNAL(win()), this, SLOT(onWin()));

    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(25);

    restartBtn = new QPushButton("Restart", this);
    restartBtn->setGeometry(100, 120, 200, 50);
    restartBtn->setFont(font);
    restartBtn->setStyleSheet(QString(BTNSTYLESHEET).arg(3).arg(15));
    connect(restartBtn, SIGNAL(clicked()), gameWidget, SLOT(restart()));

    highScoreLbl = new QLabel(QString("High Score:\n%1").arg(highScore),this);
    highScoreLbl->setGeometry(209, 20, 180, 70);
    highScoreLbl->setFont(font);
    highScoreLbl->setAlignment(Qt::AlignCenter);
    highScoreLbl->setStyleSheet(QString(LBLSTYLESHEET).arg(5).arg(20));

    scoreLbl = new QLabel("Score:\n0", this);
    scoreLbl->setGeometry(15, 20, 180, 70);
    scoreLbl->setFont(font);
    scoreLbl->setAlignment(Qt::AlignCenter);
    scoreLbl->setStyleSheet(QString(LBLSTYLESHEET).arg(5).arg(20));

    this->setFixedSize(480,760);
}

Widget::~Widget()
{
    delete restartBtn;
    delete scoreLbl;
    delete highScoreLbl;
    delete gameWidget;
}

void Widget::onScoreInc(int score)
{
    scoreLbl->setText(QString("Score:\n%1").arg(score));
    if (score > highScore)
    {
        highScore = score;
        highScoreLbl->setText(QString("High Score:\n%1").arg(highScore));

        QFile file("score.j");
        file.open(QIODevice::WriteOnly);
        file.write((char *)&highScore, sizeof(highScore));
        file.close();
    }
}

void Widget::onGameOver()
{
    QMessageBox::information(this, "GameOver", "You lost !");
}

void Widget::onWin()
{
    QMessageBox::information(this, "Congratulation", "You win !");
}

void Widget::resizeEvent(QResizeEvent *)
{
    ratioW = width() / 404.0;
    ratioH = height() / 606.0;

    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(25 * ratioH);
    restartBtn->setFont(font);
    highScoreLbl->setFont(font);
    scoreLbl->setFont(font);
    restartBtn->setStyleSheet(QString(BTNSTYLESHEET).arg(3 * ratioW).arg(15 * ratioW));
    highScoreLbl->setStyleSheet(QString(LBLSTYLESHEET).arg(5 * ratioW).arg(20 * ratioW));
    scoreLbl->setStyleSheet(QString(LBLSTYLESHEET).arg(5 * ratioW).arg(20 * ratioW));

// This part has changed.
    gameWidget->setGeometry(2 * ratioW, 200 * ratioH, 400 * ratioW, 400 * ratioH);
    restartBtn->setGeometry(15 * ratioW, 20 * ratioH, 180 * ratioW, 70 * ratioH);
    highScoreLbl->setGeometry(209 * ratioW, 20 * ratioH, 180 * ratioW, 70 * ratioH);
    scoreLbl->setGeometry(15 * ratioW, 110 * ratioH, 374 * ratioW, 70 * ratioH);
//..
}
