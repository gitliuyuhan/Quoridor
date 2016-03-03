/*======================================================
    > File Name: asynlog.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月03日 星期四 13时29分03秒
 =======================================================*/

#ifndef ASYNLOG_H
#define ASYNLOG_H

#include<iostream>
#include<string>
#include<vector>
#include<thread>
#include<mutex>
#include<condition_variable>
#include"buffer.hpp"
#include"timestamp.hpp"

namespace net
{
//日志级别
enum Loglevel
{
    ALL,//输出所有类型的日志
    DEBUG,//调试程序有关的信息
    INFO,//系统发生了哪些事情
    WARN,//警告，有潜在错误
    ERROR,//发生了错误，但不影响系统运行
    FATAL,//导致程序退出的错误
    OFF,//不记录日志
};

//异步日志类
class Asynlog
{
public:
    Asynlog();
    ~Asynlog();
    //将一行日志内容写入buffer中
    void writeLogToBuffer(const char* msg,int len,Loglevel level);
    //设置日志级别
    void setLogLevel(Loglevel level);
    //关闭日志
    void stop();
    void start();
private:
    //后台线程函数
    void threadFunc();
    //判断是否超过所设日志级别
    bool aboveLogLevel(Loglevel level);
    //组合成一条标准的日志消息
    std::string combineLogMsg(const char* log_line,int len,Loglevel level);
    //将日志级别转化为字符串
    std::string LogLevelToString(Loglevel level);
    //当前使用buffer
    std::unique_ptr<Buffer>                   cur_buffer; 
    //备用buffer
    std::unique_ptr<Buffer>                   next_buffer;
    //buffer表
    std::vector<std::unique_ptr<Buffer> >     buffer_list;
    //后台日志线程
    std::unique_ptr<std::thread>              log_thread;
    //互斥锁
    std::mutex                                mutex;
    //条件变量
    std::condition_variable                   cond;
    //时间戳
    Timestamp                                 time_stamp;
    //当前日志等级
    Loglevel                                  cur_level;
    //日志是否运行
    bool                                      running;
};

}

#endif //ASYNLOG_H
