#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QEventLoop>
#include <QTimer>
#include <QPainter>
#include <QList>


enum GestureDirect
{
    LEFT = 0,
    RIGHT = 1,
    UP = 2,
    DOWN = 3
};

enum AnimationType
{
    MOVE = 0,
    APPEARANCE = 1
};

struct Animation
{
    AnimationType type;     // 动画类型
    GestureDirect direct;   // 方向
    QPointF startPos;       // 起始点坐标 出现动画仅仅使用这个坐标
    QPointF endPos;         // 终止点坐标 移动动画的终点坐标
    int digit;              // 数码
    int digit2;             // 第二数码 数码可能被合并
};

class GameWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = 0);

private:
    int board[4][4];
    int digitCount;
    int score;
    QPoint startPos;
    QList<Animation> animationList;

    qreal w, h;
    qreal ratioW, ratioH;
    qreal rX, rY;
    bool isAnimating;
    QTimer timer;

    void init2Block();

    bool checkGameOver();
    bool checkWin();

    int getBitCount(int);

    bool playAnimation(Animation&, QPainter&);
    bool drawAnimation(QPainter&);

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

signals:
    void GestureMove(GestureDirect);
    void ScoreInc(int);
    void GameOver();
    void win();

public slots:
    void onGestureMove(GestureDirect);
    void restart();
};

#endif // GAMEWIDGET_H
