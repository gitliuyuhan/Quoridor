
//请求消息基类
#ifndef Message_H
#define Message_H

#include"./protobuf/game.SessionMsg.pb.h"

class Message
{
public:
    Message()
    {}
    virtual ~Message()
    {}
    //处理消息
    virtual void processMsg(int sockfd,game::SessionMsg& session_msg)
    {}
};

#endif //Message_H
