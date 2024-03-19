#pragma once

#include <string>
#include <map>
typedef unsigned long long uint64_t;

enum plist_format_t {
	PLIST_FORMAT_XML,
	PLIST_FORMAT_BINARY
};

struct INFOPLIST {
	std::string BuildVersion;
	std::string DeviceName;
	std::string DisplayName;
	std::string GUID;
	std::string IMEI;
	std::string LASTBACKUPDATE;
	int32_t sec;
	int32_t usec;

	std::string ProductType;
	std::string ProductVersion;
	std::string SerialNumber;
	std::string TargetIdentifier;
	std::string TargetType;
	std::string UniqueIdentofier;
	std::string iTunesVersion;
	INFOPLIST() {
		TargetType = "Device";
		iTunesVersion = "12.7.3";
	};



};

class plistapi
{
public:
	plistapi() {
		mapptype = 0;
	}
public:

	std::string buildManifestfilecolunm(uint64_t Birth, int GroupID, int InodeNumber, uint64_t LastModified, uint64_t LastStatusChange, int Mode, int ProtectionClass, int size, const char* RelativePathstr);

	int buileinfoplist(const char* srcfilename, const char* dstfilename, int apptype = 0);
	int writenewiosinfplist(const char* dstfilename, INFOPLIST oINFOPLIST);
	int readiosinfoplistfromold(const char* filename,  INFOPLIST& oINFOPLIST);

	int readiosdeviceinfo(const char* deviceid, INFOPLIST& oINFOPLIST);
	void SpliteIosDeviceInfo(const char* msgbuf, std::map<std::string, std::string>& dmap);
	int buileManifestplist_test(const char* srcfilename);
	int buileStatusplist(const char* dstfilename, int sec, int usec);
	int buileinfoplistA(const char* deviceid,const char* dstfilename, const char* meplist, int sec, int usec, int apptype=0);
	int buileManifestplistA(const char* dstfilename, INFOPLIST oINFOPLIST);
	int buileManifestplist(const char* srcfilename, const char* dstfilename);

	int mapptype;
};

int plisttestapi();