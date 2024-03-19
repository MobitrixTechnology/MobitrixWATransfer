#pragma once
#include "BaseLock.h"

#include <stdarg.h>
#include <string>
#include "BaseLock.h"


class BaseLog :public Mutex
{
public:
	void WriteLog(const char* logname, const char* fm, va_list argptr);
	void clearLogfile();
public:
	BaseLog();

	~BaseLog();

};

extern bool  g_xprintlog;
extern BaseLog g_xbaselog;


void AWriteLog(const char* namelog, const char* fm, ...);
void AClearLog();

