#ifndef MTNET_BASE_TIMESTAMP_H
#define MTNET_BASE_TIMESTAMP_H

#include <stdint.h>
#include <string>
#include <sys/time.h>

namespace mtnet
{
	class Timestamp
	{
	public:
		Timestamp();

		//参数以微秒为单位
		explicit Timestamp(int64_t microSecond);

		//数字转字符串 如：10000 到"10000"
		std::string toString() const;
		//之间转带年月日 时:分:秒.毫秒 的字符串 如: 20110327 09:16:10.22
		std::string toFormattedString() const;

		//以系统当前时间构建一个timestamp
		static Timestamp now()
		{
			struct timeval tv;
			gettimeofday(&tv, NULL);
			int64_t second = tv.tv_sec;
			return Timestamp(second * kmicroSecondPerSecond + tv.tv_usec);
		}

		void swap(Timestamp& rhs)
		{
			std::swap(microSecond_, rhs.microSecond_);
		}

		bool vaild() const
		{
			return microSecond_ > 0;
		}

		static Timestamp invalid()
		{
			return Timestamp();
		}

		static const int kmicroSecondPerSecond = 1000 * 1000;

		int64_t microSecond() const
		{
			return microSecond_;
		}


	private:
		int64_t microSecond_;
	};

//定义timestamp比较操作符

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
	return lhs.microSecond() < rhs.microSecond();
}

inline bool operator>(Timestamp lhs, Timestamp rhs)
{
	return lhs.microSecond() > rhs.microSecond();
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
	return lhs.microSecond() == rhs.microSecond();
}

inline Timestamp addTime(Timestamp timeStamp, double seconds)
{
	int64_t diff = static_cast<int64_t>(seconds * Timestamp::kmicroSecondPerSecond);
	return Timestamp(timeStamp.microSecond() + diff);
}
}

#endif //MTNET_BASE_TIMESTAMP_H