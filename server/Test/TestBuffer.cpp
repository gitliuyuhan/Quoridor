/*======================================================
    > File Name: buffer.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月06日 星期日 15时45分37秒
 =======================================================*/

#include<iostream>
#include<stdio.h>
#include<string.h>
#include"buffer.hpp"
using namespace std;

int main()
{
    char       s1[1024];
    char       s2[1024];
    net::Buffer     buf;
    buf.append("1234567890",10);
    buf.copySomeData(s1,4);
    std::cout<<s1<<std::endl;
    buf.append("asdfghjkl",9);
    buf.copySomeData(s1,5);
    std::cout<<s1<<std::endl;
    buf.readBuffer(s1,10);
    std::cout<<s1<<std::endl;
    buf.readBuffer(s2,9);
    std::cout<<s2<<std::endl;
    return 0;
}

