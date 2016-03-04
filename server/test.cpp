/*======================================================
    > File Name: test.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月01日 星期二 22时04分38秒
 =======================================================*/

#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<thread>
#include<unistd.h>
#include<vector>
#include<iterator>
#include"timestamp.hpp"
#include"buffer.hpp"
#include"logfile.hpp"
#include"asynlog.h"

net::Asynlog     log;
std::string      msg("hello,world! abcdefghijk 1234567890");

class Test
{
public:
    Test()
        :t(&Test::fun,this)
    {
        t.join();
    }
    ~Test()
    {}
public:
    void fun()
    {
        for(int i=0;i<4000;i++)
        {
            log.writeLogToBuffer(msg.c_str(),msg.length(),net::WARN);
        }
    }
private:
    std::thread    t;
};


int main()
{
    log.start();
    sleep(1);
    std::cout<<"start"<<std::endl;
    net::Timestamp  tm1;
    tm1.getNowTime();
    for(int i=0;i<100;i++)
    {
        Test  *test = new Test();
    }
    net::Timestamp   tm2;
    tm2.getNowTime();
    std::cout<<tm2.timeDiff(tm1)<<std::endl;
    log.stop();
 
    return 0;
}

