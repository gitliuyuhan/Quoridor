/*======================================================
    > File Name: main.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月06日 星期日 13时35分57秒
 =======================================================*/

#include<iostream>
#include"eventloop.h"

int main(int argc,char* argv[])
{
    if(argc <= 2)
    {
        printf("usage: %s ip_address port\n",basename(argv[0]));
        return 1;
    }
    EventLoop           loop(argv[1],argv[2]);
    return 0;
}
