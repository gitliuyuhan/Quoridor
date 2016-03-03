#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include<sys/time.h>
#include<iostream>
#include<string>
#include<string.h>

namespace net
{
class Timestamp
{
public:
    Timestamp()
    {}
    ~Timestamp()
    {}
    //获取当前时间
    void getNowTime()
    {
        gettimeofday(&time_,NULL);
    }
    //将当前时间转化为日期格式的字符串
    static void timeToString(char* t_str)
    {
        time_t    t = time(NULL);
        struct tm tmp;
        localtime_r(&t,&tmp);
        snprintf(t_str,20,"%04d%02d%02d %02d:%02d:%02d",
                tmp.tm_year+1900,tmp.tm_mon+1,tmp.tm_mday,
                tmp.tm_hour,tmp.tm_min,tmp.tm_sec);
    }
    //将当前时间转换为适合文件命名的字符串
    static void timeToFString(char* t_str)
    {
        time_t   t = time(NULL);
        struct tm tmp;
        localtime_r(&t,&tmp);
        snprintf(t_str,20,"%04d%02d%02d%02d%02d%02d",
                tmp.tm_year+1990,tmp.tm_mon+1,tmp.tm_mday,
                tmp.tm_hour,tmp.tm_min,tmp.tm_sec);
    }
    //转化为秒数
    double timeToSec()
    {
        return time_.tv_sec + time_.tv_usec/1000000.0;
    }
    //获取时间差，返回值秒
    double timeDiff(Timestamp& t)
    {
        return timeToSec()-t.timeToSec();
    }
private:
    //当前时间，由秒和微妙组成
    struct timeval time_;
};

}

#endif //TIMESTAMP_H
