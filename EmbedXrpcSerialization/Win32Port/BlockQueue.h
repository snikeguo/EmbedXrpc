#ifndef BLOCKQUEUE_H
#define BLOCKQUEUE_H
#include "Semaphore.h"
#include <mutex>
#include <queue>
enum QueueStatus
{
	Full,
	Empty,
	Ok,
};
template<class T>
class BlockingQueue
{
public:
	BlockingQueue():semaphore("BlockingQueue.semaphore"){}
	void Send(T& msg);
	QueueStatus Receive(T& msg, unsigned long timeOut);
	void Reset();
private:
	//QWaitCondition		  waitCondition;
	Semaphore			semaphore;
	std::mutex	mutex;
	std::queue<T>		queue;
};
template<class T>
void BlockingQueue<T>::Send(T& msg)
{
	mutex.lock();
	queue.push(msg);
	mutex.unlock();
	semaphore.release();
	//waitCondition.wakeAll();
}
template<class T>															//timeOut最大等待延迟时间
QueueStatus BlockingQueue<T>::Receive(T& msg, unsigned long timeOut)
{
	QueueStatus s= QueueStatus::Empty;
	bool r=semaphore.wait(timeOut);
	mutex.lock();
	if (r == true)
	{
		
		//waitCondition.wait(&mutex, timeOut);
		if (queue.size() > 0)
		{
			s = QueueStatus::Ok;

			msg = queue.front();

			queue.pop();

		}
		else
		{
			s = QueueStatus::Empty;
		}
		
	}
	mutex.unlock();
	return s;
}
template<class T>
void BlockingQueue<T>::Reset()
{
	mutex.lock();
	while (!queue.empty()) queue.pop();
	while (semaphore.wait(0) == true);
	mutex.unlock();
}
#endif