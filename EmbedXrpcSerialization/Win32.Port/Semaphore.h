#pragma once
#include <mutex>
#include <condition_variable>
#include <string>
#include <cassert>
#include <chrono>
#include <iostream>

class Semaphore {
private:
    int _count;                             //�ȴ��߳�����
    std::mutex _mutex;                      //������
    std::condition_variable _condition;     //��������
    std::string _name;                      //�ź�������

public:
    explicit Semaphore(std::string name, int value = 0) :
        _name(std::move(name)), _count(value) {}
    void release()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _count += 1;
        _condition.notify_one();
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
                return true;
            }
        }
        if (_count <= 0) {                            // ��Դ����
            std::cv_status status = _condition.wait_for(lock, std::chrono::milliseconds(ms));
            if (status == std::cv_status::timeout)   // ��ʱ
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
    /**
     * ��������ź���������
     * @return ����
     */
    std::string name() {
        return _name;
    }
};