#ifndef BUFFER_H
#define BUFFER_H

#include<iostream>
#include<vector>

namespace net
{
class Buffer
{
public:
    Buffer(int size=1024*1024) //初始化1M
    {
        buffer.resize(size);
        write_index = 0;
        read_index = 0;
    }
    ~Buffer()
    {}
    //可写字节数
    int writableSize()
    {
        return buffer.size()-write_index;
    }
    //可读数据大小
    int readableSize()
    {
        return write_index-read_index;
    }
    //往buffer写数据
    void append(const char* data,int len)
    {
        std::copy(data,data+len,buffer.begin()+write_index);
        write_index += len;
    }
    //将未读数据移动到起始位置
    void moveToHead()
    {
        std::copy(getReadPtr(),getReadPtr()+readableSize(),buffer.begin());
        read_index = 0;
        write_index = 0+readableSize();
    }

    //读数据
    template<typename T>
    void readBuffer(T* p,int len)
    {
        copySomeData(p,len);
        read_index = read_index+len;
    }
    //向外拷贝一些数据
    void copySomeData(T* p,int len)
    {
        std::copy(getReadPtr(),getReadPtr()+len,p);
    }
    //重置buffer
    void resetBuffer()
    {
        write_index = 0;
        read_index = 0;
    }
    //输出buffer中的数据
    void printBuffer()
    {
        for(auto i = buffer.begin();i != buffer.begin()+write_index;i++)
            std::cout<<*i;
    }
    //获取读指针
    void getReadPtr()
    {
        return this->begin() + read_index;
    }
    //头指针
    char* begin()
    {
        return &(*buffer.begin());
    }
private:
    std::vector<char>     buffer;
    int                   write_index; //写数据位置
    int                   read_index; //读数据位置
};

}

#endif //BUFFER_H
