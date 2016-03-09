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
#include<string>
#include"../tcpsocket.hpp"
#include"../protobuf/game.SessionMsg.pb.h"

void sendMsg(int sockfd)
{
    int        length=0;
    std::string     s;
    game::SessionMsg    msg;
    msg.set_type(2);
    msg.set_account("1234567890");
    msg.set_passwd("1234567890");
    msg.set_msg("12345678901234567890");
    msg.set_x(2000);
    while(1)
    {
    //    std::cin>>msg;
        sleep(1);
        msg.SerializePartialToString(&s);
        length = s.length();
        std::cout<<::write(sockfd,&length,sizeof(length))<<std::endl;
        std::cout<<::write(sockfd,s.c_str(),length)<<std::endl;
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
