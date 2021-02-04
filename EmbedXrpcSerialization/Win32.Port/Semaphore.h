#pragma once
#include <mutex>
#include <condition_variable>
#include <string>
#include <cassert>
#include <chrono>
#include <iostream>

class Semaphore {
private:
    volatile int _count;                             //等待线程数量
    std::mutex _mutex;                      //互斥锁
    std::condition_variable _condition;     //条件变量
    std::string _name;                      //信号量名字

public:
    explicit Semaphore(std::string name, int value = 0) :
        _name(std::move(name)), _count(value) {}
    void release()
    {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _count += 1;
        }
        
        _condition.notify_one();
    }
    int Count()
    {
        int i = 0;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            i = _count;
        }
        return i;
    }
    bool wait(uint32_t ms = 0) {
        std::unique_lock<std::mutex> lock(_mutex);
        //std::cout<<_name+":"<<_count<<std::endl;
        if (ms == 0)
        {
            if (_count <= 0)
                return false;
            else
            {
                _count--;
                assert(_count >= 0);
                return true;
            }
        }
        if (_count <= 0) {                            // 资源不够
            std::cv_status status = _condition.wait_for(lock, std::chrono::milliseconds(ms));
            if (status == std::cv_status::timeout)   // 超时
            {
                //std::cout << _name + ":timeout" << std::endl;
                return false;
            }
            else
            {
                _count--;
                return true;
            }
        }
        else         
        {
            _count--;
            return true;
        }
    }
    bool peek(uint32_t ms = 0) {
        std::unique_lock<std::mutex> lock(_mutex);
        //std::cout<<_name+":"<<_count<<std::endl;
        if (ms == 0)
        {
            if (_count <= 0)
                return false;
            else
            {
                //_count--;
                return true;
            }
        }
        if (_count <= 0) {                            // 资源不够
            std::cv_status status = _condition.wait_for(lock, std::chrono::milliseconds(ms));
            if (status == std::cv_status::timeout)   // 超时
            {
                //std::cout << _name + ":timeout" << std::endl;
                return false;
            }
            else
            {
                //_count--;
                return true;
            }
        }
        else
        {
            //_count--;
            return true;
        }
    }
    /**
     * 返回这个信号量的名字
     * @return 名字
     */
    std::string name() {
        return _name;
    }
};