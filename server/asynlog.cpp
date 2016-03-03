/*======================================================
    > File Name: asynlog.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月03日 星期四 14时13分34秒
 =======================================================*/

#include<iostream>
#include<stdio.h>
#include<string.h>
#include<functional>
#include<pthread.h>
#include"asynlog.h"
#include"logfile.hpp"

using namespace net;

Asynlog::Asynlog()
    :running(false),
    cur_level(ALL)
{
}

//开启日志服务
void Asynlog::start()
{
    cur_buffer.reset(new Buffer());
    next_buffer.reset(new Buffer());
    running = true;
    std::cout<<"日志服务正在启动......"<<std::endl;
    log_thread.reset(new std::thread(&Asynlog::threadFunc,this));
}

Asynlog::~Asynlog()
{
    stop();
}

//退出日志系统
void Asynlog::stop()
{
    if(running)
    {
        running = false;
        //等待日志线程退出
        log_thread->join();
        std::cout<<"日志系统已关闭"<<std::endl;
    }
}

//设置日志级别
void Asynlog::setLogLevel(Loglevel level)
{
    this->cur_level = level;
}

//判断是否超过所设日志级别
bool Asynlog::aboveLogLevel(Loglevel level)
{
    return level >= this->cur_level;
}

//将日志级别转化为字符串
std::string Asynlog::LogLevelToString(Loglevel level)
{
    std::string    s;    
    switch(level)
    {
        case OFF:
            s = "OFF";  break;
        case FATAL:
            s = "FATAL"; break;
        case ERROR:
            s = "ERROR"; break;
        case WARN:
            s = "WARN"; break;
        case INFO:
            s = "INFO"; break;
        case DEBUG:
            s = "DEBUG"; break;
        case ALL:
            s = "ALL"; break;
        default:
            s = "UNKNOWN";
    }
    return s;
}

//组成一条标准的日志消息
std::string Asynlog::combineLogMsg(const char* msg,int len,Loglevel level)
{
    char          s[20];
    std::string   str;
    //获取时间
    Timestamp::timeToString(s);
    str += s;
    //线程id
    snprintf(s,sizeof(s),"%lu",pthread_self());
    str = str + " " + s + " " + LogLevelToString(level) + " " + msg + "\n"; 
    return str;
}

//向buffer中送入日志消息
void Asynlog::writeLogToBuffer(const char* msg,int len,Loglevel level)
{
    if(running != true)
    {
        std::cout<<"日志服务未开启"<<std::endl;
        return;
    }
    if(!aboveLogLevel(level))
    {
        return;
    }
    //日志行
    std::string log_msg(std::move(combineLogMsg(msg,len,level)));
    //加锁
    std::unique_lock<std::mutex>      locker(mutex);
    //当前buffer还有足够空间,直接写入buffer
    if(cur_buffer->writableSize() >= log_msg.length())
    {
        cur_buffer->append(log_msg.c_str(),log_msg.length());
    }
    else
    {
        //将满的buffer移到列表中
        this->buffer_list.push_back(std::move(cur_buffer));
        if(next_buffer)
        {
            //如果预备buffer空闲，则交给cur_buffer使用
            cur_buffer = std::move(next_buffer);
        }
        else
        {
            //没有可以使用的buffer了,新建一个
            cur_buffer.reset(new Buffer());
        }
        cur_buffer->append(log_msg.c_str(),log_msg.length());
        //buffer满了，唤醒后台线程
        this->cond.notify_one();
    }
}

void Asynlog::threadFunc()
{
    std::cout<<"日志后台线程启动......"<<std::endl;
    //用来写日志的buffer表
    std::vector<std::unique_ptr<Buffer> >       buffer_list_write;
    Logfile       output("./running.log");
    //预留16个满buffer空间
    buffer_list_write.reserve(16);
    while(running)
    {
        {
            std::unique_lock<std::mutex>    locker(mutex);
            //后台线程只有一个，不用考虑虚假唤醒问题
            if(buffer_list.empty())
            {
                //开始计时
                Timestamp      tm1;
                tm1.getNowTime();
                //列表为空，等待新的被写满buffer，或3秒刷一次
                cond.wait_for(locker,std::chrono::seconds(3));
                //结束计时
                Timestamp      tm2;
                tm2.getNowTime();
                //判断是否是因为超时唤醒
                if(tm2.timeDiff(tm1)-3 >= 0 && cur_buffer->readableSize())
                {
                    buffer_list.push_back(std::move(cur_buffer));
                    cur_buffer.reset(new Buffer());
                }
            }
            //交换两个容器
            buffer_list_write = std::move(buffer_list);
            if(!next_buffer)
            {
                next_buffer.reset(new Buffer());
            }
        }
        //将日志写入文件
        for(int i = 0;i < buffer_list_write.size();i++)
        {
            output.writeLog(buffer_list_write[i]->begin(),buffer_list_write[i]->readableSize());
        }
        buffer_list_write.clear();
    }
}
