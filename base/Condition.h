#ifndef MTNET_BASE_CONDITION_H
#define MTNET_BASE_CONDITION_H

#include "Mutex.h"
#include "Uncopyable.h"

#include <pthread.h>
#include <errno.h>

namespace mtnet
{
	class Condition : public Uncopyable
	{
	public:
		explicit Condition(Mutex& mutex)
			:mutex_(mutex)
		{
			pthread_cond_init(&pcond_, NULL);
		}
		~Condition()
		{
			pthread_cond_destroy(&pcond_);
		}
		void wait()
		{
			pthread_cond_wait(&pcond_, mutex_.getMutex());
		}
		void notify()
		{
			pthread_cond_signal(&pcond_);
		}
		//可能会造成惊群现象，通常使用notify函数
		void notifyAll()
		{
			pthread_cond_broadcast(&pcond_);
		}
	protected:
	private:
		Mutex& mutex_;
		pthread_cond_t pcond_;
	};
}

#endif  //MTNET_BASE_CONDITION_H