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

EventLoop::EventLoop(const char* ip,const char* port):conn_table(100)
{
    //开启线程池
    thread_pool.start(5);
    //网络连接
    Socket.socketInit(ip,port);
    Socket.Bind();
    Socket.Listen();
    //epoll
    this->epollInit();
    this->addfd(Socket.getSockfd(),false);
    std::cout<<"等待处理网络连接......"<<std::endl;
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
                //建立连接
                int connfd = Socket.Accept();
                std::cout<<"有新用户建立连接"<<std::endl;
                //加入连接表
                this->addToConnTable(connfd);               
            }
            else if(events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR) )
            {
                //该客户端有异常
                this->removefd(sockfd);
                std::cout<<"客户端有异常"<<std::endl;
            }
            else if(events[i].events & EPOLLIN)
            {
                std::cout<<"触发了读事件"<<std::endl;
                //将该用户的请求任务加入线程池
                thread_pool.addTask(conn_table[sockfd]);
            }
        }

    }
}

EventLoop::~EventLoop()
{
    this->closeEventLoop();
}

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
void EventLoop::addfd(int fd,int oneshot = false)
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
//重置fd上的事件
void EventLoop::resetOneShot(int fd)
{
    epoll_event    event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLRDHUP | EPOLLONESHOT;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
}
//添加连接套接字
void EventLoop::addToConnTable(int connfd)
{
    std::shared_ptr<User>   user_ptr(new User(connfd));
    user_ptr->bindFunc(std::bind(&EventLoop::resetOneShot,this,std::placeholders::_1));
    conn_table[connfd] = user_ptr;
    //注册套接字事件，设置非阻塞
    this->addfd(connfd,true);
}
//从epoll移除套接字
void EventLoop::removefd(int fd)
{
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
    ::close(fd);
    conn_table[fd]->close();
}
//关闭
void EventLoop::closeEventLoop()
{
    ::close(epollfd);
    this->Socket.Close();
    thread_pool.stop();
}
