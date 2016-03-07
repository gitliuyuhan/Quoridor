
#pragma once

#include"messagefactory.hpp"
#include"message.hpp"
#include"./protobuf/game.SessionMsg.pb.h"

class LoginMessage : public Message
{
public:
    LoginMessage(){}
    ~LoginMessage(){}
    void processMsg(int sockfd,game::SessionMsg  &session_msg) override
    {
        std::cout<<session_msg.account()<<std::endl;
        std::cout<<session_msg.passwd()<<std::endl;
    }
};

class ChatMessage : public Message
{
public:
    ChatMessage(){}
    ~ChatMessage(){}
    void processMsg(int sockfd,game::SessionMsg  &session_msg) override
    {
        std::cout<<session_msg.msg()<<std::endl;
    }
};

REGISTER_MESSAGE(LoginMessage,1);
REGISTER_MESSAGE(ChatMessage,2);

