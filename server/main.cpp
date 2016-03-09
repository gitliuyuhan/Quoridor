/*======================================================
    > File Name: main.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月06日 星期日 13时35分57秒
 =======================================================*/

#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include"eventloop.h"

int main(int argc,char* argv[])
{
    if(argc <= 2)
    {
        printf("usage: %s ip_address port\n",basename(argv[0]));
        return 1;
    }
    std::cout<<"当前进程id = "<<getpid()<<std::endl;
    EventLoop           loop(argv[1],argv[2]);
    return 0;
}
