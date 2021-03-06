/*======================================================
    > File Name: eventloop.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月05日 星期六 13时03分23秒
 =======================================================*/

#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include<iostream>
#include<vector>
#include<memory>
#include<sys/epoll.h>
#include<fcntl.h>
#include"user.hpp"
#include"tcpsocket.hpp"
#include"threadpool.hpp"
#include"asynlog.h"

#define MAX_EVENT_NUMBER                 100

class EventLoop
{
public:
    EventLoop(const char* ip,const char* port);
    ~EventLoop();
    //初始化epoll
    void epollInit();
    //设置套接字非阻塞
    int setNonblocking(int fd);
    //注册epoll事件
    void addfd(int fd,int oneshot = false)
    {
        epoll_event      event;
        event.data.fd = fd;
        event.events = EPOLLIN | EPOLLRDHUP;
        //是否注册EPOLLONESHOT事件
        if(oneshot)
        {
            event.events |= EPOLLONESHOT;
        }
        epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
        setNonblocking(fd);
    }

private:
    //添加连接套接字
    void addToConnTable(int connfd);
    //从epoll移除套接字
    void removefd(int fd);
    //关闭eventloop
    void closeEventLoop();
    //重置fd事件
    void resetOneShot(int fd);
private:
    //用户连接表
    std::vector<std::shared_ptr<User> >           conn_table;
    int                                           epollfd;
    epoll_event                                   events[MAX_EVENT_NUMBER];
    net::TcpSocket                                Socket;
    net::ThreadPool<User>                         thread_pool;
    net::Asynlog                                  log;
};

#endif //EVENTLOOP_H
