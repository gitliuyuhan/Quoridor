/*======================================================
    > File Name: user.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月05日 星期六 13时06分47秒
 =======================================================*/

#ifndef USER_H
#define USER_H

#include<unistd.h>
#include<thread>
#include<stdio.h>
#include<strings.h>
#include"buffer.hpp"
#include"message.hpp"
#include"allmessage.hpp"
#include"messagefactory.hpp"
#include"./protobuf/game.SessionMsg.pb.h"

class User
{
public:
    User(int fd=-1):opponent_fd(-1),user_buffer(1024)
    {
        self_fd = fd;
    }
    ~User()
    {
        close();
    }
    //任务处理
    void process()
    {
        std::cout<<"thread_id:"<<std::this_thread::get_id()<<std::endl;
        if(readMsg() )
        {
            while(getMsgFromBuffer() ){}
        }
    }
    //读数据
    int readMsg()
    {
        char        buf[1024];
        int ret = ::read(self_fd,buf,sizeof(buf));
        if(ret < 0)
        {
            perror("read fail");
        }
        if(ret > 0)
        {
            user_buffer.append(buf,ret);
        }
        //重新注册fd
        reset_oneshot(self_fd);
        return ret;
    }
    //从buffer取数据
    bool getMsgFromBuffer()
    {
        //消息长度字段
        int                length=0;
        if(user_buffer.readableSize() <= sizeof(length))
            return false;
        user_buffer.copySomeData((char*)&length,sizeof(length));
        //判断buffer中够不够一个完整的数据包
        if(length+sizeof(length) > user_buffer.readableSize())
            return false;
        //读一个数据包出来
        char               session_msg[1024];
        //清空
        ::bzero(session_msg,1024);
        user_buffer.readBuffer((char*)&length,sizeof(length));//长度
        user_buffer.readBuffer(session_msg,length); //实际数据
        //处理数据
        parseMsg(session_msg);
        return true;
    }
    //处理数据
    void parseMsg(std::string msg)
    {
        game::SessionMsg      session_msg;
        if(session_msg.ParseFromString(msg))
        {
            auto msg_ptr = MessageFactory::produce_unique(session_msg.type());
            msg_ptr->processMsg(self_fd,session_msg);
        }
        else
        {
            std::cout<<"消息解析错误"<<std::endl;
        }
//        std::cout<<msg<<std::endl;
    }
    //关闭套接字
    void close()
    {
        self_fd = -1;
    }
    //function
    void bindFunc(std::function<void(int)>  func)
    {
        reset_oneshot = func;
    }
private:
    //对方的sockfd
    int                           opponent_fd;
    //自己的sockfd
    int                           self_fd;
    //用户缓冲区
    net::Buffer                   user_buffer;
    //静态成员，匹配队列，每个元素是一个房间，等待其他人匹配
    //静态成员，map,所有在线用户状态
    //function,用来重置fd
    std::function<void(int)>      reset_oneshot;
};

#endif //USER_H

