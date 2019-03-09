#ifndef MTNET_BASE_LOGGING_H
#define MTNET_BASE_LOGGING_H

#include <stdarg.h> 
#include <stdio.h>

enum LogLevel
{
	LOG_LEVEL_TRACE,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_INFO,
	LOG_LEVEL_WARN,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_FATAL,
};

#ifndef LOG_LEVEL
#define LOG_LEVEL	LOG_LEVEL_INFO //LOG_LEVEL_TRACE
#endif

/*
#ifndef SYS_LOG_DETAIL
#define PRINT(format,...)   printf("%s:"format,SYS_LOG_TAG,##__VA_ARGS__);
#else
#define PRINT(format,...)   printf("%s %s: [%s,%s,%d]: %s: " format,__DATE__,__TIME__,__FILE__,__FUNCTION__,__LINE__,SYS_LOG_TAG,##__VA_ARGS__);
#endif // !SYS_LOG_DETAIL
*/

void actualPrint(const char *format, va_list args)
{
	char buffer[512] = "";  
	vsnprintf(buffer, 512, format, args);
	printf("%s", buffer);
}
void LOG_TRACE(const char* format, ...)
{
	if (LOG_LEVEL <= LOG_LEVEL_TRACE)
	{
		va_list va;
		va_start(va, format);
		actualPrint(format,va);
		va_end(va);
	}
}

void LOG_DEBUG(const char* format, ...)
{
	if (LOG_LEVEL <= LOG_LEVEL_DEBUG) {

		va_list va;
		va_start(va, format);
		actualPrint(format,va);
		va_end(va);
	}
}

void LOG_INFO(const char* format, ...)
{
	if (LOG_LEVEL <= LOG_LEVEL_INFO) {
		va_list va;
		va_start(va, format);
		actualPrint(format,va);
		va_end(va);
	}
}

void LOG_WARN(const char* format, ...)
{
	if (LOG_LEVEL <= LOG_LEVEL_WARN) {
		va_list va;
		va_start(va, format);
		actualPrint(format,va);
		va_end(va);
	}
}

void LOG_ERROR(const char* format, ...)
{
	if (LOG_LEVEL <= LOG_LEVEL_ERROR) {
		va_list va;
		va_start(va, format);
		actualPrint(format,va);
		va_end(va);
	}
}


void LOG_FATAL(const char* format, ...)
{
	if (LOG_LEVEL <= LOG_LEVEL_FATAL) {
		va_list va;
		va_start(va, format);
		actualPrint(format,va);
		va_end(va);
	}
}

#endif //MTNET_BASE_LOGGING_H