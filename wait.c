/*======================================================
    > File Name: wait.c
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月08日 星期二 20时19分21秒
 =======================================================*/

#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

int main(int argc,char *argv[],char **environ)
{
    int      status = 0;
    pid_t    pid,ret;
    pid = fork();
    if(pid < 0)
    {
        printf("fork erro\n");
    }
    else if(pid==0)
    {
        printf("my process\n");
        execve("./server/server",argv,environ);
    }
    else
    {
        printf("This is parent process with pid of %d\n",getpid());
        while(pid>0)
        {
            ret = wait(&status);
            if(ret == -1)
            {
                printf("no have child process\n");
            }
            else
            {
//                if(WIFEXITED(status))
//                {
                    printf("this child process %d exit normally.\n",ret);
                    pid = fork();
                    if(pid < 0)
                    {
                        printf("fork erro\n");
                    }
                    else if(pid==0)
                    {
                        execve("./server/server",argv,environ);
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
