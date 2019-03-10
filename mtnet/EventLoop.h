#ifndef MYNET_MTNET_EVENTLOOP_H
#define MYNET_MTNET_EVENTLOOP_H

#include "Timestamp.h"
#include "Mutex.h"
#include "Thread.h"
#include "Callbacks.h"
#include "TimerId.h"

#include <memory>
#include <vector>

namespace mtnet
{
	class Poller;
	class Channel;
	class TimerQueue;

	class EventLoop : private Uncopyable
	{
	public:
		typedef std::function<void()> Functor;
		EventLoop();
		~EventLoop();
		void loop();
		void quit();
		//poll返回时的时间戳，通常标志着收到新数据
		Timestamp pollReturnTime() const
		{
			return pollReturnTime_;
		}

		//在loop线程中运行回调函数
		void runInLoop(const Functor& cb);
		//将回调cb加入到queue中去，在稍后的pool中会执行
		void queueInLoop(const Functor& cb);

		//在指定时间运行回调
		TimerId runAt(const Timestamp& time, const TimerCallback& cb);
		//在制定延时多久后运行
		TimerId runAfter(double delay, const TimerCallback& cb);
		//周期性运行 
		TimerId runEvery(double interval, const TimerCallback& cb);

		//只内部使用 
		void wakeup();
		void updateChannel(Channel* channel);
		void removeChannel(Channel* channel);

		void assertInLoopThread()
		{
			if (!isInLoopThread())
			{
				abortNotInLoopThread();
			}
		}

		bool isInLoopThread() const
		{
			return threadId_ == CurrentThread::tid();
		}
		
	protected:
	private:
		void abortNotInLoopThread();
		void handleRead();  //waked up
		void doPendingFunctors();

		typedef std::vector<Channel*> ChannelList;

		bool looping_;
		bool quit_;
		bool calllingPendingFunctors_;
		const pid_t threadId_;
		Timestamp pollReturnTime_;
		std::shared_ptr<Poller> poller_;
		std::shared_ptr<TimerQueue> timeQueue_;
		int wakeupFd_;
		std::shared_ptr<Channel> wakeupChannel_;
		ChannelList activeChannels_;
		MutexLock mutex_;
		std::vector<Functor> pendingFunctors_;
	};
}

#endif //MYNET_MTNET_EVENTLOOP_H