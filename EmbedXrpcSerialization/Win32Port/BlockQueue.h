#ifndef BLOCKQUEUE_H
#define BLOCKQUEUE_H
#include <QWaitCondition>
#include <QSemaphore>
#include <QMutex>
#include <QQueue>
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
	void Send(T& msg);
	QueueStatus Receive(T& msg, unsigned long timeOut);
	void Reset();
private:
	//QWaitCondition		  waitCondition;
	QSemaphore			semaphore;
	QMutex				  mutex;
	QQueue<T>				queue;
};
template<class T>
void BlockingQueue<T>::Send(T& msg)
{
	mutex.lock();
	queue.enqueue(msg);
	mutex.unlock();
	semaphore.release(1);
	//waitCondition.wakeAll();
}
template<class T>															//timeOut最大等待延迟时间
QueueStatus BlockingQueue<T>::Receive(T& msg, unsigned long timeOut)
{
	QueueStatus s= QueueStatus::Empty;
	bool r=semaphore.tryAcquire(1, timeOut);
	mutex.lock();
	if (r == true)
	{
		
		//waitCondition.wait(&mutex, timeOut);
		if (queue.count() > 0)
		{
			s = QueueStatus::Ok;

			msg = queue.dequeue();

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
	queue.clear();
	while (semaphore.tryAcquire(1) == true);
	mutex.unlock();
}
#endif