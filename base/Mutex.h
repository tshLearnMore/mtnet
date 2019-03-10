#ifndef MTNET_BASE_MUTEX_H
#define MTNET_BASE_MUTEX_H

#include "Thread.h"
#include "Uncopyable.h"

#include <assert.h>
#include <pthread.h>

namespace mtnet
{
	class MutexLock : public Uncopyable
	{
	public:
		MutexLock()
		{
			pthread_mutex_init(&mutex_, NULL);
		}
		~MutexLock()
		{
			pthread_mutex_destroy(&mutex_);
		}
		void lock()
		{
			pthread_mutex_lock(&mutex_);
		}
		void unlock()
		{
			pthread_mutex_unlock(&mutex_);
		}
		pthread_mutex_t* getMutex()
		{
			return &mutex_;
		}
	protected:
	private:
		pthread_mutex_t mutex_;
	};

	class MutexLockGuard : public Uncopyable
	{
	public:
		explicit MutexLockGuard(MutexLock& mutex)
			:mutex_(mutex)
		{
			mutex_.lock();
		}
		~MutexLockGuard()
		{
			mutex_.unlock();
		}
	protected:
	private:
		MutexLock& mutex_;
	};
}

#endif  //MTNET_BASE_MUTEX_H