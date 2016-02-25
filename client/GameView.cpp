/*======================================================
    > File Name: GameView.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月11日 星期四 16时19分27秒
 =======================================================*/

#include "gameview.h"

#define FOOTHOLD  40  //落脚点的大小
#define DITCH     20  //沟槽的宽度

#define INIT      0   //初始棋盘
#define HORIZON   1   //水平阻挡

GameView::GameView(QWidget *parent) : QWidget(parent)
{
    this->operation_type = INIT;
}

GameView::~GameView()
{}

//画棋盘
void GameView::paintEvent(QPaintEvent*)
{
    QPainter paint(this);
    QImage   image;

    switch(operation_type)
    {
        case INIT:
            image.load(":/image/1.png");
            for(int x=0;x<9;x++)
            {
                for(int y=0;y<9;y++)
                {
                    paint.drawImage(
                            x*(FOOTHOLD+DITCH),
                            y*(FOOTHOLD+DITCH),
                            image,
                            0,
                            0,
                            FOOTHOLD,
                            FOOTHOLD
                            );
                }
            }
            break;
        case HORIZON:
            image.load(":/image/2.png");
            paint.drawImage(
                    2*(FOOTHOLD+DITCH),
                    2*(FOOTHOLD+DITCH)+FOOTHOLD,
                    image,
                    0,
                    0,
                    2*FOOTHOLD+DITCH,
                    DITCH
                    );
            break;
    }
}

//点击事件
void GameView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        this->operation_type = HORIZON;
        this->update(
                2*(FOOTHOLD+DITCH),
                2*(FOOTHOLD+DITCH)+FOOTHOLD,
                2*FOOTHOLD+DITCH,
                DITCH
                );
    }
}
