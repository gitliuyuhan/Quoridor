#ifndef SYNCQUEUE_H
#define SYNCQUEUE_H

#include<list>
#include<mutex>
#include<condition_variable>
#include<iostream>

namespace net
{
//同步队列
template<typename T>
class SyncQueue
{
public:
    SyncQueue(int size)
        : max_size(size),
          need_stop(false)
    {
    }
    ~SyncQueue()
    {
    }
    //停止同步队列
    void stop()
    {
        {
            std::lock_guard<std::mutex>     locker(mutex_);
            this->need_stop = true;
        }
        not_full.notify_all();
        not_empty.notify_all();
    }
    //获取同步队列大小
    size_t size()
    {
        std::lock_guard<std::mutex>    locker(mutex_);
        return queue_.size();
    }
    //添加任务
    void Put(T&  x)
    {
        std::unique_lock<std::mutex>   locker(mutex_);
        //缓冲区满了则等待
        not_full.wait(locker,[this]{ return need_stop||!isFull(); } );
        if(need_stop)
            return;
        queue_.push_back(x);
        not_empty.notify_one();
    }
    //取一个任务
    void Take(T& x)
    {
        std::unique_lock<std::mutex>    locker(mutex_);
        //缓冲区为空则等待
        not_empty.wait(locker,[this]{ return need_stop || !isEmpty(); });
        if(need_stop)
            return;
        x = queue_.front();
        queue_.pop_front();
        not_full.notify_one();
    }
private:
    //队列为空返回true
    bool isEmpty()
    {
//        std::lock_guard<std::mutex>   locker(mutex_);
        bool b = queue_.empty();
//        if(b)
//            std::cout<<"缓冲区空了，等待新任务......"<<std::endl;
        return b;
    }
    //队列满了返回true
    bool isFull()
    {
//        std::lock_guard<std::mutex>   locker(mutex_);
        bool b = queue_.size() == max_size;
//        if(b)
//            std::cout<<"缓冲区满了，等待取任务......"<<std::endl;
        return b;
    }

private:
    //队列缓冲区
    std::list<T>                    queue_;
    //互斥锁
    std::mutex                      mutex_;
    //不为空的条件变量
    std::condition_variable         not_empty;
    //不为满的条件变量
    std::condition_variable         not_full;
    //同步队列的最大容量
    int                             max_size;
    //停止标志
    bool                            need_stop;
};

}


#endif //SYNCQUEUE_H
