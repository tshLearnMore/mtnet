#include "Thread.h"

#include <unistd.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>
#include <assert.h>

namespace mtnet
{
	namespace CurrentThread
	{
		__thread const char* t_threadName = "undefine";
	}
}

namespace
{
	__thread pid_t t_cachedTid = 0;//每个线程有自己的tid

	pid_t gettid()
	{
		return static_cast<pid_t>(::syscall(SYS_gettid));
	}

	struct ThreadData
	{
		typedef mtnet::Thread::ThreadFunc ThreadFunc;
		ThreadFunc func_;
		std::string name_;
		std::weak_ptr<pid_t> wkTid_;

		ThreadData(const ThreadFunc& func,const std::string& name, const std::shared_ptr<pid_t>& tid)
			:func_(func),
			name_(name),
			wkTid_(tid)
		{}

		void runInhread()
		{
			pid_t tid = mtnet::CurrentThread::tid();
			std::shared_ptr<pid_t> ptid = wkTid_.lock();
			if (ptid)
			{
				*ptid = tid;
				ptid.reset();
			}
			mtnet::CurrentThread::t_threadName = name_.empty() ? "mtnetThread":name_.c_str();
			::prctl(PR_SET_NAME, mtnet::CurrentThread::t_threadName);
			func_();
			mtnet::CurrentThread::t_threadName = "finished";
		}
	};

	void* startThread(void* pObj)
	{
		ThreadData* data = static_cast<ThreadData*>(pObj);
		data->runInhread();
		delete data;
		return NULL;
	}
}

using namespace mtnet;

pid_t CurrentThread::tid()
{
	if (t_cachedTid == 0)
	{
		t_cachedTid = gettid();
	}
	return t_cachedTid;
}

const char* CurrentThread::name()
{
	return t_threadName;
}

bool CurrentThread::isMainThread()
{
	return tid() == ::getpid();
}

AtomicInt32 Thread::numCreated_;

Thread::Thread(const ThreadFunc& func, const std::string& name)
	:started_(false),
	joined_(false),
	pthreadId_(0),
	tid_(new pid_t(0)),
	func_(func),
	name_(name)
{
	numCreated_.increment();
}

Thread::~Thread()
{
	if (started_ && !joined_)
	{
		pthread_detach(pthreadId_);
	}
}

void Thread::start()
{
	assert(!started_);//或者直接return
	started_ = true;
	ThreadData* data = new ThreadData(func_,name_,tid_);
	if (pthread_create(&pthreadId_,NULL,&startThread,data))
	{
		started_ = false;
		delete data;
		abort();
	}
}

void Thread::join()
{
	assert(started_);//或者直接return
	assert(!joined_);
	joined_ = true;
	pthread_join(pthreadId_,NULL);
}