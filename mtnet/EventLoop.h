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
		//poll����ʱ��ʱ�����ͨ����־���յ�������
		Timestamp pollReturnTime() const
		{
			return pollReturnTime_;
		}

		//��loop�߳������лص�����
		void runInLoop(const Functor& cb);
		//���ص�cb���뵽queue��ȥ�����Ժ��pool�л�ִ��
		void queueInLoop(const Functor& cb);

		//��ָ��ʱ�����лص�
		TimerId runAt(const Timestamp& time, const TimerCallback& cb);
		//���ƶ���ʱ��ú�����
		TimerId runAfter(double delay, const TimerCallback& cb);
		//���������� 
		TimerId runEvery(double interval, const TimerCallback& cb);

		//ֻ�ڲ�ʹ�� 
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