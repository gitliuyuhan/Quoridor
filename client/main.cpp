/*======================================================
    > File Name: main.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月12日 星期五 15时51分13秒
 =======================================================*/

#include"gameview.h"

#include<QApplication>

int main(int argc,char* argv[])
{
    QApplication app(argc,argv);
    GameView  w;
    w.show();
    return app.exec();
}
