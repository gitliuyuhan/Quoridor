/*======================================================
    > File Name: messagefactory.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月07日 星期一 15时27分29秒
 =======================================================*/

#include"messagefactory.hpp"

//类静态成员初始化
std::map<int,std::function<Message*()> >         MessageFactory::reg;

