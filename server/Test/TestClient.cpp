/*======================================================
    > File Name: TestClient.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月06日 星期日 14时20分31秒
 =======================================================*/

#include<iostream>
#include<unistd.h>
#include<thread>
#include"../tcpsocket.hpp"

void sendMsg(int sockfd)
{
    int    length=0;
    char   msg[1024];
    while(1)
    {
        std::cout<<"输入数据包内容"<<std::endl;
        std::cin>>msg;
        length = strlen(msg);
        std::cout<<::write(sockfd,&length,sizeof(length))<<std::endl;
        std::cout<<::write(sockfd,msg,length)<<std::endl;
    }
}

int main(int argc,char* argv[])
{
    if(argc <= 2)
    {
        std::cout<<"Usage: "<<argv[0]<<" ip port"<<std::endl;
        return 1;
    }
    net::TcpSocket     Socket;
    Socket.socketInit(argv[1],argv[2]);
    if(Socket.Connect() == 0)
    {
        std::cout<<"connect"<<std::endl;
        std::thread        t(sendMsg,Socket.getSockfd());
        t.join();
    }
    return 0;
}
