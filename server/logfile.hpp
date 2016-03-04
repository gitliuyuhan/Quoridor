#ifndef LOGFILE_H
#define LOGFILE_H

#include<string>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

namespace net
{
class Logfile
{
public:
    Logfile(std::string file)
    {
        fd = open(file.c_str(),O_CREAT | O_RDWR | O_APPEND,0664);
        if(fd == -1)
            perror("logfile.hpp open");
    }
    ~Logfile()
    {
        close(fd);
    }
    //写日志
    void writeLog(char* msg,int len)
    {
        int ret = write(fd,msg,len);
        if(ret == -1)
            perror("logfile.hpp writeLog");
    }
private:
    int      fd;
};

}

#endif //LOGFILE_H
