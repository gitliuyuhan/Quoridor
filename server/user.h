/*======================================================
    > File Name: user.h
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月05日 星期六 13时06分47秒
 =======================================================*/

#ifndef USER_H
#define USER_H

class User
{
public:
    User(int fd=-1):opponent_fd(-1)
    {
        self_fd = fd;
    }
    ~User()
    {
        close();
    }
    //任务处理
    void process()
    {}
    //关闭套接字
    void close()
    {
        self_fd = -1;
    }
private:
    //对方的sockfd
    int       opponent_fd;
    //自己的sockfd
    int       self_fd;
    //静态成员，匹配队列，每个元素是一个房间，等待其他人匹配
    //静态成员，map,所有在线用户状态
};

#endif //USER_H

