
//保证文件只被编译一次
#pragma once

#include<map>
#include<string>
#include<functional>
#include<memory>
#include"message.hpp"

class MessageFactory
{
public:
    MessageFactory(){}
    MessageFactory(const MessageFactory&) = delete;
    MessageFactory(MessageFactory&&) = delete;
    //消息注册类
    template<typename T>
    class Register
    {
    public:
        Register(const int type)
        {
            MessageFactory::reg.emplace(type,[]{ return new T(); } );
        }
        template<typename... Args>
        Register(const int type,Args... args)
        {
            MessageFactory::reg.emplace(type,[&]{ return new T(args...); } );
        }
    };
    //消息生产
    static Message* produce(const int type)
    {
        if(reg.find(type) == reg.end())
        {
            throw std::invalid_argument("the message type is not exist!");
        }
        //调用function构造消息类
        return reg[type]();
    }
    static std::unique_ptr<Message> produce_unique(const int type)
    {
        //普通指针初始化智能指针
        return std::unique_ptr<Message>(produce(type));
    }
    static std::shared_ptr<Message> produce_shared(const int type)
    {
        return std::shared_ptr<Message>(produce(type));
    }
private:
    //消息注册表
    static std::map<int,std::function<Message*()> >        reg; 
};

//类静态成员初始化
//std::map<int,std::function<Message*()> >         MessageFactory::reg;

//构造Register类对象自动注册消息类
#define REGISTER_MESSAGE_NAME(T)       reg_msg_##T##_
#define REGISTER_MESSAGE(T,key,...)    static MessageFactory::Register<T>     REGISTER_MESSAGE_NAME(T)(key,##__VA_ARGS__)

