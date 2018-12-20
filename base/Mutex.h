#ifndef MTNET_BASE_MUTEX_H
#define MTNET_BASE_MUTEX_H

#include "Thread.h"
#include "Uncopyable.h"

#include <assert.h>
#include <pthread.h>

namespace mtnet
{
	class Mutex : public Uncopyable
	{
	public:
		Mutex()
		{
			pthread_mutex_init(&mutex_, NULL);
		}
		~Mutex()
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
		explicit MutexLockGuard(Mutex& mutex)
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
		Mutex& mutex_;
	};
}

#endif  //MTNET_BASE_MUTEX_H