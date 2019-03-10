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
				expiration_ = addTime(now,interval_);//����ʱ��
			} 
			else
			{
				expiration_ = Timestamp::invalid();//���һ����Ч��ʱ��
			}
		}
	protected:
	private:
		const TimerCallback callback_;
		Timestamp expiration_;//����ʱ��
		const double interval_;
		const bool repeat_;
	};
}


#endif //MYNET_MTNET_TIMER_H