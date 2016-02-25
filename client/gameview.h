/*======================================================
    > File Name: gameview.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月11日 星期四 16时15分23秒
 =======================================================*/

#ifndef GAMEVIEW_H  
#define GAMEVIEW_H  

#include<QWidget>  
#include<QPainter>  
#include<QMouseEvent>

class GameView : public QWidget  
{  
    Q_OBJECT  
public:  
    GameView(QWidget *parent = 0);  
    ~GameView();  
private slots:  

private:  
    //绘图
    virtual void paintEvent(QPaintEvent *);
    //鼠标点击事件
    virtual void mousePressEvent(QMouseEvent *);
private:
    //操作类型
    int            operation_type;
};  

#endif // GAMEVIEW_H  
