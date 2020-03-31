#pragma once
#ifndef _BLOCKINGQUEUE_H_
#define _BLOCKINGQUEUE_H_

#include <mutex>
#include <condition_variable>
#include <deque>
#include <assert.h>


template <typename T>
class BlockingQueue {
public:
	using MutexLockGuard = std::lock_guard<std::mutex>;

	BlockingQueue()
		: _mutex(),
		_notEmpty(),
		_queue()
	{

	}

	BlockingQueue(const BlockingQueue&) = delete;
	BlockingQueue& operator=(const BlockingQueue&) = delete;

	void put(const T& x)
	{
		{
			MutexLockGuard lock(_mutex);
			_queue.push_back(x);
		}
		_notEmpty.notify_all();
	}

	void put(T&& x)
	{
		{
			MutexLockGuard lock(_mutex);
			_queue.push_back(std::move(x));
		}
		_notEmpty.notify_all();
	}

	bool take(uint32_t time,T *data)
	{
		std::unique_lock<std::mutex> lock(_mutex);

		//auto state=_notEmpty.wait_for(lock,std::chrono::milliseconds(time), [this] {  return !this->_queue.empty(); });
		//assert(!_queue.empty());
		auto state = _notEmpty.wait_for(lock, std::chrono::milliseconds(time), [this] {  return !this->_queue.empty(); });
		if (state == true)
		{
			T front(std::move(_queue.front()));
			_queue.pop_front();
			*data = front;
			return true;
		}
		else
		{
			return false;
		}
	}

	size_t size() const
	{
		MutexLockGuard lock(_mutex);
		return _queue.size();
	}

private:
	mutable std::mutex _mutex;
	std::condition_variable _notEmpty;
	std::deque<T> _queue;
};

#endif  // _BLOCKINGQUEUE_H_
