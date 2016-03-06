#ifndef THREADPOOL_H
#define THREADPOOL_H

#include<list>
#include<thread>
#include<functional>
#include<memory>
#include<atomic>
#include"syncqueue.hpp"

namespace net
{
//线程池
template<typename Task>
class ThreadPool
{
#define MAX_TASK_COUNT   5   //容器任务数量 

public:
    ThreadPool():sync_queue(MAX_TASK_COUNT)
    {
    }
    ~ThreadPool()
    {
        //如果没有显示停止线程池时
        stop();
    }
    //停止线程池
    void stop()
    {
        std::call_once(flag,[this]{ stopThreadGroup(); } );
    }
    //开启线程池
    void start(int num_threads = 100)
    {
        running = true;
        std::cout<<"线程池正在开启......"<<std::endl;
        //创建线程组
        for(int i=0;i<num_threads;++i)
        {
            //为线程绑定工作函数
            thread_group.push_back(std::make_shared<std::thread>(&ThreadPool::run,this));
        }
    }
    //往线程池添加任务
    void addTask(std::shared_ptr<Task>  task)
    {
        sync_queue.Put(task);
    }
private:
    //线程工作函数
    void run()
    {
        auto thread_id = std::this_thread::get_id();
        std::cout<<"同步线程"<<thread_id<<"正在运行"<<std::endl;
        while(running)
        {
            //取任务执行
            std::shared_ptr<Task>    task;
            sync_queue.Take(task);
            //判断线程池是否已停止
            if(!running)
                return;
            task->process();
        }
    }
    //停止线程池中的所有线程
    void stopThreadGroup()
    {
        //停止正在操作同步队列的线程
        sync_queue.stop();
        //准备停止线程池
        running = false;
        for(auto t : thread_group)
        {
            if(t)
                t->join();
        }
        thread_group.clear();
        std::cout<<"线程池已停止"<<std::endl;
    }
private:
    //线程组
    std::list<std::shared_ptr<std::thread> >     thread_group;
    //同步队列
    SyncQueue<std::shared_ptr<Task> >            sync_queue;
    //原子变量，线程池停止标志
    std::atomic_bool                             running;
    //保证多线程下函数只调用一次标志
    std::once_flag                               flag;
};

}

#endif //THREADPOOL_H
