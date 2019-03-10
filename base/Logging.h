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

	void LOG_TRACE(const char* format, ...);

	void LOG_DEBUG(const char* format, ...);

	void LOG_INFO(const char* format, ...);

	void LOG_WARN(const char* format, ...);

	void LOG_ERROR(const char* format, ...);


	void LOG_FATAL(const char* format, ...);


#endif //MTNET_BASE_LOGGING_H