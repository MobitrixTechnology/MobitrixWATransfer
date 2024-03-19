#pragma once

#include <string>
#include <vector>
#include <map>
#include <windows.h>


struct sessioninfo {
	int id;//Z_PK
	int sessiontype;//ZSESSIONTYPE
	std::string subject;//ZPARTNERNAME
	int last_message_table_id;//ZLASTMESSAGE
	int message_table_id;//ZMESSAGECOUNTER
	std::string key_remote_jid;//ZCONTACTJID
	int64_t creation;//ZLASTMESSAGEDATE
	int gtype;//0  1:
	int archived;
	int jid_id;//
};
struct groupmemberinfo {
	int id;//Z_PK
	std::string gjid;
	int sessionid;//ZCHATMESSION
	int admin;//ZISADMIN
	int pending;//ZCONTACTABID
	std::string sent_sender_key;//ZSENDERKEYSENT
	std::string jid;//ZMEMBERJID
};
struct messengesinfo {
	int _id;//2-
	std::string key_remote_jid;//
	int key_from_me;//ZISFROMME
	std::string key_id;//ZSTANZAID
	int status;//ZMESSAGESTATUS 
	int need_push;//0
	int starred;
	std::string data;//ZTEXT
	int64_t timestamp;//ZMESSAGEDATE
	std::string media_url;//ZMEDIAURL
	std::string media_mime_type;//ZVCARDSTRING
	int media_wa_type;//ZMESSAGETYPR
	std::string media_size;//ZFILESIZE
	std::string media_name;//ZMEDIALOCALPATH
	std::string media_hash;//ZVCARDNAME
	int media_duration;//ZMOVIEDURATION
	int origin;//ZMEDIAORIGIN
	std::string latitude;//ZLATITUDE
	std::string longitude;//ZLONGGITUDE
	char* thum_image;//
	int thum_image_len;
	std::string remote_resource;//
	int64_t received_timestamp;//ZMESSAGEDATE
	int64_t send_timestamp;//ZSENTDATE
	int64_t receipt_server_timestamp;//ZMESSAGEDATE
	char* raw_data;
	int raw_data_len;
	std::string recipient_count;
	std::string media_caption;//ZAUTHORNAME  ZTITLE

	std::string attlocalfile;
	std::string thlocalfile;

	std::string mediadatafile;//

	std::string remoteattfile;
	std::string remotethfile;
	bool ishadmedia;

	std::string sender_jid;
};
struct messengesmediainfo {
	std::string rejid;
	int message_row_id;
	int chat_row_id;
	int autotransfer_retry_enable;//0
	std::string media_job_uuid;
	int transferred;//1
	int transcoded;//1
	std::string file_path;
	std::string file_size;
	int suspicious_content;//0
	int trim_from;//0
	int trim_to;//0
	int face_x;//0
	int face_y;//0
	std::string media_key;
	std::string media_key_timestamp;
	std::string width;
	std::string height;
	int has_streaming_sidecar;
	int git_attribution;//0
	std::string thumbnail_height_width_ratio;
	std::string direct_path;
	int first_scan_length;
	std::string message_url;
	std::string mime_type;
	std::string file_length;
	std::string media_name;
	std::string file_hash;
	int media_duration;
	int page_count;
	std::string enc_file_hash;
	int is_animated_sticker;

	messengesmediainfo()
	{
		 autotransfer_retry_enable = 0;//0
		 transferred = 1;//1
		 transcoded = 1;//1
		 trim_from = 0;//0
		 trim_to = 0;//0
		 face_x = 0;//0
		 face_y = 0;//0
		 git_attribution=0;//0
		 thumbnail_height_width_ratio = "0.0";
		 page_count = 0;
		 media_duration = 0;
		 is_animated_sticker = 0;
	}

};


struct attserializeinfo {
	std::string filename;
	std::string filesize;

	std::string latitude;//facex
	std::string longitude;//facey
};
#define sleep(n) Sleep(n*1000)

void usleep(DWORD waitTime);
std::string GetCurWorkRootDir();
bool AllisNum(std::string str);
void SplitStoV(char* buf, int len, const char* e, std::vector<std::string>& sVector, bool isdelempty=true);
void Replace(std::string& str, const char* oldvalue, const char* newvalue);
std::string GetMidData(const char* str, const char* spa, const char* spb);
int CreatePath(const char* folder, bool all = true);
void RemoveAll(std::string wst);

bool IsPathExist(const char* csPath);
unsigned long GFileSize(const char* filename);
char* CReadFromFile(const char* filename, unsigned int& len);
bool getfileinfo(const char* filename, long long& len);
char* CReadNuFromFile(const char* filename, unsigned int& len);
bool WriteToFile(const char* filename, const char* buf, unsigned int len);
void MGetCurTime(char* curTime, bool btype=false);
void MGetNewtmIOSCurTime(char* curTime);

std::string MGetIOSDateFromTimet(std::string lTime);
std::string GetLogName(const char* logname);
int my_strnicmp(const char* dst, const char* src, int count);
bool ComPareLeft(std::string fstr, std::string sstr);
bool ComParRight(std::string fstr, std::string sstr);
void makeLower(std::string& str);
void makeUpper(std::string& str);


std::string FindDataFromMap(std::map<std::string, std::string>& dmap, const char* key);
int copyfile(const char* srcfile, const char* dstfile);
std::string GetFilePath(const char* filename);
std::string GetFilePath_Unxi(const char* filename);
std::string GetFileName_Unxi(const char* file);
std::string GetFileName(const char* file);
int formatiostm(std::string iostm);
std::string getiostm(const char* wwtm);
int64_t formatiostm_wm(std::string iostm);
int64_t formatiostm_wmEx(std::string iostm);

//void to_hex(char* s, int l, char* d);
bool MySetEnvironment(std::string mypath);
std::string bytestohexstring(char* bytes, int bytelength);

std::string bytestohexstringPrint(char* bytes, int bytelength);
size_t hexs2bin(const char* hex, char** out);
std::string generate();
std::string GetMd5c(unsigned char* buf, unsigned int len);
std::string GetMd5cTobase64(unsigned char* buf, unsigned int len);
bool myget_file_md5str(const char* file_path, std::string& md5str);
std::string GetSha1Str( char* buf , int len);
char* GetSha1StrA(const char* buf, int len);
std::string GetSuffix(const char* filename, bool makelow=false);
char* toByteArray(int i);
int myrandom(int s, int m);
int GFileModitm(const char* filename);
int getnowiostm();
int nb_system(const char* cmd);
;

int GetFileInfo(std::string& strPath, int& iCreateTime);
int  EnumPath_Clear(const char* Path);
std::string GetLogDir();
char* utf8Format(char* pByte, int dwSize);
