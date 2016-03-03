#ifndef BUFFER_H
#define BUFFER_H

#include<iostream>
#include<vector>

namespace net
{
class Buffer
{
public:
    Buffer():buffer(1024*1024) //初始化1M
    {
        write_index = 0;
    }
    ~Buffer()
    {}
    //可写字节数
    int writableSize()
    {
        return buffer.size()-write_index;
    }
    //已缓存数据大小
    int readableSize()
    {
        return write_index;
    }
    //往buffer写数据
    void append(const char* data,int len)
    {
        std::copy(data,data+len,buffer.begin()+write_index);
        write_index += len;
    }
    //重置buffer
    void resetBuffer()
    {
        write_index = 0;
    }
    //输出buffer中的数据
    void printBuffer()
    {
        for(auto i = buffer.begin();i != buffer.begin()+write_index;i++)
            std::cout<<*i;
    }
private:
    std::vector<char>     buffer;
    int                   write_index; //写数据位置
};

}

#endif //BUFFER_H
