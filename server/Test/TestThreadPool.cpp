/*======================================================
    > File Name: TestThreadPool.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月04日 星期五 19时25分08秒
 =======================================================*/

#include<iostream>
#include<unistd.h>
#include"threadpool.hpp"

class Task
{
public:
    Task():mark(0)
    {
    }
    void process()
    {
        sleep(1);
        std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
        for(mark = 0;mark < 2;++mark)
            std::cout<<"No."<<mark<<"  hello,world"<<std::endl;
    }
private:
    int    mark;
};

net::ThreadPool<Task>    pool;

void PutTask()
{
    for(int i=0;i<10;i++)
    {
        std::shared_ptr<Task>    t(new Task);
        pool.addTask(t);
    }
}

int main()
{
    pool.start(4);
    std::thread    p(PutTask);
    char    c;
    std::cin>>c;
    pool.stop();
    p.join();
    return 0;
}
