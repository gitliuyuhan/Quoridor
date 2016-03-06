#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

namespace net
{
//封装socket
class TcpSocket
{
public:
    TcpSocket()
    {}
    ~TcpSocket()
    {}
    //获取sockfd
    int getSockfd()
    {
        return this->sockfd;
    }
    //socket初始化
    void socketInit(const char* ip,const char* port_)
    {
        int   port = atoi(port_);
        bzero(&server_addr,sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        inet_pton(AF_INET,ip,&server_addr.sin_addr);
        server_addr.sin_port = htons(port);
        sockfd = socket(PF_INET,SOCK_STREAM,0);
        if(sockfd < 0)
            perror("socket");
    }
    //连接服务器
    int Connect()
    {
        int ret = connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
        if(ret == -1)
            perror("connect");
        return ret;
    }
    //绑定
    int Bind()
    {
        int ret = bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
        if(ret == -1)
            perror("bind");
        return ret;
    }
    //监听
    int Listen(int backlog = 10)
    {
        int ret = listen(sockfd,backlog);
        if(ret == -1)
            perror("listen");
        return ret;
    }
    //接受连接
    int Accept()
    {
        socklen_t        client_addr_length = sizeof(client_addr);
        int connfd = accept(sockfd,(struct sockaddr*)&client_addr,&client_addr_length);
        if(connfd < 0)
            perror("accept");
        return connfd;
    }
    //关闭socket
    int Close()
    {
        int ret = close(sockfd);
        if(ret == -1)
            perror("close socket");
        return ret;
    }
private:
    int                       sockfd;
    struct sockaddr_in        server_addr;
    struct sockaddr_in        client_addr;
};

}

#endif //TCPSOCKET_H
