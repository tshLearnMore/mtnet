#include "EventLoop.h"

#include "Logging.h"
#include "Channel.h"
#include "Poller.h"
#include "TimerQueue.h"

#include <functional>
#include <assert.h>
#include <signal.h>
#include <sys/eventfd.h>
#include <unistd.h>   //close()

using namespace mtnet;

//每个线程一份__thread保证
__thread EventLoop* t_loopInThisThread = 0;
const int kPollTimeMs = 10000;

static int createEventfd()
{
	int eventfd = ::eventfd(0,EFD_NONBLOCK | EFD_CLOEXEC);
	if (eventfd < 0)
	{
		LOG_ERROR("Failed in eventfd");
		abort();
	}
	return eventfd;
}

class IgnoreSigPipe
{
public:
	IgnoreSigPipe()
	{
		::signal(SIGPIPE,SIG_IGN);
	}
};
/*为了忽略SIGPIPE信号
当客户端ctr+c断开连接时，服务端没有来得及处理，就会导致给已关闭的连接继续
发送数据，这会导致服务端意外退出
*/
IgnoreSigPipe initObj;

EventLoop::EventLoop()
	:looping_(false),
	quit_(false),
	calllingPendingFunctors_(false),
	threadId_(CurrentThread::tid()),
	poller_(new Poller(this)),
	timeQueue_(new TimerQueue(this)),
	wakeupFd_(createEventfd()),
	wakeupChannel_(new Channel(this,wakeupFd_))
{
	LOG_TRACE("EventLoop create %x  in threaad  %d",this,threadId_);

	if (t_loopInThisThread)
	{
		LOG_TRACE("Another EventLoop %d exitts in this thread %d",t_loopInThisThread,threadId_);
	}
	else
	{
		t_loopInThisThread = this;
	}
	wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead,this));
	// 一直关注wakeupfd的可读事件
	wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{
	assert(!looping_);
	::close(wakeupFd_);
	t_loopInThisThread = 0;
}

void EventLoop::loop()
{
	assert(!looping_);
	assertInLoopThread();
	looping_ = true;
	quit_ = false;

	while(!quit_)
	{
		activeChannels_.clear();
		pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
		ChannelList::iterator it = activeChannels_.begin();
		for (;it != activeChannels_.end();it++)
		{
			(*it)->handelEvent(pollReturnTime_);
		}
		doPendingFunctors();
	}
	LOG_TRACE("EventLoop %x stop looping",this);
	looping_ = false;
}

void EventLoop::quit()
{
	quit_ = true;
	if (!isInLoopThread())
	{
		wakeup();
	}
}

void EventLoop::runInLoop(const Functor& cb)
{
	if (isInLoopThread())
	{
		cb();
	} 
	else
	{
		queueInLoop(cb);//加入到queue
	}
}

void EventLoop::queueInLoop(const Functor& cb)
{
	{
		MutexLockGuard lock(mutex_);
		pendingFunctors_.push_back(cb);
	}
	if (!isInLoopThread() || calllingPendingFunctors_)
	{
		wakeup();
	}
}

TimerId EventLoop::runAt(const Timestamp& time, const TimerCallback& cb)
{
	return timeQueue_->addTimer(cb, time, 0.0);
}

TimerId EventLoop::runAfter(double delay, const TimerCallback& cb)
{
	Timestamp time(addTime(Timestamp::now(), delay));
	return runAt(time, cb);
}

TimerId EventLoop::runEvery(double interval, const TimerCallback& cb)
{
	Timestamp time(addTime(Timestamp::now(), interval));
	return timeQueue_->addTimer(cb, time, interval);
}

void EventLoop::updateChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
	assertInLoopThread();
	poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
	assertInLoopThread();
	poller_->removeChannel(channel);
}

void EventLoop::abortNotInLoopThread()
{
	LOG_FATAL("EventLoop::abortNotInLoopThread - EventLoop %x was creatre in threadId_ = %d current thread id = %d",this,threadId_,CurrentThread::tid());
}

void EventLoop::wakeup()
{
	uint64_t one = 1;
	ssize_t n = ::write(wakeupFd_,&one, sizeof(one));
	if (n != sizeof(one))
	{
		LOG_ERROR("EventLoop::wakeup() write %d bytes instead of 8" ,n);
	}
}

void EventLoop::handleRead()
{
	uint64_t one = 1;
	ssize_t n = ::read(wakeupFd_,&one, sizeof(one));
	if (n != sizeof(one))
	{
		LOG_ERROR("EventLoop::handleRead() reads %d bytes instead of 8",n);
	}
}

void EventLoop::doPendingFunctors()
{
	std::vector<Functor> functions;
	calllingPendingFunctors_ = true;

	{
		MutexLockGuard lock(mutex_);
		functions.swap(pendingFunctors_);
	}
	for (size_t i = 0; i < functions.size(); i++)
	{
		functions[i]();
	}
	calllingPendingFunctors_ = false;
}