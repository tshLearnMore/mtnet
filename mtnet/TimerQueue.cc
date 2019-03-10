#include "TimerQueue.h"

#include "Logging.h"
#include "EventLoop.h"
#include "Timer.h"
#include "TimerId.h"
#include "Timestamp.h"

#include <functional>
#include <sys/timerfd.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

namespace mtnet
{
	//Linux中timerfd操作封装
	namespace detail
	{
		int createTimerfd()
		{
			int timerfd = ::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK | TFD_CLOEXEC);
			if (timerfd < 0)
			{
				LOG_FATAL("Failed in timerfd_create") ;
			}
			return timerfd;
		}

		struct timespec howMuchTimeFromNow(Timestamp when)
		{
			//微秒 
			int64_t microseconds = when.microSecond() - Timestamp::now().microSecond();
			if (microseconds < 100)
			{
				microseconds = 100;
			}

			struct timespec ts;
			ts.tv_sec = static_cast<time_t>(microseconds / Timestamp::kmicroSecondPerSecond);
			ts.tv_nsec = static_cast<long>((microseconds % Timestamp::kmicroSecondPerSecond) * 1000);
			return ts;
		}

		void readTimerfd(int timerfd, Timestamp now)
		{
			uint64_t howmany;
			ssize_t n = ::read(timerfd,&howmany, sizeof(howmany));
			LOG_TRACE("TimerQueue::handleRead() %d at %s", howmany , now.toString());
			if (n != sizeof(howmany))
			{
				LOG_ERROR("TimerQueue::handleRead()  reads %d bytes instead of 8",n);
			}
		}

		void resetTimerfd(int timerfd, Timestamp expiration)
		{
			struct itimerspec newValue;
			struct itimerspec oldValue;
			bzero(&newValue, sizeof(newValue));
			bzero(&oldValue, sizeof(oldValue));
			newValue.it_value = howMuchTimeFromNow(expiration);
			int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
			if (ret)
			{
				LOG_ERROR("timerfd_settime()");
			}
		}
	}
}

using namespace mtnet;
using namespace mtnet::detail;

TimerQueue::TimerQueue(EventLoop* loop)
	:loop_(loop),
	timerfd_(createTimerfd()),
	timerfdChannel_(loop,timerfd_),
	timers_()
{
	timerfdChannel_.setReadCallback(std::bind(&TimerQueue::handleRead, this));
	timerfdChannel_.enableReading();
}

TimerQueue::~TimerQueue()
{
	::close(timerfd_);
	for (TimerList::iterator it = timers_.begin(); it!=timers_.end();it++)
	{
		delete it->second;
	}
}

TimerId TimerQueue::addTimer(const TimerCallback& cb, Timestamp when, double interval)
{
	Timer* timer = new Timer(cb,when,interval);
	loop_->runInLoop(std::bind(&TimerQueue::addTimerInloop,this,timer));
	return TimerId(timer);
}

void TimerQueue::addTimerInloop(Timer* timer)
{
	loop_->assertInLoopThread();
	//时间队列的最小值有更改
	bool earlistChanged = insert(timer);
	if (earlistChanged)
	{
		resetTimerfd(timerfd_,timer->expiration());
	}
}

void TimerQueue::handleRead()
{
	loop_->assertInLoopThread();
	Timestamp now(Timestamp::now());
	readTimerfd(timerfd_,now);

	std::vector<Entry> expired = getExpired(now);
	for (std::vector<Entry>::iterator it = expired.begin(); it != expired.end();it++)
	{
		it->second->run();
	}
	reset(expired,now);
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{
	std::vector<Entry>  expired;
	Entry sentry = std::make_pair(now,reinterpret_cast<Timer*>(UINTPTR_MAX));
	TimerList::iterator it = timers_.lower_bound(sentry);
	assert(it == timers_.end() || now < it->first);
	std::copy(timers_.begin(),it,back_inserter(expired));
	timers_.erase(timers_.begin(),it);

	return expired;
}

void TimerQueue::reset(const std::vector<Entry>& expired, Timestamp now)
{
	Timestamp nextExpire;
	for (std::vector<Entry>::const_iterator it = expired.begin();it != expired.end();it++)
	{
		if (it->second->repeat())
		{
			it->second->restart(now);
			insert(it->second);
		} 
		else
		{
			//优化 移到一个free list中
			delete it->second;
		}
	}

	if (!timers_.empty())
	{
		nextExpire = timers_.begin()->second->expiration();
	}

	if (nextExpire.vaild())
	{
		resetTimerfd(timerfd_,nextExpire);
	}
}

bool TimerQueue::insert(Timer* timer)
{
	bool earliestChanged = false;
	Timestamp when = timer->expiration();
	TimerList::iterator it = timers_.begin();
	if (it == timers_.end() || when < it->first)
	{
		earliestChanged = true;
	}
	std::pair<TimerList::iterator,bool> result = timers_.insert(std::make_pair(when,timer));
	assert(result.second);

	return earliestChanged;
}