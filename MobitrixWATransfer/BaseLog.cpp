
/***************************************************************************
 *   Copyright (C) 2012 by sdzw   *
 *   root@localhost.localdomain   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "BaseLog.h"
#include "string.h"
#include <stdarg.h>

#include <sys/stat.h>
#include <stdlib.h>
#include <corecrt_io.h>


#include "publicfun.h"



bool  g_xprintlog = false;
BaseLog g_xbaselog;
BaseLog::BaseLog()
{
	//

}


BaseLog::~BaseLog()
{
}
void BaseLog::clearLogfile()
{
	std::string  tpath=GetLogDir();
	if(tpath.length()>6)
		EnumPath_Clear(tpath.c_str());

}
void BaseLog::WriteLog(const char* logname, const char* fm, va_list argptr)
{

	std::string filename = GetLogName(logname);
	struct stat file;
	int result = stat(filename.c_str(), &file);
	if (result == 0 && file.st_size > 1068384 * 50)
	{
		return;
	}
	int isize = 0;
	char buff[5 * 1024];
	memset(buff, 0, 5 * 1024);
	char curtime[32];
	memset(curtime, 0, 32);
	int curtimelen = 0;
	MGetCurTime(curtime, true);
	curtimelen = sprintf(buff, "%s\t", curtime);
	isize = vsnprintf(buff + curtimelen, 5 * 1024 - 1 - curtimelen, fm, argptr);
	isize += curtimelen;
	if (isize > 5 * 1024)
	{
		isize=5 * 1024;
	}

//	printf(buff);
//	printf("\n");
//	fflush(stdout);
	Lock();
	FILE* pfile = fopen(filename.c_str(), "ab+");
	if (pfile)
	{
		fwrite(buff, 1, isize, pfile);
		fwrite("\r\n", 1, 2, pfile);
		fclose(pfile);
	}
	Unlock();
}
void AClearLog()
{

	g_xbaselog.clearLogfile();

}
void AWriteLog(const char* namelog, const char* fm, ...)
{
	//return;
	va_list argptr;
	va_start(argptr, fm);
	g_xbaselog.WriteLog(namelog, fm, argptr);
	va_end(argptr);

}

