#ifndef MYNET_MTNET_TIMERID_H
#define MYNET_MTNET_TIMERID_H

namespace mtnet
{
	class Timer;
	class TimerId
	{
	public:
		explicit TimerId(Timer* timer)
			:value_(timer)
		{ }
	private:
		Timer* value_;
	};

}

#endif //MYNET_MTNET_TIMERID_H