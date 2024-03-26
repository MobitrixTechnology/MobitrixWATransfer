#pragma once
//IOS->Android

#include "publicfun.h"

#include "wasqliteparsing.h"
class waios2android
{
public:
	int start_work(const char* backpath, const char* datapath, int wt);
	int write_work();

	int init_msgstore_db();
	//READ
	int readdatabase();
	int readsessiondata();
	int readgroupinfodata();

	int  getmessagetotalnumber();
	int  readmessagesdataA(int _id,int start, int limit);
	std::string getgroupgjid(int z_pk);
	bool getkeyremotejid(int id, std::string& jid, int& gtype);
	bool getchatidfromjid(std::string jid, int& cid);
	//bool getmediainfo(sqliteparsing* osqliteparser ,int id);
	bool getmediainfo(wasqliteparsing* osqliteparser, int id, std::string& media_url, std::string& media_mime_type, std::string& media_size, std::string& media_name, std::string& media_hash, int& media_duration, int& origin, std::string& latitude, std::string& longitude, std::string& media_caption, std::string& thumbpath);

	std::string getmemberjid(int id);
	bool setsessionlastid(int id, int mid);
	//WRITE
	int writedatabase(const char* dbfile);
	void insertjjidmap(int id, std::string jiid);
	int getchatjid(wasqliteparsing* osqliteparser, const char* jid, int type, bool& isadd);
	int getchatjiEx(wasqliteparsing* osqliteparser, const char* jid, int type, bool& isadd);
	int getchat_id(const char* dbfile);
	int getchatidfrommap(std::string jid);
	int getjididfrommap(std::string jid);
	bool checkchat_isexit(wasqliteparsing* osqliteparser, const char* dbfile, int jid_id);
	int update_chat_jid_table(const char* dbfile);
	int write_chat_list_table(const char* dbfile);
	int write_group_participants_table(const char* dbfile);
	int write_messages_table(const char* dbfile);
	bool ismessagstableexist(const char* dbfile);
	int write_message_table(const char* dbfile);
	int write_messages_media_table(const char* dbfile);
	int readselfjid();
	void getsort(std::string jid, int& sort);
	int copy_medaifile();

	int Writetrandat( const char* scrfile, const char* dstfile);


	int getwhserialize(attserializeinfo oattserialize, std::string chatid,std::string& data);

	int sendpress(int step, int tol, int cur);
private:


	//group.net.whatsapp.WhatsApp.shared
	std::string miosdatapath;
	std::string mioschatdb;
	std::string mtoandroidpath;//

	std::string mWallPaperpath;
	std::string mWhatsAppAnimatedGifspath;
	std::string mWhatsAppAudiopath;
	std::string mWhatsAppDocumentspath;
	std::string mWhatsAppImagespath;
	std::string mWhatsAppProfilePhotospath;
	std::string mWhatsAppStickerspath;
	std::string mWhatsAppVoiceNotespath;
	std::string mWhatsAppVideospath;




	std::string mnewdb;
	std::string mWhatsAppMediapath;

	std::map<int, sessioninfo> msessionmap;//
	std::map<int ,groupmemberinfo>  mgroupmemberinfomap;
	//Media
	std::map<std::string, int> mchat_jidarr;
	std::map<int, messengesmediainfo>  mchatmediasarr;
	std::map<int,messengesinfo>  mchatmessagesarr;

	std::map<std::string, int> m_jidarr;//
	std::map<std::string, int> msorftmap;

	std::string mxmlfile;
	std::string  mlimxmlfile;
	std::string mjid;//ID
	std::string mselfjid;
	std::string mphonestr;
	std::string mdstmsgdbfile;

	int m_wt;


	FILE* mpfile;
};

