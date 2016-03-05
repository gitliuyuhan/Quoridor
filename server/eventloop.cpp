/*======================================================
    > File Name: eventloop.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月05日 星期六 15时16分58秒
 =======================================================*/

#include<errno.h>
#include"tcpsocket.hpp"
#include"eventloop.h"

EventLoop::EventLoop(const char* ip,const char* port)
{
    Socket.socketInit(ip,port);
    Socket.Bind();
    Socket.Listen();

    this->eventInit();
    this->addfd();
    while(true)
    {
        int number = epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
        if((number<0)&&(errno != EINTR))
        {
            perror("epoll_wait fail");
            break;
        }
        //就绪的事件
        for(int i=0;i<number;i++)
        {
            int sockfd = events[i].data.fd;
            //有新连接
            if(sockfd == Socket.getSockfd())
            {
                int connfd = Socket.Accept();
                this->addToConnTable(connfd);               
            }
            else if(events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR) )
            {
                //该客户端有异常
                this->removefd(sockfd);
            }
            else if(events[i].events & EPOLLIN)
            {
            }
        }

    }
}

EventLoop::~EventLoop()
{}

//初始化
void EventLoop::epollInit()
{
    this->epollfd = epoll_create(5);
}
//设置非阻塞套接字
int EventLoop::setNonblocking(int fd)
{
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}
//注册事件
void EventLoop::addfd(int fd)
{
    epoll_event      event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLRDHUP;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    setNonblocking(fd);
}
//添加连接套接字
void EventLoop::addToConnTable(int connfd)
{
    std::shared_ptr<User>   user_ptr(new User(connfd));
    conn_table.push_back(user_ptr);
}
//从epoll移除套接字
void EventLoop::removefd(int fd)
{
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
    ::close(fd);
    conn_table[fd]->close();
}
