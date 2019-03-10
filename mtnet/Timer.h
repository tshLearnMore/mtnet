#ifndef MYNET_MTNET_TIMER_H
#define MYNET_MTNET_TIMER_H

#include "Uncopyable.h"
#include "Timestamp.h"
#include "Callbacks.h"

namespace mtnet
{
	class Timer : Uncopyable
	{
	public:
		Timer(const TimerCallback& cb,Timestamp when, double interval)
			:callback_(cb),
			expiration_(when),
			interval_(interval),
			repeat_(interval > 0.0)
		{ }
		void run() const
		{
			callback_();
		}
		Timestamp expiration() const
		{
			return expiration_;
		}
		bool repeat() const
		{
			return repeat_;
		}
		void restart(Timestamp now)
		{
			if (repeat_)
			{
				expiration_ = addTime(now,interval_);//更新时间
			} 
			else
			{
				expiration_ = Timestamp::invalid();//获得一个无效的时间
			}
		}
	protected:
	private:
		const TimerCallback callback_;
		Timestamp expiration_;//到期时间
		const double interval_;
		const bool repeat_;
	};
}


#endif //MYNET_MTNET_TIMER_H