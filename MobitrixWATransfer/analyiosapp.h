#pragma once

#include "wasqliteparsing.h"
//IOS APP
class analyiosapp
{
public:
	int analyzerwaapp(const char* backuppath, const char* outpath, int apptype, int tol);
private:
	int toanalyiosapp(const char* backpath, const char* outpathpath, const char* domain, const char* package, int index, int& num);
	int toanalyiosapp_mbdb(const char* backuppath, const char* outpathpath, const char* domain, const char* package, int index, int& num);

	
};

