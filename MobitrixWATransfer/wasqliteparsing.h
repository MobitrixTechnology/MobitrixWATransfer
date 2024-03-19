#pragma once
//
#include <sqlite3/sqlite3.h>
//#include "publicfun.h"
#include <string>
#include <vector>
#include <map>
#define MAXCOUNT  2000
struct WALimitUnit
{
	unsigned int pos;
	unsigned int len;
	WALimitUnit()
	{
		pos = len = 0;
	}



};

struct wablockbytes
{
	char* pdata;
	unsigned int plen;
	wablockbytes()
	{
		pdata = NULL;
		plen = 0;
	}
};

class wasqliteparsing
{

public:
	wasqliteparsing(const char* databasename);

	~wasqliteparsing();


private:

	sqlite3* pdb;       //
	char* errMsg;       //
	int res;            //
	bool  isopenok;
public:

	bool SqlExe(char* sql);    //

	bool SqlQuery(const char* sql, std::vector<std::map<std::string, std::string> >& str);//
	int  SqlQuery(const char* sql, std::string& str, const char* splite1 = "\2", char splite2 = '\1');
	bool SqlQuery(const char* sql, std::vector<std::map<std::string, wablockbytes> >& str);
	const char* GetLastError();
	bool SqlQuery_Huawei(const char* sql, std::map<int, std::string>& filenamemap);
	bool text();
	int InitDataBaseToWal(std::string sPath, bool isWal);//WAL

};

