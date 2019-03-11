#include "Logging.h"

#include <stdarg.h> 
#include <stdio.h>
#include <errno.h>

void actualPrint(const char *format, va_list args)
{
	char buffer[512] = "";  
	vsnprintf(buffer, 512, format, args);
	printf("%s \n", buffer);
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