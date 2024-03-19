#include "publicfun.h"
///#include <Windows.h>
#include <io.h>

#include <sys/stat.h>
#include <time.h>

#include "zmd5.h"
#include "sha1.h"

#include "BaseLog.h"
#include "Base64.h"


#define READ_DATA_SIZE	1024
#define MD5_SIZE		16
#define MD5_STR_LEN		(MD5_SIZE * 2)

void usleep(DWORD waitTime) {
	LARGE_INTEGER perfCnt, start, now;

	QueryPerformanceFrequency(&perfCnt);
	QueryPerformanceCounter(&start);

	do {
		QueryPerformanceCounter((LARGE_INTEGER*)&now);
	} while ((now.QuadPart - start.QuadPart) / (float)(perfCnt.QuadPart) * 1000 * 1000 < waitTime);
}

std::string GetFileUpperDir(std::string filepath)
{
	int pos1;
	pos1 = filepath.find_last_of('\\');
	if(pos1>0)
		return filepath.substr(0,pos1);
	else
	{
		return "";
	}

}
std::string GetCurWorkRootDir()
{
	std::string retstr;

	char	ReSetup[MAX_PATH];
	memset(ReSetup, 0, MAX_PATH);
	GetModuleFileNameA(NULL, ReSetup, MAX_PATH);
	std::string tmpstr = GetFileUpperDir(ReSetup);

	return tmpstr;
}

void SplitStoV(char* buf, int len,const char* e, std::vector<std::string>& sVector, bool isdelempty)
{
	sVector.clear();
	int count = 0;
	char* tp = buf;
	int splitword = strlen(e);
	char* p = (char*)strstr(buf, e);
	while (p != NULL)
	{
		*p = 0;
		int off = p - tp;
		if (!(isdelempty && off == 0))
		{
			sVector.push_back(tp);
			count++;
		}
		tp = p + splitword;
		len -= (off + splitword);
		p = (char*)strstr(tp, e);
	}
	if (len > 0)
	{
		sVector.push_back(tp);
		count++;
		return;
	}
	if (!isdelempty && len == 0)
	{
		sVector.push_back(tp);
		count++;
	}
	return;
}


struct PosionUnit
{
	char* p;		
	int len;		
	bool newvalue;	
	PosionUnit()
	{
		newvalue = false;
		len = 0;
		p = NULL;
	}
};
void Replace(std::string& str, const char* oldvalue, const char* newvalue)
{
	if (str.length() == 0)
	{
		return;
	}
	int len1 = strlen(oldvalue);
	int len2 = strlen(newvalue);
	if (len1 == len2)
	{
		if (memcmp(oldvalue, newvalue, len1) == 0)
		{
			return;
		}
	}


	std::vector<PosionUnit> vect;
	int newlen = 0;
	char* p = NULL;
	int len = str.size();
	int leftlen = str.size();
	char* buf = (char*)str.c_str();
	bool flg = false;
	do
	{

		p = (char*)strstr(buf, oldvalue);
		if (p)
		{
			PosionUnit unitold;
			unitold.p = buf;
			unitold.len = p - buf;
			vect.push_back(unitold);
			PosionUnit unitnew;
			unitnew.newvalue = true;
			vect.push_back(unitnew);
			buf = p + len1;
			leftlen -= (unitold.len + len1);
			flg = true;
			newlen += (unitold.len + len2);
		}
		else
		{
			if (buf != NULL && leftlen && flg)
			{
				PosionUnit unitnew;
				unitnew.p = buf;
				unitnew.len = leftlen;
				vect.push_back(unitnew);
				newlen += leftlen;
			}
			break;
		}

	} while (p);

	if (flg)		
	{

		len = 0;
		char* buf = new char[newlen + 1];
		buf[newlen] = 0;
		std::vector<PosionUnit>::iterator itt;
		for (itt = vect.begin(); itt != vect.end(); ++itt)
		{
			if (!itt->newvalue)
			{
				memcpy(buf + len, itt->p, itt->len);
				len += itt->len;
			}
			else
			{

				memcpy(buf + len, newvalue, len2);
				len += len2;
			}
		}
		str = buf;
		delete[] buf;
	}

}

void RemoveAll(std::string wst)
{
	std::string  wstCurrentFindPath;
	wstCurrentFindPath.assign(wst);
	wstCurrentFindPath.append("\\*.*");

	std::string  wstCurrentFile;
	WIN32_FIND_DATA wfd;
	HANDLE h = FindFirstFile(wstCurrentFindPath.c_str(), &wfd);
	if (h == INVALID_HANDLE_VALUE)
	{
		return;
	}
	do
	{
		if (lstrcmp(wfd.cFileName, ".") == 0 ||
			lstrcmp(wfd.cFileName, "..") == 0)
		{
			continue;
		}
		wstCurrentFile.assign(wst);
		wstCurrentFile.append("\\");
		wstCurrentFile.append(wfd.cFileName);
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			RemoveAll(wstCurrentFile);
		}
		else
		{
			DeleteFile(wstCurrentFile.c_str());
		}
	} while (FindNextFile(h, &wfd));
	FindClose(h);
	RemoveDirectory(wst.c_str());
}

std::string GetMidData(const char* str,const  char* spa, const char* spb)
{
	std::string outstr;
	char* findfirst = (char*)strstr(str, spa);
	if (findfirst)
	{
		char* findsecond = (char*)strstr(findfirst + strlen(spa), spb);
		if (findsecond)
		{
			int len = findsecond - findfirst - strlen(spa);
			char* buf = new char[len + 1];
			buf[len] = 0;
			strncpy(buf, findfirst + strlen(spa), len);
			outstr = buf;
			delete[] buf;
		}
		else
		{
			return "";
		}
	}
	else
	{
		return "";
	}

	return outstr;

}
int CreatePath(const char* folder, bool all)
{
	int errcount = 0;

	if (_access(folder, 0) == 0)
		::SetFileAttributes(folder, 0);

	if (all)
	{
		char path[1024];
		memset(path, 0, 1024 * sizeof(char));

		for (int i = 0; folder[i]; ++i)
		{
			if (folder[i] == '\\' || folder[i] == '/')
				errcount += CreatePath(path, false);
			path[i] = folder[i];
		}
		errcount += CreatePath(path, false);
	}
	else if (_access(folder, 0) != 0)
	{
		if (!::CreateDirectory(folder, NULL))
		{
			AWriteLog("work","CreateDirectory [%s]errcode:%d", folder,GetLastError());
			errcount++;
		}
			
	}

	return errcount;
}
int GFileModitm(const char* filename)
{
	struct stat file;

	int result = stat(filename, &file);

	if (result == 0)
	{
		return file.st_mtime;
		//return file.st_size;
	}
	else
	{
		return -1;
	}

	return -1;
}

unsigned long GFileSize(const char* filename)
{
	struct stat file;

	int result = stat(filename, &file);

	if (result == 0)
	{
	
		return file.st_size;
	}
	else
	{
		return 0;
	}

	return 0;
}

//
char* CReadNuFromFile(const char* filename, unsigned int& len)
{

	char* outbuf = NULL;

	FILE* pfile = fopen(filename, "rb");
	if (pfile)
	{
		fseek(pfile, 0, SEEK_END);
		int tollen = ftell(pfile);
		fseek(pfile, 0, SEEK_SET);
		if (len > tollen)
		{
			fclose(pfile);
			return outbuf;
		}
		outbuf = new char[len + 1];
		outbuf[len] = 0;
		fread(outbuf, 1, len, pfile);
		fclose(pfile);
	}
	return outbuf;
}
bool getfileinfo(const char* filename, long long& len)
{
	len = 0;
	FILE* pfile = fopen(filename, "rb");
	if (pfile)
	{
		fseek(pfile, 0, SEEK_END);
		len = ftell(pfile);
		fseek(pfile, 0, SEEK_SET);
		return true;
	}
	return false;
}
//
char* CReadFromFile(const char* filename, unsigned int& len)
{
	len = 0;
	char* outbuf = NULL;

	FILE* pfile = fopen(filename, "rb");
	if (pfile)
	{
		fseek(pfile, 0, SEEK_END);
		len = ftell(pfile);
		fseek(pfile, 0, SEEK_SET);
		if (len == 0)
		{
			fclose(pfile);
			return outbuf;
		}
		outbuf = new char[len + 1];
		outbuf[len] = 0;
		fread(outbuf, 1, len, pfile);
		fclose(pfile);
	}
	return outbuf;
}

bool WriteToFile(const char* filename, const char* buf, unsigned int len)
{
	FILE* pfile = fopen(filename, "wb");
	if (pfile)
	{
		fwrite(buf, 1, len, pfile);
		fclose(pfile);
	}
	else
	{
		return false;
	}
	return true;
}
bool IsPathExist(const char* csPath)
{
	int nRet = _access(csPath, 0);

	return 0 == nRet || EACCES == nRet;

}


std::string GetLogDir()
{

	std::string rootpath = GetCurWorkRootDir();
	char tmp[512];
	memset(tmp, 0, 512);

	sprintf(tmp, "%s\\log\\", rootpath.c_str());



	return std::string(tmp);


}


std::string GetLogName(const char* logname)
{

	std::string rootpath = GetCurWorkRootDir();
	char tmp[512];
	memset(tmp, 0, 512);

	sprintf(tmp, "%s\\log\\", rootpath.c_str());

	if (_access(tmp, 0) != 0)
	{
		CreatePath(tmp, true);

	}
	time_t t = time(NULL);
	char desstr[512];
	memset(desstr, 0, 512);
	int iYear = 0;
	int iMonth = 0;
	int iDay = 0;
	int iHour = 0;
	int iMinut = 0;
	int iSecon = 0;
	struct tm stime = { 0 };
	stime = *localtime(&t);
	iYear = stime.tm_year + 1900;
	iMonth = stime.tm_mon + 1;
	iDay = stime.tm_mday;
	iHour = stime.tm_hour;
	iMinut = stime.tm_min;
	iSecon = stime.tm_sec;

	sprintf(desstr, "%s%s_%d%02d%02d.log", tmp, logname, iYear, iMonth, iDay);

	return desstr;


}

void MGetCurTime(char* curTime, bool btype)
{
	time_t lTime;
	struct tm* tmTime = NULL;
	char szOverdue[32];

	time(&lTime);
	tmTime = localtime(&lTime);

	if (btype)
		strftime(curTime, 32, "%Y-%m-%d %H:%M:%S", tmTime);
	else
		strftime(curTime, 32, "%Y%m%d%H%M%S", tmTime);

	//	cout<<curTime<<endl;

}

void MGetNewtmIOSCurTime(char* curTime)
{
	time_t lTime;
	struct tm* tmTime = NULL;
	char szOverdue[32];

	time(&lTime);
	tmTime = localtime(&lTime);

	strftime(curTime, 32, "%Y-%m-%dT%H:%M:%SZ", tmTime);

	//	cout<<curTime<<endl;

}

std::string MGetIOSDateFromTimet(std::string lTime)
{
	//time_t lTime;
	if (lTime == "0"|| lTime.length()==0)
	{
		return "";
	}
	if (lTime.length() == 13)
	{
		lTime = lTime.substr(0,10);
	}
	time_t tmNow = atoi(lTime.c_str());
	tm* ptmNow = localtime(&tmNow);  

	char curTime[64] = { 0 };
	strftime(curTime, 32, "%Y-%m", ptmNow);
	return std::string(curTime);


	//	cout<<curTime<<endl;

}
int my_strnicmp(const char* dst, const char* src, int count)
{
	int ch1, ch2;
	do
	{
		if (((ch1 = (unsigned char)(*(dst++))) >= 'A') && (ch1 <= 'Z'))
			ch1 += 0x20;
		if (((ch2 = (unsigned char)(*(src++))) >= 'A') && (ch2 <= 'Z'))
			ch2 += 0x20;
	} while (--count && ch1 && (ch1 == ch2));
	return (ch1 - ch2);
}



bool ComPareLeft(std::string fstr, std::string sstr)
{
	if (fstr.length() < sstr.length())
	{
		return false;
	}

	if (my_strnicmp(fstr.c_str(), sstr.c_str(), sstr.length()) == 0)
	{
		return true;
	}
	return false;
}
bool ComParRight(std::string fstr, std::string sstr)
{
	int flen = fstr.length();
	int slen = sstr.length();
	if (flen < slen)
	{
		return false;
	}
	std::string ffstr = fstr.substr(flen- slen, slen);
	if (my_strnicmp(ffstr.c_str(), sstr.c_str(), sstr.length()) == 0)
	{
		return true;
	}
	return false;
}

void makeLower(std::string& str)
{
	if (str.empty())
	{
		return;
	}

	int len = str.size();

	for (int m = 0; m < len; ++m)
	{
		if (str[m] >= 65 && str[m] <= 90)
		{
			str[m] += 32;
		}
	}
}

void makeUpper(std::string& str)
{
	if (str.empty())
	{
		return;
	}

	int len = str.size();

	for (int m = 0; m < len; ++m)
	{
		if (str[m] >= 97 && str[m] <= 122)
		{
			str[m] -= 32;
		}
	}
}

char* utf8Format(char* pByte, int dwSize)
{

	if (dwSize >= 3 && pByte[0] == 0xEF && pByte[1] == 0xBB && pByte[2] == 0xBF)
	{
		pByte += 3; dwSize -= 3;
	}
	DWORD nWide = ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pByte, dwSize, NULL, 0);

	LPWSTR w_str = static_cast<LPWSTR>(malloc((nWide + 100) * sizeof(WCHAR)));
	::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pByte, dwSize, w_str, nWide);
	w_str[nWide] = L'\0';

	DWORD wide = ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)w_str, nWide, NULL, 0, NULL, NULL);

	char* m_pstrXML = static_cast<LPTSTR>(malloc((wide + 100) * sizeof(TCHAR)));
	::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)w_str, nWide, m_pstrXML, wide, NULL, NULL);
	m_pstrXML[wide] = ('\0');
	free(w_str);


	return m_pstrXML;

}

std::string FindDataFromMap(std::map<std::string, std::string>& dmap, const char* key)
{
	std::string str="";
	std::map<std::string, std::string>::iterator itt;
	itt = dmap.find(key);
	if (itt != dmap.end())
	{
		str = itt->second;
	}

	return str;
}

#define BUFFSIZE 10240
int copyfile(const char* srcfile, const char* dstfile)
{
	FILE* fin;  
	FILE* fout; 
	char* buff; 

	int ret;   


	fin = fopen(srcfile, "rb");
	if (NULL == fin)  
	{
		return -2;
	}


	fout = fopen(dstfile, "wb");
	if (NULL == fout) 
	{

		fclose(fin);
		return -1;
	}

	buff = (char*)malloc(BUFFSIZE);
	if (NULL == buff) 
	{
		fprintf(stderr, "molloc failed\n");
		fclose(fin);
		fclose(fout);
		return -3;
	}


	while (1)
	{

		ret = fread(buff, 1, BUFFSIZE, fin);


		if (ret != BUFFSIZE)
		{
			fwrite(buff, ret, 1, fout);
		}
		else
		{
			fwrite(buff, BUFFSIZE, 1, fout);
		}

		if (feof(fin))
			break;
	}

	fclose(fin);
	fclose(fout);

	free(buff);
	return 0;

}

int formatiostm(std::string iostm)
{

	if (iostm.length() > 9)
	{
		iostm = iostm.substr(0, 9);
	}
	else if (iostm.length() < 8)
	{
		return 0;
	}
	if (iostm[0] == '-')
	{
		return 0;
	}

	unsigned int tm = 0;
	sscanf(iostm.c_str(), "%u", &tm);
	tm += 978307200;


	return tm;
}

int64_t formatiostm_wmEx(std::string iostm)
{
	if (iostm.empty())
		return 0;
	double itmp = atof(iostm.c_str()) ;

	int64_t itm = int64_t(itmp);
	itm += 978307200;
	if (itm > 1860874262)
	{
		return time(0);
	}

	return itm;


}
int64_t formatiostm_wm(std::string iostm)
{
	if (iostm.empty())
		return 0;
	double itmp=atof(iostm.c_str())*1000;

	int64_t itm = int64_t(itmp);
	itm += 978307200000;


	return itm;


}
int getnowiostm()
{
	int tm=time(0);
	return tm - 978307200;

}

std::string getiostm(const char* wwtm)
{

	long long retval;
	int i;

	retval = 0;
	for (; *wwtm; wwtm++) {
		retval = 10 * retval + (*wwtm - '0');
	}
	retval = retval - 978307200000;
	if (retval < 0)
	{
		return "0";
	}
	int iostm=retval / 1000;
	char buf[12] = {0};
	sprintf(buf,"%d", iostm);

	return std::string(buf);

}


const char hex_table[] = {
'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'
};
void to_hex(char* s, int l, char* d)
{
	while (l--)
	{
		*(d++) = hex_table[*s >> 4];
		*(d++) = hex_table[*(s++) & 0x0f];
	}
}

std::string bytestohexstringPrint(char* bytes, int bytelength)
{
	if (!bytes)
		return "";
	std::string str("");
	std::string str2("0123456789abcdef");
	int index = 0;
	for (int i = 0; i < bytelength; i++) {
		int b;
		b = 0x0f & (bytes[i] >> 4);
		char s1 = str2.at(b);
		str.append(1, str2.at(b));
		b = 0x0f & bytes[i];
		str.append(1, str2.at(b));
		char s2 = str2.at(b);
		str.append(1, ' ');
		index += 1;
		if (index % 16 == 0)
		{
			str.append(1, '\n');
		}
	}

	AWriteLog("socket","str:\r\n%s\r\n", str.c_str());
	return str;
}

std::string bytestohexstring(char* bytes, int bytelength)
{
	if (!bytes)
		return "";
	std::string str("");
	std::string str2("0123456789abcdef");
	for (int i = 0; i < bytelength; i++) {
		int b;
		b = 0x0f & (bytes[i] >> 4);
		char s1 = str2.at(b);
		str.append(1, str2.at(b));
		b = 0x0f & bytes[i];
		str.append(1, str2.at(b));
		char s2 = str2.at(b);
	}
	return str;
}
int hexchr2bin(const char hex, char* out)
{
	if (out == NULL)
		return 0;

	if (hex >= '0' && hex <= '9') {
		*out = hex - '0';
	}
	else if (hex >= 'A' && hex <= 'F') {
		*out = hex - 'A' + 10;
	}
	else if (hex >= 'a' && hex <= 'f') {
		*out = hex - 'a' + 10;
	}
	else {
		return 0;
	}

	return 1;
}
size_t hexs2bin(const char* hex, char** out)
{
	size_t len;
	char   b1;
	char   b2;
	size_t i;

	if (hex == NULL || *hex == '\0' )
		return 0;

	len = strlen(hex);
	if (len % 2 != 0)
		return 0;
	len /= 2;

	*out = (char*)malloc(len);
	memset(*out, 'A', len);
	for (i = 0; i < len; i++) {
		if (!hexchr2bin(hex[i * 2], &b1) || !hexchr2bin(hex[i * 2 + 1], &b2)) {
			return 0;
		}
		(*out)[i] = (b1 << 4) | b2;
	}
	return len;
}

std::string GetFilePath(const char* filename)
{

	char* p = strrchr((char*)filename, '\\');
	if (p)
	{
		std::string suffix = filename;
		int index = strlen(p);

		suffix = suffix.substr(0, (strlen(filename) - index + 1));

		int len = suffix.size();
		return suffix;
	}
	else
	{
		return filename;
	}
}

std::string GetFilePath_Unxi(const char* filename)
{

	char* p = strrchr((char*)filename, '/');
	if (p)
	{
		std::string suffix = filename;
		int index = strlen(p);

		suffix = suffix.substr(0, (strlen(filename) - index + 1));

		int len = suffix.size();
		return suffix;
	}
	else
	{
		return filename;
	}
}

std::string GetFileName(const char* file)
{
	std::string str;
	char* p = strrchr((char*)file, '\\');
	if (p)
	{
		p = p + 1;
		str = p;
	}
	else
	{
		str = file;
	}
	return str;

}
std::string GetFileName_Unxi(const char* file)
{
	std::string str;
	char* p = strrchr((char*)file, '/');
	if (p)
	{
		p = p + 1;
		str = p;
	}
	else
	{
		str = file;
	}
	return str;

}

#define BUFSIZE 4096
#define VARNAME TEXT("Path")
bool MySetEnvironment(std::string mypath)
{
	DWORD dwRet, dwErr;
	LPTSTR pszOldVal;

	DWORD dwFlags = 0;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	BOOL fExist, fSuccess;

	pszOldVal = (LPTSTR)malloc(BUFSIZE * sizeof(TCHAR));
	if (NULL == pszOldVal)
	{
		printf("Out of memory\n");
		return FALSE;
	}

	dwRet = GetEnvironmentVariable(VARNAME, pszOldVal, BUFSIZE);
	if (0 == dwRet)
	{
		dwErr = GetLastError();
		if (ERROR_ENVVAR_NOT_FOUND == dwErr)
		{
			printf("Environment variable does not exist.\n");
			fExist = FALSE;
		}
	}
	else if (BUFSIZE < dwRet)
	{
		pszOldVal =
			(LPTSTR)realloc(pszOldVal, dwRet * sizeof(TCHAR));
		if (NULL == pszOldVal)
		{
			printf("Out of memory\n");
			return false;
		}
		dwRet = GetEnvironmentVariable(VARNAME, pszOldVal, dwRet);
		if (!dwRet)
		{
			printf("GetEnvironmentVariable failed (%d)\n",
				GetLastError());
			return false;
		}
		else fExist = TRUE;
	}
	else fExist = TRUE;

	std::string tmpmypath;
	if (fExist)
	{
		tmpmypath = pszOldVal;
		tmpmypath += ";";
		tmpmypath += mypath;
	}
	else
	{
		tmpmypath = mypath;
	}
	if (!SetEnvironmentVariable(VARNAME, tmpmypath.c_str()))
	{
		printf("SetEnvironmentVariable failed (%d)\n",
			GetLastError());
		return false;
	}

	return true;
}
#define GUID_LEN 64
std::string generate()
{
	char buf[GUID_LEN] = { 0 };
	GUID guid;

	if (CoCreateGuid(&guid))
	{
		return std::move(std::string(""));
	}

	sprintf(buf,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);

	return std::move(std::string(buf));
}
char* GetSha1StrA(const char* buf, int len)
{

	char hash[48] = { 0 };
	SHA1Context sha;
	SHA1Reset(&sha);
	SHA1Input(&sha, (const unsigned char*)buf, len);
	if (SHA1Result(&sha))
	{
		unsigned int i = 0, j = 0;
		while (i < 5)
		{
			hash[j] = sha.Message_Digest[i] & 0xFF;
			hash[j + 1] = (sha.Message_Digest[i] >> 8) & 0xFF;
			hash[j + 2] = (sha.Message_Digest[i] >> 16) & 0xFF;
			hash[j + 3] = (sha.Message_Digest[i] >> 24) & 0xFF;
			i++;
			j += 4;
		}

		return hash;
	}
	else
	{
		return NULL;
	}

}
std::string GetSha1Str(char* buf, int len)
{

	char hash[48] = { 0 };
	SHA1Context sha;
	SHA1Reset(&sha);
	SHA1Input(&sha, (const unsigned char*)buf, len);

	if (SHA1Result(&sha))
	{

		sprintf(hash, "%08x%08x%08x%08x%08x", sha.Message_Digest[0], sha.Message_Digest[1], sha.Message_Digest[2], sha.Message_Digest[3], sha.Message_Digest[4]);
		return std::string(hash);
	}
	else
	{
		return "";
	}

}
std::string GetMd5cTobase64(unsigned char* buf, unsigned int len)
{

	int i;

	unsigned char md5_value[MD5_SIZE];
	MD5_CTX md5;
	MD5Init(&md5);
	MD5Update(&md5, buf, len);
	MD5Final(&md5, md5_value);
	std::string retstr = "";
	Base64::EncodeStr((const char*)md5_value,16,retstr);
	return retstr;


}
std::string GetMd5c(unsigned char* buf, unsigned int len)
{

	int i;

	unsigned char md5_value[MD5_SIZE];
	MD5_CTX md5;
	MD5Init(&md5);
	MD5Update(&md5, buf, len);
	MD5Final(&md5, md5_value);

	return bytestohexstring((char*)md5_value, 16);


}
bool myget_file_md5str(const char* file_path, std::string& md5str)
{
	char md5_str[40] = { 0 };

	int i;

	int ret;
	unsigned char data[READ_DATA_SIZE];
	unsigned char md5_value[MD5_SIZE];
	MD5_CTX md5;
	FILE* fp = fopen(file_path, "rb");
	if (NULL == fp)
	{
		perror("open");
		return false;
	}
	MD5Init(&md5);

	int bytes;
	while ((bytes = fread(data, 1, 1024, fp)) != 0)
	{
		MD5Update(&md5, data, bytes);
	}
	fclose(fp);

	MD5Final(&md5, md5_value);

	for (i = 0; i < MD5_SIZE; i++)
	{
		_snprintf(md5_str + i * 2, 2 + 1, "%02x", md5_value[i]);
	}
	md5_str[MD5_STR_LEN] = '\0'; // add end
	md5str = md5_str;
	return true;
}

std::string GetSuffix(const char* filename, bool makelow)
{

	char* p = (char*)strrchr(filename, '.');
	if (p)
	{
		p = p + 1;
		std::string suffix = p;
		int len = suffix.size();
		if (makelow)
		{
			for (int m = 0; m < len; m++)
			{
				if (suffix[m] >= 65 && suffix[m] <= 90)
				{
					suffix[m] += 32;
				}
			}
		}
		return suffix;
	}
	else
	{
		return "";
	}
}
int myrandom(int s,int m) {

	return (s+rand() % (m-s));
}


int nb_system(const char* cmd)
{
	STARTUPINFOA si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof STARTUPINFO;

	PROCESS_INFORMATION pi = { 0 };
	if (CreateProcessA(NULL, (LPSTR)cmd, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{

		WaitForSingleObject(pi.hProcess, INFINITE);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else
	{
		return -1;
	}
	return 0;
}

 bool AllisNum(std::string str)
 {
	     for (int i = 0; i < str.size(); i++)
		     {
		         int tmp = (int)str[i];
		         if (tmp >= 48 && tmp <= 57)
			         {
			             continue;
			         }
		         else
			         {
			             return false;
			         }
		     }
	     return true;
}
int  EnumPath_Clear(const char* Path)
{
	WIN32_FIND_DATA FindData;
	HANDLE hError;
	int nowtime = time(0);
	char FilePathName[512];

	char FullPathName[512];
	strcpy(FilePathName, Path);
	strcat(FilePathName, "\\*.*");
	hError = FindFirstFile(FilePathName, &FindData);
	if (hError == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	while (::FindNextFile(hError, &FindData))
	{
	
		if (strcmp(FindData.cFileName, ".") == 0
			|| strcmp(FindData.cFileName, "..") == 0)
		{
			continue;
		}

		wsprintf(FullPathName, "%s\\%s", Path, FindData.cFileName);


		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			EnumPath_Clear(FullPathName);
		}
		else
		{
		 //FullPathName
			int iCreateTime=0;
			std::string  mfile = std::string(FullPathName);
			if (GetFileInfo(mfile,iCreateTime)==0)
			{
				if (iCreateTime + 1728000 < nowtime)
				{
					remove(FullPathName);
				}
			}

		}
	}
	FindClose(hError);
	return 0;
}
int GetFileInfo(std::string& strPath, int& iCreateTime)
{
	struct _stat tmpInfo;
	if (_stat(strPath.c_str(), &tmpInfo) != 0)
	{
		return -1;
	}
	iCreateTime = static_cast<int>(tmpInfo.st_ctime);

	return 0;
}
typedef BOOL(WINAPI* LPFN_GLPI)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);
DWORD CountSetBits(ULONG_PTR bitMask)
{
	DWORD LSHIFT = sizeof(ULONG_PTR) * 8 - 1;
	DWORD bitSetCount = 0;
	ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;
	DWORD i;

	for (i = 0; i <= LSHIFT; ++i)
	{
		bitSetCount += ((bitMask & bitTest) ? 1 : 0);
		bitTest /= 2;
	}
	return bitSetCount;
}
char* toByteArray(int i) {
	char*bt = new char[5];
	bt[0] = (byte)(0xff & i);
	bt[1] = (byte)((0xff00 & i) >> 8);
	bt[2] = (byte)((0xff0000 & i) >> 16);
	bt[3] = (byte)((0xff000000 & i) >> 24);
	bt[4] = 0;
	return bt;
}
