#ifndef MTNET_BASE_THREAD_H
#define MTNET_BASE_THREAD_H

#include "Atomic.h"
#include "Uncopyable.h"

#include <functional>
#include <memory>
#include <pthread.h>

namespace mtnet
{

class Thread : Uncopyable
{
public:
	typedef std::function<void ()> ThreadFunc;

	explicit Thread(const ThreadFunc& func, const std::string& name = std::string());
	~Thread();

	void start();
	void join();

	bool started() const
	{
		return started_;
	}
	bool joined() const
	{
		return joined_;
	}
	pid_t tid() const
	{
		return *tid_;
	}
	const std::string& name() const
	{
		return name_;
	}
	static int numCreated()
	{
		return numCreated_.get();
	}
protected:
private:
	bool	started_;
	bool	joined_;
	pthread_t	pthreadId_;
	std::shared_ptr<pid_t> tid_;
	ThreadFunc	func_;
	std::string		name_;

	static AtomicInt32 numCreated_;
};

namespace CurrentThread
{
	pid_t tid();
	const char* name();
	bool isMainThread();
}
}

#endif  //MTNET_BASE_THREAD_H