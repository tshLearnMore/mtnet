/*
EventLoopThreadPool�̳߳ط�װ
*/
#ifndef MYNET_MTNET_EVENTLOOPTHREADPOOL_H
#define MYNET_MTNET_EVENTLOOPTHREADPOOL_H

#include "Uncopyable.h"
#include "Condition.h"
#include "Mutex.h"
#include "Thread.h"

#include <vector>
#include <functional>
#include <memory>

namespace mtnet
{
	class EventLoop;
	class EventLoopThread;

	class EventLoopThreadPool : Uncopyable
	{
	public:
		EventLoopThreadPool(EventLoop* baseLoop);
		~EventLoopThreadPool();

		void setThreadNum(int numThreads)
		{
			numThreads_ = numThreads;
		}
		void start();
		EventLoop* getNextLoop();
	protected:
	private:
		EventLoop* baseLoop_;
		bool started_;
		int numThreads_;
		int next_;
		std::vector<std::shared_ptr<EventLoopThread>> threads_;
		std::vector<EventLoop*> loops_;
	};
}

#endif //MYNET_MTNET_EVENTLOOPTHREADPOOL_H