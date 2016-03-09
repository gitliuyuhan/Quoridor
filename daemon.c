/*======================================================
    > File Name: daemon.c
    > Author: MiaoShuai
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月08日 星期二 19时38分42秒
 =======================================================*/
//#include<iostream>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/param.h>
#include<sys/stat.h>
#include<syslog.h>
int watch_server(char *argv[],char **environ)
{
    int      status = 0;
    pid_t    pid,ret;
    pid = fork();
    if(pid < 0)
    {
        syslog(LOG_INFO,"fork erro %s",argv[0]);
    }
    else if(pid==0)
    {
        syslog(LOG_INFO,"my process %s %s %s",argv[0],argv[1],argv[2]);
        execve("/home/lyh/GitHub/Quoridor/server/server",argv,environ);
    }
    else
    {
        syslog(LOG_INFO,"This is parent process with pid of %d",getpid());
        while(pid>0)
        {
            ret = wait(&status);
            if(ret == -1)
            {
                syslog(LOG_INFO,"no have child process");
                break;
            }
            else
            {
//                if(WIFEXITED(status))
//                {
                    syslog(LOG_INFO,"this child process %d exit normally",ret);
                    pid = fork();
                    if(pid < 0)
                    {
                        syslog(LOG_INFO,"fork erro");
                    }
                    else if(pid==0)
                    {
                        syslog(LOG_INFO,"rebuild my process %s %s %s",argv[0],argv[1],argv[2]);
                        execve("/home/lyh/GitHub/Quoridor/server/server",argv,environ);
                    }
 //               }
 //               else
 //               {
 //                   printf("this child process %d exit abnormally.\n",ret);
 //               }
            }
        }
    }
    return 0;
}
int init_daemon(char *argv[],char **environ)
{
    int pid;
    int i;
    
    //忽略终端的I/O信号
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);
    signal(SIGHUP,SIG_IGN);
    
    pid = fork();
    if(pid > 0)
    {   
        exit(0);
    }
    else if(-1 == pid)
    {
        return -1;
    }

    //将自己脱离init并提升为会话组长
    setsid();
    pid = fork();
    if(pid > 0)
    {
        
        exit(0);
    }
    else if(-1 == pid)
    {
        return -1;
    }
    
    //关闭所有从父进程那继承来的套接字描述符
    for(i=0; i< NOFILE; close(i++));

    //改变文件目录使得进程不与任何文件系统关联
    chdir("/");

    //将文件屏蔽字设为０
    umask(0);
    syslog(LOG_INFO,"守护进程 %d 创建完毕......",getpid());
    watch_server(argv,environ);
    return 0;
}

int main(int argc,char *argv[],char **environ)
{
    openlog("lyh",LOG_CONS|LOG_PID,0);
    syslog(LOG_INFO,"开始创建守护进程......");
    init_daemon(argv,environ);
//    watch_server(argv,environ);
    closelog();
    return 0;
}


