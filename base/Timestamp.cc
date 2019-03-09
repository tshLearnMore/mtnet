#include "Timestamp.h"

#include <sys/time.h>
#include <stdio.h>
//使用 PRId64需要 跨平台方式打印int64_t 
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#undef __STDC_FORMAT_MACROS

using namespace mtnet;

Timestamp::Timestamp()
	:microSecond_(0)
{
}

Timestamp::Timestamp(int64_t microSecond)
	:microSecond_(microSecond)
{
}

std::string Timestamp::toString() const
{
	char buf[32] = { 0 };
	int64_t second = microSecond_ / kmicroSecondPerSecond;
	int64_t microSecond = microSecond_ % kmicroSecondPerSecond;
	snprintf(buf, sizeof(buf)-1, "%" PRId64 ".%06" PRId64 "", second, microSecond);
	return buf;
}

std::string Timestamp::toFormattedString() const
{
	char buf[32] = {0};
	time_t second = static_cast<time_t>(microSecond_ / kmicroSecondPerSecond);
	int microSecond = static_cast<int>(microSecond_ % kmicroSecondPerSecond);
	struct tm tm_time;
	gmtime_r(&second, &tm_time);//转换为用UTC时间表示的时间
	snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
		tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
		tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
		microSecond);
	return buf;
}