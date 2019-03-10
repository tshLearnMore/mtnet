#ifndef MYNET_MTNET_TIMERQUEUE_H
#define MYNET_MTNET_TIMERQUEUE_H

#include "Timestamp.h"
#include "Mutex.h"
#include "Callbacks.h"
#include "channel.h"
#include "Uncopyable.h"

#include <set>
#include <vector>

namespace mtnet
{
	class EventLoop;
	class Timer;
	class TimerId;

	class TimerQueue : Uncopyable
	{
	public:
		TimerQueue(EventLoop* loop);
		~TimerQueue();
		//在指定时间回调，当interval > 0时将周期性重复调用
		TimerId addTimer(const TimerCallback& cb, Timestamp when, double interval);
	protected:
	private:
		typedef std::pair<Timestamp, Timer*> Entry;
		typedef std::set<Entry> TimerList;

		void addTimerInloop(Timer* timer);
		//定时器到时调用
		void handleRead();
		//返回所有到期的定时器
		std::vector<Entry> getExpired(Timestamp now);
		void reset(const std::vector<Entry>& expired, Timestamp now);
		bool insert(Timer* timer);

		EventLoop* loop_;
		const int timerfd_;
		Channel timerfdChannel_;
		//定时器列表按照到期时间排序
		TimerList timers_;
	};
}

#endif //MYNET_MTNET_TIMERQUEUE_H