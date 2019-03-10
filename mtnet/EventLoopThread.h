#ifndef MYMTNET_MTNET_EVENTLOOPTHREAD_H
#define MYMTNET_MTNET_EVENTLOOPTHREAD_H

#include "Condition.h"
#include "Mutex.h"
#include "Thread.h"
#include "Uncopyable.h"

namespace mtnet
{
	class EventLoop;
	class EventLoopThread : private Uncopyable
	{
	public:
		EventLoopThread();
		~EventLoopThread();
		EventLoop* startLoop();
	private:
		void threadFunc();

		EventLoop* loop_;
		bool exiting_;
		Thread thread_;
		MutexLock mutex_;
		Condition cond_;
	};
}


#endif //MYMTNET_MTNET_EVENTLOOPTHREAD_H