#ifndef MYNET_BASE_ATOMIC_H
#define MYNET_BASE_ATOMIC_H

#include "Uncopyable.h"
#include <stdint.h>

namespace mtnet
{

namespace detail
{
	//数值的原子操作
	template<typename T>
	class AtomicInterT : Uncopyable
	{
	public:
		AtomicInterT()
			:value_(0)
		{}
		T get() const
		{
			return __sync_val_compare_and_swap(const_cast<volatile T*>(&value_),0,0);
		}
		T getAndAdd(T x)
		{
			return __sync_fetch_and_add(&value_,x);
		}
		T addAndGet(T x)
		{
			return getAndAdd(x) + x;
		}
		void add(T x)
		{
			getAndAdd(x);
		}
		void increment()
		{
			getAndAdd(1);
		}
		void decrement()
		{
			getAndAdd(-1);
		}
		T getAndSet(T newValue)
		{
			return __sync_lock_test_and_set(&value_,newValue);
		}
	protected:
	private:
		volatile T value_;
	};
}

typedef detail::AtomicInterT<int32_t> AtomicInt32;
typedef detail::AtomicInterT<int64_t> AtomicInt64;
}

#endif  //MYNET_BASE_ATOMIC_H