#pragma once


#include "publicfun.h"
#include "wasqliteparsing.h"
#include "iosdatastruct.h"
#include "BaseLock.h"
typedef struct DEAL_CHAT_INFO {
	void* androidtoios;
	std::string mmsgstoredbfile;
	int tindex;
	std::string tablename;
}*lpdealchatinfo, dealchatinfo;
class android2ios
{

public:

	int start_work(const char* androiddbpath, const char* androidmediapath, std::string iospath, std::string datapath, int watype);
	int read_work();
	int write_work();
	int dealmessagepk();

	//Z_METADATA
	int readmetadata();

	int readchatsessionfromiphone();
	int readchatsesisonfromandroid();
	int readchatsesisonfromandroidEx();
	int readjidfromdb();
	bool isexitchatsession(std::string jid);
	int getsessionpk(std::string jid);
	std::string getsubjectStr(const char* jid);
	int setgroupinftosession(int seid,int groupinfoid);
	int updatesort(int sessionid, int msort);
	bool getisgroupstatus(std::string kjid,bool &isgroup);
	bool updatechat(int zpk,int date,int chatpk);
	//groupinfo
	int readgrouinfofromiphone();
	int readgrouinffromandroid();
	int readgrouinffromandroidEx();
	bool isexitgrougid(std::string jid);

	int readgroumemberinfofromiphone();
	int readgroumemberinffromandroid();
	int readgroumemberinffromandroidA();
	int readgroumemberinffromandroidEx();
	bool isexitgrougmember(std::string jid_chatid);
	int getzpkgrougmember(std::string jid_chatid);
	int addgroupmemeber(std::string gjid, std::string jid);
	int insertothergroumemberEx(std::string jid, int sessionid);
	int getzpkgrougmemberEx(std::string jid_chatid, std::string gjid, std::string jid);
	int readmthumbmap();

	int readmthumbmapA();
	int readmthumbmapB();

	int readmediafromiphone();
	int readmessagechatfromiphone();
	int readmessageinffromandroid(int start);
	int dealmessagechatthread_newmessage(int start);

	std::string getchatromte_row_jid(const char* cid);
	std::string getjid_romte_row_jid(const char* jjid);
	int getmediadatailinfo(wasqliteparsing& osqliteparser, int message_id, std::string& filepath, std::string& width, std::string& height, std::string& media_name, std::string& file_size);
	int getmediadatailinfo_new(wasqliteparsing& osqliteparser, int message_id, std::string& filepath, std::string& width, std::string& height, std::string& media_name, std::string& file_size, std::string& media_duration, std::string& media_url, std::string& media_mime_type);
	int getmediadatailinfoA(std::string chatid,std::string  hexmessage, std::string& filepath, std::string& width, std::string& height);

	int getthumpicinfo(wasqliteparsing& osqliteparser, std::string keyid, std::string& thumdata);
	char* getthumpicinfo_B(std::string keyid, int& outlen);
	int getthumpicinfo_messagea(int message_rowid, std::string& thumdata);


	int initrestoredata();

	int initmprimarymap();
	int updateprimarymap(int index,int max);
	int initrestoreplist();

	int readselfjid();


	int insertManifestdb_attfile(const char* localfilename, const char* filename);

	//1.session
	int writesessiontable();
	//2. groupinfo
	int writegroupinfotable();
	//3. groupmember
	int writegroupmemberinfotable();
	//4.chat
	int writeamessageinfotable();
	//5.media
	int writemediainfotable();
	//6.mprimarymap
	int writeprimaryinfo();
	//Z_METADATA
	int writezmetadata();
	//8.mnewManifestdb
	int writeManifestdb();

	int readoldManifestdb();
	int readoldManifestdbA();

	int tranoldiphonefile();

	int buildrestoreplist();


	int insertnewdbinfo();
	int insertpackageinfo();
	bool ishadiosmsgdb;



	int getstartindex();
	void tostopthread();
	Mutex m_indexMutex;
	int m_chatlimitindex;
	int m_runthreadnumber;
	int dealmessagechatthread(int start);
	Mutex m_mediaindexMutex;
	Mutex m_chatsessionMutex;

	int m_totalnum;
	std::string m_chattable;
private:
	int m_watype;
	std::string m_androiddatapath;//Android
	std::string mChatStoragedbfile;//
	std::string mNewChatStoragedbfile;//
	std::string mmsgstoredbfile;
	std::string mwadbfile;
	std::string mxmlfile;
	std::string mandroidpath;
	std::string  mlimxmlfile;
	std::string androidmainpath;
	std::string mpermediapath;
	std::string mrestorepath;
	std::string miospath;
	std::string mManifestdb;
	std::string mnewManifestdb;

	std::string mold_infoplist;
	std::string mold_Manifestplist;
	std::string mold_Statusplist;

	std::string mnew_infoplist;
	std::string mnew_Manifestplist;
	std::string mnew_Statusplist;

	std::string m_iosdevice;
	std::string mideviceid;
	std::map<int, std::string> mchatidmap;
	std::map<int, std::string> mjididmap;
	//MAP  ZCONTACTJID
	std::map<int,ZWACHATSESSION> mchatsessionmap;
	std::map<std::string, int> mcheckchatmessionma;
	//MAP ZSUBJECTOWNERJID
	std::map<int, ZWAGROUPINFO> mgroupinfomap;
	std::map<std::string, int> mcheckgroupinfoma;
	//MAP GROUPMEMBERINFO
	std::map<std::string, ZWAGROUPMEMBER> mgroupmembermap;
	//std::map<std::string, ZWAMESSAGE> mwamessagemap;

	std::map<int, ZWAMESSAGE*> mwamessagemap;
	std::map<int, ZWAMEDIAITEM*> mwamediamap;

	std::map<std::string, int> msorftmap;
	void getsort(std::string jid,int& sort);

	//std::map<std::string, std::string> mthumbmap;

	std::map<std::string, wablockbytes> mthumbmap;
	std::map<int, std::string> mthumbmap_a;
	Z_METADATA mZ_METADATA;
	std::map<int, ZPRIMARYKEY> mprimarymap;

	int mchatmaxzpk = 0;
	int mgrouinfomaxpk = 0;
	int mgroupmemberindex = 0;
	int mgroupmemberopt = 0;


	int mmediazpk = 0;
	int mmessagezpk = 0;
	int mmessageZDOCID = 0;
	int mmessageZSORT = 1;

	std::string mselfjid;
	std::string mphonestr;

	int mInodeNumber;

	std::map<int, ManifestFilesInfo>  manifestfilemap;
	std::map<std::string, int>  manifestfilecheck;
	int manifestfileindex;

	bool mishadolddata;


};
int getmessagerownumber(const char* filename);
#ifdef WIN32
unsigned int  __stdcall delandroidchatServer(void* para);
#else
void* delandroidchatServer(void* para);
#endif