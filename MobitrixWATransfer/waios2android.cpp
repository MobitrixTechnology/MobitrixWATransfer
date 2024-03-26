#include "waios2android.h"
#include "BaseLog.h"
#include "publicfun.h"
#include "javaSerialize.h"
#include <io.h>
#include "analyiosapp.h"



int waios2android::start_work( const char* backpath,const char* datapath,int wt)
{

	analyiosapp oanalyiosapp;
	int tol=0;
	oanalyiosapp.analyzerwaapp(backpath, datapath, wt, tol);

	m_wt = wt;
	miosdatapath = datapath;
	if(wt==0)
	{
		miosdatapath += "group.net.whatsapp.WhatsApp.shared\\ChatStorage.sqlite";
	}
	else {
		miosdatapath += "group.net.whatsapp.WhatsAppSMB.shared\\ChatStorage.sqlite";
	}

	if (!IsPathExist(miosdatapath.c_str()))
	{

		return -1;
	}


	mtoandroidpath = datapath;
	if (wt == 0)
	{
		mtoandroidpath += "toandroid\\";
	}
	else {
		mtoandroidpath += "bu_toandroid\\";
	}

	mxmlfile = mtoandroidpath;
	mlimxmlfile = mtoandroidpath;
	if (wt == 0)
	{
		mxmlfile += "shared_prefs\\com.whatsapp.registration.RegisterPhone.xml";

		mlimxmlfile += "shared_prefs\\com.hwhatsapp_preferences_light.xml";
	}
	else {
	
		mxmlfile += "shared_prefs\\com.whatsapp.registration.RegisterPhone.xml";
		mlimxmlfile += "shared_prefs\\com.whatsapp.hw4b_preferences_light.xml";
	}
	readselfjid();

	CreatePath(mtoandroidpath.c_str());

	//Media File
	if (wt == 0)
	{
		mWallPaperpath = mtoandroidpath;
		mWallPaperpath += "WhatsApp\\Media\\WallPaper\\";
		mWhatsAppAnimatedGifspath = mtoandroidpath;
		mWhatsAppAnimatedGifspath += "WhatsApp\\Media\\WhatsApp Animated Gifs\\";
		mWhatsAppAudiopath = mtoandroidpath;
		mWhatsAppAudiopath += "WhatsApp\\Media\\WhatsApp Audio\\";
		mWhatsAppDocumentspath = mtoandroidpath;
		mWhatsAppDocumentspath += "WhatsApp\\Media\\WhatsApp Documents\\";
		mWhatsAppImagespath = mtoandroidpath;
		mWhatsAppImagespath += "WhatsApp\\Media\\WhatsApp Images\\";
		mWhatsAppProfilePhotospath = mtoandroidpath;
		mWhatsAppProfilePhotospath += "WhatsApp\\Media\\WhatsApp Profile Photos\\";
		mWhatsAppStickerspath = mtoandroidpath;
		mWhatsAppStickerspath += "WhatsApp\\Media\\WhatsApp Stickers\\";
		mWhatsAppVoiceNotespath = mtoandroidpath;
		mWhatsAppVoiceNotespath += "WhatsApp\\Media\\WhatsApp Voice Notes\\";
		mWhatsAppVideospath = mtoandroidpath;
		mWhatsAppVideospath += "WhatsApp\\Media\\WhatsApp Video\\";
	}
	else {
		mWallPaperpath = mtoandroidpath;
		mWallPaperpath += "WhatsApp Business\\Media\\WallPaper\\";
		mWhatsAppAnimatedGifspath = mtoandroidpath;
		mWhatsAppAnimatedGifspath += "WhatsApp Business\\Media\\WhatsApp Business Animated Gifs\\";
		mWhatsAppAudiopath = mtoandroidpath;
		mWhatsAppAudiopath += "WhatsApp Business\\Media\\WhatsApp Business Audio\\";
		mWhatsAppDocumentspath = mtoandroidpath;
		mWhatsAppDocumentspath += "WhatsApp Business\\Media\\WhatsApp Business Documents\\";
		mWhatsAppImagespath = mtoandroidpath;
		mWhatsAppImagespath += "WhatsApp Business\\Media\\WhatsApp Business Images\\";
		mWhatsAppProfilePhotospath = mtoandroidpath;
		mWhatsAppProfilePhotospath += "WhatsApp Business\\Media\\WhatsApp Business Profile Photos\\";
		mWhatsAppStickerspath = mtoandroidpath;
		mWhatsAppStickerspath += "WhatsApp Business\\Media\\WhatsApp Business Stickers\\";
		mWhatsAppVoiceNotespath = mtoandroidpath;
		mWhatsAppVoiceNotespath += "WhatsApp Business\\Media\\WhatsApp Business Voice Notes\\";
		mWhatsAppVideospath = mtoandroidpath;
		mWhatsAppVideospath += "WhatsApp Business\\Media\\WhatsApp Business Video\\";
	
	}

	CreatePath(mWallPaperpath.c_str());
	
	CreatePath(mWhatsAppAnimatedGifspath.c_str());
	CreatePath(mWhatsAppAudiopath.c_str());
	CreatePath(mWhatsAppDocumentspath.c_str());
	CreatePath(mWhatsAppImagespath.c_str());
	CreatePath(mWhatsAppProfilePhotospath.c_str());
	CreatePath(mWhatsAppStickerspath.c_str());
	CreatePath(mWhatsAppVoiceNotespath.c_str());
	CreatePath(mWhatsAppVideospath.c_str());
	
	mnewdb = mtoandroidpath;
	mnewdb += "newdb\\";

	RemoveAll(mnewdb.c_str());
	CreatePath(mnewdb.c_str());

	mWhatsAppMediapath = datapath;
	if (wt == 0)
	{
		mWhatsAppMediapath += "group.net.whatsapp.WhatsApp.shared\\Message\\";
	}
	else {
		mWhatsAppMediapath += "group.net.whatsapp.WhatsAppSMB.shared\\Message\\";
	}
	readdatabase();
	mdstmsgdbfile = "";
	std::string msgdbfile = mtoandroidpath;
	msgdbfile += "databases\\msgstore.db";

	std::string msgdbfile_shm = mtoandroidpath;
	msgdbfile_shm += "databases\\msgstore.db-shm";

	std::string msgdbfile_wal = mtoandroidpath;
	msgdbfile_wal += "databases\\msgstore.db-wal";


	std::string dstmsgdbfile = mnewdb;
	dstmsgdbfile += "msgstore.db";

	std::string dstmsgdbfile_shm = mnewdb;
	dstmsgdbfile_shm += "msgstore.db-shm";

	std::string dstmsgdbfile_wal = mnewdb;
	dstmsgdbfile_wal += "msgstore.db-wal";

	if (!IsPathExist(msgdbfile.c_str()))
	{
		if (mpfile)
			fclose(mpfile);

		return -1;
	}
	remove(dstmsgdbfile.c_str());
	remove(dstmsgdbfile_shm.c_str());
	remove(dstmsgdbfile_wal.c_str());
	copyfile(msgdbfile.c_str(), dstmsgdbfile.c_str());
	copyfile(msgdbfile_shm.c_str(), dstmsgdbfile_shm.c_str());
	copyfile(msgdbfile_wal.c_str(), dstmsgdbfile_wal.c_str());
	if (!IsPathExist(dstmsgdbfile.c_str()))
	{
		if (mpfile)
			fclose(mpfile);
		return -1;
	}
	mdstmsgdbfile = dstmsgdbfile;
	return 0;
}
int waios2android::write_work()
{


	if (mdstmsgdbfile.length() == 0)
	{
		if(mpfile)
			fclose(mpfile);
		return 0;
	}

	writedatabase(mdstmsgdbfile.c_str());

	return 0;

}


int waios2android::readselfjid()
{
	mselfjid = "";
	mphonestr = "";
	if (!IsPathExist(mlimxmlfile.c_str()))
	{
		if (!IsPathExist(mxmlfile.c_str()))
		{
			return -1;
		}
	}
	int isregphone = -1;
	unsigned int len;
	char* xmlbuf = CReadFromFile(mxmlfile.c_str(), len);
	if (xmlbuf)
	{
		std::string countrycode = GetMidData(xmlbuf, "<string name=\"com.whatsapp.registration.RegisterPhone.country_code\">", "</string>");
		std::string phonenmuber = GetMidData(xmlbuf, "<string name=\"com.whatsapp.registration.RegisterPhone.phone_number\">", "</string>");

		if (phonenmuber.length() != 0)
		{
			mphonestr = countrycode + phonenmuber;
			mselfjid = countrycode + phonenmuber;

			mselfjid += "@s.whatsapp.net";
			isregphone = 0;
		}
		delete[] xmlbuf;
	}
	if (len == 0|| mselfjid.length()==0)
	{
		xmlbuf = CReadFromFile(mlimxmlfile.c_str(), len);
		if (xmlbuf)
		{
			std::string registration_jid = GetMidData(xmlbuf, "<string name=\"registration_jid\">", "</string>");

			if (registration_jid.length() != 0)
			{

				mselfjid = registration_jid;

				mselfjid += "@s.whatsapp.net";
				isregphone = 0;
			}
			delete[] xmlbuf;
		}
	}
	return isregphone;
}
int waios2android::init_msgstore_db()
{
	return 0;
}
int waios2android::readdatabase()
{

	 readsessiondata();
	 readgroupinfodata();
	 int start = 0;
	 int t=getmessagetotalnumber();
	 readmessagesdataA(2, 0,10000);

	return 0;
}
int waios2android::readsessiondata()
{
	wasqliteparsing osqliteparser(miosdatapath.c_str());
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "select Z_PK,ZCONTACTJID,ZARCHIVED,ZLASTMESSAGEDATE,ZPARTNERNAME,ZGROUPINFO,ZSESSIONTYPE from ZWACHATSESSION");
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser.SqlQuery(slqstr, vmapstr))
	{
		for (int n = 0; n < vmapstr.size(); n++)
		{
			sessioninfo osessioninfo;
			std::string Z_PK = FindDataFromMap(vmapstr.at(n), "Z_PK");
			std::string ZCONTACTJID = FindDataFromMap(vmapstr.at(n), "ZCONTACTJID");
			std::string ZLASTMESSAGEDATE = FindDataFromMap(vmapstr.at(n), "ZLASTMESSAGEDATE");
			std::string ZGROUPINFO = FindDataFromMap(vmapstr.at(n), "ZGROUPINFO");
			std::string ZPARTNERNAME = FindDataFromMap(vmapstr.at(n), "ZPARTNERNAME");
			std::string ZSESSIONTYPE = FindDataFromMap(vmapstr.at(n), "ZSESSIONTYPE");
			osessioninfo.id = atoi(Z_PK.c_str());
			osessioninfo.key_remote_jid = ZCONTACTJID;//GROUP
			//if (!ZGROUPINFO.empty())
			if (ZCONTACTJID == "status@broadcast")
			{
				osessioninfo.gtype = 5;
				osessioninfo.subject = ZPARTNERNAME;
			}
			else if(strstr(ZCONTACTJID.c_str(),".0:0@s.whatsapp.net"))
			{
				osessioninfo.gtype = 17;
				osessioninfo.subject = ZPARTNERNAME;
			}
			else if (strstr(ZCONTACTJID.c_str(), "@s.whatsapp.net"))
			{
				osessioninfo.gtype = 0;
				osessioninfo.subject = ZPARTNERNAME;
			}
			else if (strstr(ZCONTACTJID.c_str(), "@temp"))
			{
				osessioninfo.gtype = 2;
				osessioninfo.subject = ZPARTNERNAME;
			}
			else
			{
				osessioninfo.gtype = 1;
				osessioninfo.subject = ZPARTNERNAME;
			}
			std::string ZARCHIVED = FindDataFromMap(vmapstr.at(n), "ZARCHIVED");
			if (ZARCHIVED.length() != 0)
			{
				osessioninfo.archived = atoi(ZARCHIVED.c_str());
			}
			else {
				osessioninfo.archived = 0;
			}
			osessioninfo.creation = formatiostm_wm(ZLASTMESSAGEDATE);
			osessioninfo.last_message_table_id = 0;
			osessioninfo.message_table_id = 0;
			msessionmap.insert(std::make_pair(osessioninfo.id, osessioninfo));
		}
	}
	return 0;
}
//

std::string waios2android::getgroupgjid(int z_pk)
{
	std::map<int, sessioninfo>::iterator itt;
	itt = msessionmap.find(z_pk);
	if (itt != msessionmap.end())
	{
		return itt->second.key_remote_jid;
	}
	return "";

}

//
int waios2android::readgroupinfodata()
{
	wasqliteparsing* osqliteparser = new wasqliteparsing(miosdatapath.c_str());
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "select Z_PK,ZISADMIN,ZCHATSESSION,ZMEMBERJID,ZSENDERKEYSENT,ZCONTACTABID from ZWAGROUPMEMBER");
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser->SqlQuery(slqstr, vmapstr))
	{
		for (int n = 0; n < vmapstr.size(); n++)
		{
			groupmemberinfo ogroupmemberinfo;
			std::string ZISADMIN = FindDataFromMap(vmapstr.at(n), "ZISADMIN");
			std::string ZCHATSESSION = FindDataFromMap(vmapstr.at(n), "ZCHATSESSION");
			std::string ZMEMBERJID = FindDataFromMap(vmapstr.at(n), "ZMEMBERJID");
			std::string ZSENDERKEYSENT = FindDataFromMap(vmapstr.at(n), "ZSENDERKEYSENT");
			std::string ZCONTACTABID = FindDataFromMap(vmapstr.at(n), "ZCONTACTABID");
			std::string Z_PK = FindDataFromMap(vmapstr.at(n), "Z_PK");
			if (Z_PK == "" || Z_PK.length() == 0)
			{
				continue;
			}
			if (ZISADMIN.empty()|| ZISADMIN.length()==0)
			{
				ogroupmemberinfo.admin = 0;
			}
			else 
			{
				int tmpadmin= atoi(ZISADMIN.c_str());
				if (tmpadmin == 1)
				{
					ogroupmemberinfo.admin = 2;
				}
				else
				{
					ogroupmemberinfo.admin = 0;
				}
			
			}
			if (ZCHATSESSION=="" || ZCHATSESSION.length() == 0)
			{
				continue;
			}

			std::string gjid= getgroupgjid(atoi(ZCHATSESSION.c_str()));
			if (gjid.empty()|| gjid=="")
			{
				continue;
			}
			ogroupmemberinfo.gjid = gjid;
			ogroupmemberinfo.jid = ZMEMBERJID;
			
			if (ZSENDERKEYSENT.empty())
			{
				ogroupmemberinfo.sent_sender_key = "";
			}
			else
			{
				ogroupmemberinfo.sent_sender_key = ZSENDERKEYSENT;
			}
			ogroupmemberinfo.id=atoi(Z_PK.c_str());

			ogroupmemberinfo.pending = 0;

			mgroupmemberinfomap.insert(std::make_pair(ogroupmemberinfo.id  , ogroupmemberinfo));
		}
	}
	delete osqliteparser;
	return 0;
}
std::string waios2android::getmemberjid(int id)
{
	std::map<int, groupmemberinfo>::iterator itt;
	itt = mgroupmemberinfomap.find(id);
	if (itt != mgroupmemberinfomap.end())
	{
		return itt->second.jid;
	}
	return "";
}

bool waios2android::getchatidfromjid(std::string jid, int& cid)
{
	bool isret = false;
	std::map<int, sessioninfo>::iterator itt;
	for (itt = msessionmap.begin(); itt != msessionmap.end(); itt++)
	{
		if (jid == itt->second.key_remote_jid)
		{
			cid=itt->first;
			isret = true;
			break;
		}
	}
	return isret;
}
bool waios2android::getkeyremotejid(int id, std::string& jid, int& gtype)
{
	std::map<int, sessioninfo>::iterator itt;
	itt = msessionmap.find(id);
	if (itt != msessionmap.end())
	{
		jid= itt->second.key_remote_jid;

		gtype = itt->second.gtype;
		return true;
	}
	return false;
}
//lase_id
bool waios2android::setsessionlastid(int id,  int mid)
{
	std::map<int, sessioninfo>::iterator itt;
	itt = msessionmap.find(id);
	if (itt != msessionmap.end())
	{
		itt->second.last_message_table_id = mid;

		itt->second.message_table_id = mid;
		return true;
	}
	return false;

}

bool waios2android::getmediainfo(wasqliteparsing* osqliteparser,int id,std::string & media_url,std::string &media_mime_type,std::string & media_size,std::string& media_name,std::string &media_hash,int & media_duration,int & origin,std::string &latitude,std::string & longitude,std::string & media_caption,std::string &thumbpath)
{//
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "select ZFILESIZE,ZMOVIEDURATION,ZMEDIAORIGIN,ZMESSAGE,ZLATITUDE,ZLONGITUDE,ZMEDIAURLDATE,ZAUTHORNAME,ZMEDIALOCALPATH,ZMEDIAURL,ZTITLE,ZVCARDNAME,ZVCARDSTRING,ZXMPPTHUMBPATH from ZWAMEDIAITEM where Z_PK==\'%d\'",id);
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser->SqlQuery(slqstr, vmapstr))
	{
		if (vmapstr.size() != 1)
		{
			return false;
		}

		media_name = FindDataFromMap(vmapstr.at(0), "ZMEDIALOCALPATH");
		media_mime_type= FindDataFromMap(vmapstr.at(0), "ZVCARDSTRING");
		media_size= FindDataFromMap(vmapstr.at(0), "ZFILESIZE");
		media_url = FindDataFromMap(vmapstr.at(0), "ZMEDIAURL");
		media_hash = FindDataFromMap(vmapstr.at(0), "ZVCARDNAME");
		thumbpath = FindDataFromMap(vmapstr.at(0), "ZXMPPTHUMBPATH");	
		std::string  media_durationstr = FindDataFromMap(vmapstr.at(0), "ZMOVIEDURATION");
		if (media_durationstr.empty())
		{
			media_duration = 0;
		}
		else
		{
			media_duration = atoi(media_durationstr.c_str());
		}

		std::string originstr=  FindDataFromMap(vmapstr.at(0), "ZMEDIAORIGIN");
		if (originstr.empty())
		{
			origin = 0;
		}
		else
		{
			origin = atoi(originstr.c_str());
		}

		latitude= FindDataFromMap(vmapstr.at(0), "ZLATITUDE");
		longitude = FindDataFromMap(vmapstr.at(0), "ZLONGITUDE");

		media_caption = FindDataFromMap(vmapstr.at(0), "ZAUTHORNAME");
		if (media_caption.empty())
		{
			media_caption = FindDataFromMap(vmapstr.at(0), "ZTITLE");
		}
	}
	return true;
}


int  waios2android::getmessagetotalnumber()
{

	std::string count_str;
	wasqliteparsing* osqliteparser = new wasqliteparsing(miosdatapath.c_str());

	int ret = osqliteparser->SqlQuery("select count(*) from ZWAMESSAGE", count_str, "\1", '\2');
	if (ret == -1)
	{
		delete osqliteparser;
		return -1;
	}
	Replace(count_str, "\1", "");
	Replace(count_str, "\2", "");
	return atoi(count_str.c_str());

}

int  waios2android::readmessagesdataA(int _id,int start,int limit)
{
	bool isinxunhuan = false;
	wasqliteparsing* osqliteparser = new wasqliteparsing(miosdatapath.c_str());
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "select Z_PK,ZISFROMME,ZMESSAGESTATUS,ZGROUPMEMBER,ZMESSAGETYPE,ZCHATSESSION,ZMEDIAITEM,ZMESSAGEDATE,ZSENTDATE,ZFROMJID,ZSTANZAID,ZTEXT,ZTOJID,ZSTARRED from ZWAMESSAGE limit %d,%d", start, limit);
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser->SqlQuery(slqstr, vmapstr))
	{
		if (vmapstr.size() == limit)
		{
			isinxunhuan = true;
			start += limit;
		}
	//	int _id = 2;
		AWriteLog("waios2android", "readmessagesdata Size:%d", vmapstr.size());
		for (int n = 0; n < vmapstr.size(); n++)
		{

			std::string ZMESSAGETYPE = FindDataFromMap(vmapstr.at(n), "ZMESSAGETYPE");
			if (ZMESSAGETYPE.empty())
			{
				continue;
			}
			std::string ZMESSAGESTATUS = FindDataFromMap(vmapstr.at(n), "ZMESSAGESTATUS");


			std::string ZISFROMME = FindDataFromMap(vmapstr.at(n), "ZISFROMME");


			int keyfromme = atoi(ZISFROMME.c_str());
			int status = 13;
			int mediawatype = 0;

			if (ZMESSAGESTATUS == "3")
			{
				status = 0;
			}

			int ztype = atoi(ZMESSAGETYPE.c_str());
			if (ztype == 0)
			{//

				mediawatype = 0;
			}
			else if (ztype == 1)
			{//
				mediawatype = 1;
			}
			else if (ztype == 2|| ztype == 11)
			{//
				mediawatype = 3;
			}
			else if (ztype == 3)
			{//
				mediawatype = 2;
			}
			else if (ztype == 4)
			{//
				mediawatype = 4;
			}
			else if (ztype == 5)
			{//
				mediawatype = 5;
			}
			else if (ztype == 6)
			{//
				continue;

			}
			else if (ztype == 7)
			{//
				//continue;
				mediawatype = 97;
			}

			else if (ztype == 8)
			{//
				mediawatype = 9;
			}

			else if (ztype == 10)
			{//
				continue;
			}

			else if (ztype == 15)
			{// -> 20

				mediawatype = 20;
				
				continue;
			}
			else {
				continue;
			}

			std::string ZFROMJID = FindDataFromMap(vmapstr.at(n), "ZFROMJID");
			std::string ZTOJID = FindDataFromMap(vmapstr.at(n), "ZTOJID");
			std::string ZCHATSESSION = FindDataFromMap(vmapstr.at(n), "ZCHATSESSION");
			if (ZCHATSESSION.empty())
			{
				continue;
			}
			std::string ssmetext = FindDataFromMap(vmapstr.at(n), "ZTEXT");
			if (strstr(ssmetext.c_str(), "Proficiat super mooi nieuws!! Geniet ervan met zijn "))
			{
			
			}
	
			int sessionid = atoi(ZCHATSESSION.c_str());
			std::string jid;
			int gtype = 0;
			if (!getkeyremotejid(sessionid, jid, gtype))
			{

				continue;

			}

			std::string ZSTARRED = FindDataFromMap(vmapstr.at(n), "ZSTARRED");
			std::string ZSTANZAID = FindDataFromMap(vmapstr.at(n), "ZSTANZAID");
			std::string ZMEDIAITEM = FindDataFromMap(vmapstr.at(n), "ZMEDIAITEM");
			std::string ZMESSAGEDATE = FindDataFromMap(vmapstr.at(n), "ZMESSAGEDATE");
			std::string ZSENTDATE = FindDataFromMap(vmapstr.at(n), "ZMESSAGEDATE");
			int64_t timestamp = formatiostm_wm(ZMESSAGEDATE);
			int64_t send_timestamp = formatiostm_wm(ZSENTDATE);
			std::string sender_jid="";
			std::string remoteresource = "";
			if (gtype == 1 && keyfromme == 0)
			{

				std::string ZGROUPMEMBER = FindDataFromMap(vmapstr.at(n), "ZGROUPMEMBER");
				if (ZGROUPMEMBER.empty())
				{
					//
					continue;
				}
				else
				{
					int zgroupmemberid = atoi(ZGROUPMEMBER.c_str());

					remoteresource = getmemberjid(zgroupmemberid);
					if (remoteresource.empty())
					{
						continue;

					}
					sender_jid = remoteresource;
				}

			}
			else if (keyfromme == 1)
			{
				sender_jid = mselfjid;

			}
			else {
			//
				sender_jid = jid;

			}
			std::string metext = FindDataFromMap(vmapstr.at(n), "ZTEXT");
			std::string media_url;
			std::string media_mime_type;
			std::string media_size = "0";
			std::string media_name;
			std::string media_hash;
			int media_duration = 0;
			int origin = 0;
			std::string latitude = "0.0";
			std::string longitude = "0.0";
			std::string media_caption;
			std::string thumbpath;
			bool ishadmedia = false;
			if (!ZMEDIAITEM.empty())
			{
				int zid = atoi(ZMEDIAITEM.c_str());
				if (getmediainfo(osqliteparser, zid, media_url, media_mime_type, media_size, media_name, media_hash, media_duration, origin, latitude, longitude, media_caption, thumbpath))
				{
					ishadmedia = true;
				}

			}

			bool isdown = false;


			std::string attlocalfile = "";
			std::string thlocalfile = "";
			std::string remoteattfile = "";
			std::string mediadatafile = "";
			std::string remotethfile = "";
			if (mediawatype == 4)
			{//

				metext = std::string(media_mime_type);
				media_mime_type = "";

				media_name = std::string(media_hash);
				media_hash = "";
			}
			else if (mediawatype == 97)
			{
				mediawatype = 0;
			}
			else if (mediawatype == 1 || mediawatype == 2 || mediawatype == 3 || mediawatype == 9 || mediawatype == 20)
			{//
				if (!media_name.empty())
				{
					isdown = true;

					attlocalfile = mWhatsAppMediapath + media_name;
#ifdef WIN32
					Replace(attlocalfile, "/", "\\");
#endif
				}
				if (!thumbpath.empty())
				{
					thlocalfile = mWhatsAppMediapath + thumbpath;
#ifdef WIN32
					Replace(thlocalfile, "/", "\\");
#endif
				}
				if (mediawatype == 1)
				{//
					if (!media_name.empty())
					{
						remoteattfile = mWhatsAppImagespath + media_name;//	WhatsApp Images
						if (m_wt == 0)
						{
							mediadatafile = "Media/WhatsApp Images/" + media_name;
						}
						else {
							
							mediadatafile = "Media/WhatsApp Business Images/" + media_name;
						}
					}
					if (!thumbpath.empty())
					{

						remotethfile = mWhatsAppImagespath + thumbpath;//	WhatsApp Images
					}


				}
				else if (mediawatype == 20)
				{
					//
					if (!media_name.empty())
					{
						media_name = GetFileName_Unxi(media_name.c_str());
						remoteattfile = mWhatsAppStickerspath + media_name;//	WhatsApp Images
						if (m_wt == 0)
						{
							mediadatafile = "Media/WhatsApp Stickers/" + media_name;
						}
						else {
							mediadatafile = "Media/WhatsApp Business Stickers/" + media_name;
						}
					}
					//	media_name = "";
				}

				else if (mediawatype == 2)
				{//
					if (!media_name.empty())
					{
						remoteattfile = mWhatsAppVoiceNotespath + media_name;//	WhatsApp Images
						if (m_wt == 0)
						{
							mediadatafile = "Media/WhatsApp Voice Notes/" + media_name;
						}
						else {
							mediadatafile = "Media/WhatsApp Business Voice Notes/" + media_name;
						}
					}
					if (!thumbpath.empty())
					{
						remotethfile = mWhatsAppVoiceNotespath + thumbpath;//	WhatsApp Images
					}

				}
				else if (mediawatype == 3)
				{//

					if (!media_name.empty())
					{
						remoteattfile = mWhatsAppVideospath + media_name;//	WhatsApp Images
						if (m_wt == 0)
						{
							mediadatafile = "Media/WhatsApp Video/" + media_name;
						}
						else {
							mediadatafile = "Media/WhatsApp Business Video/" + media_name;
						}
					}
					if (!thumbpath.empty())
					{
						remotethfile = mWhatsAppVideospath + thumbpath;//	WhatsApp Images
					}

				}
				else if (mediawatype == 9)
				{//

					if (!media_name.empty())
					{
						media_name = GetFileName_Unxi(media_name.c_str());
						//media_caption
						media_name = media_caption;
						remoteattfile = mWhatsAppDocumentspath + media_caption;//	WhatsApp Images
						if (m_wt == 0)
						{
							mediadatafile = "Media/WhatsApp Documents/" + media_caption;
						}
						else {
						
							mediadatafile = "Media/WhatsApp Business Documents/" + media_caption;
						}
					}
					if (!thumbpath.empty())
					{

						remotethfile = mWhatsAppDocumentspath + thumbpath;//	WhatsApp Images
					}

				}
			}
			if (mediadatafile.length() != 0)
			{
				messengesmediainfo omessengesmediainfo;
				omessengesmediainfo.message_row_id = _id;
				omessengesmediainfo.media_job_uuid = "";
				omessengesmediainfo.file_path = mediadatafile;
				omessengesmediainfo.file_size = media_size;
				omessengesmediainfo.file_length = media_size;

				int findxb = latitude.find(".");
				int findyb = longitude.find(".");

				omessengesmediainfo.width = longitude.substr(0, findyb);;
				omessengesmediainfo.height = latitude.substr(0, findxb);


				omessengesmediainfo.media_name = media_name;
				omessengesmediainfo.mime_type = media_mime_type;
				omessengesmediainfo.media_duration = media_duration;
				//	omessengesmediainfo.media_duration = 2;
				omessengesmediainfo.rejid = jid;
				mchatmediasarr.insert(std::make_pair(_id, omessengesmediainfo));
			}

			messengesinfo omessengesinfo;
			omessengesinfo.data = metext;
			omessengesinfo._id = _id;
			omessengesinfo.starred = 0;
			if (ZSTARRED == "1")
			{
				omessengesinfo.starred = 1;
			}
			omessengesinfo.key_remote_jid = jid;//
			omessengesinfo.key_from_me = keyfromme;//ZISFROMME
			omessengesinfo.key_id = ZSTANZAID;//ZSTANZAID
			omessengesinfo.status = status;//ZMESSAGESTATUS 
			omessengesinfo.need_push = 0;//0

			omessengesinfo.timestamp = timestamp;//ZMESSAGEDATE
			omessengesinfo.media_url = media_url;//ZMEDIAURL
			omessengesinfo.media_mime_type = media_mime_type;//ZVCARDSTRING
			omessengesinfo.media_wa_type = mediawatype;//ZMESSAGETYPR
			omessengesinfo.media_size = media_size;//ZFILESIZE
			omessengesinfo.media_name = media_name;//ZMEDIALOCALPATH
			omessengesinfo.media_hash = media_hash;//ZVCARDNAME
			omessengesinfo.media_duration = media_duration;//ZMOVIEDURATION
			omessengesinfo.origin = origin;//ZMEDIAORIGIN
			omessengesinfo.latitude = latitude;//ZLATITUDE
			omessengesinfo.longitude = longitude;//ZLONGGITUDE
			omessengesinfo.thum_image = NULL;//
			omessengesinfo.remote_resource = remoteresource;//
			omessengesinfo.received_timestamp = timestamp;//ZMESSAGEDATE
			omessengesinfo.send_timestamp = send_timestamp;//ZSENTDATE
			omessengesinfo.receipt_server_timestamp = timestamp;//ZMESSAGEDATE
			omessengesinfo.raw_data = NULL;
			omessengesinfo.recipient_count = "";
			omessengesinfo.media_caption = media_caption;//ZAUTHORNAME  ZTITLE
			omessengesinfo.attlocalfile = attlocalfile;
			omessengesinfo.thlocalfile = thlocalfile;
			omessengesinfo.mediadatafile = mediadatafile;//
			omessengesinfo.remoteattfile = remoteattfile;
			omessengesinfo.remotethfile = remotethfile;
			omessengesinfo.ishadmedia = ishadmedia;
			omessengesinfo.sender_jid = sender_jid;
			mchatmessagesarr.insert(std::make_pair(_id, omessengesinfo));
			setsessionlastid(sessionid, _id);
			_id += 1;
		}
	}
	vmapstr.clear();
	delete osqliteparser;
	if(isinxunhuan)
		readmessagesdataA(_id,start,limit);
	return 0;
}

int waios2android::writedatabase(const char* dbfile)
{

	update_chat_jid_table(dbfile);
	
	getchat_id(dbfile);


	 write_group_participants_table(dbfile);
	
	 write_messages_table(dbfile);

	 write_messages_media_table(dbfile);

	 if(mpfile)
		 fclose(mpfile);
	 return 0;
}
//chat _id
int waios2android::getchat_id(const char* dbfile)
{
	wasqliteparsing* osqliteparser = new wasqliteparsing(dbfile);
	std::map<int, sessioninfo>::iterator itt;
	for (itt = msessionmap.begin(); itt != msessionmap.end(); itt++)
	{

		char updatesql[512] = { 0 };
		sprintf(updatesql, "select _id from chat where jid_row_id==%d", itt->second.jid_id);

		std::vector<std::map<std::string, std::string> > vmapstr;
		int mtalnum = 0;
	
		if (osqliteparser->SqlQuery(updatesql, vmapstr))
		{
			if (vmapstr.size() == 1)
			{
				int chatjid_id = atoi(FindDataFromMap(vmapstr.at(0), "_id").c_str());
				std::map<std::string, int>::iterator itty;
				itty = mchat_jidarr.find(itt->second.key_remote_jid);
				if (itty == mchat_jidarr.end())
				{
					mchat_jidarr.insert(make_pair(itt->second.key_remote_jid, chatjid_id));
				}

			}
		}
	}
	delete osqliteparser;
	return 0;
}
int waios2android::getjididfrommap(std::string jid)
{

	std::map<std::string, int>::iterator itt;
	itt = m_jidarr.find(jid);
	if (itt != m_jidarr.end())
	{
		return itt->second;
	}
	return 0;

}
int waios2android::getchatidfrommap(std::string jid)
{
	std::map<std::string, int>::iterator itty;
	itty = mchat_jidarr.find(jid);
	if (itty != mchat_jidarr.end())
	{
		return itty->second;
	}
	return -1;
}
void waios2android::insertjjidmap(int id,std::string jiid)
{
	std::map<std::string, int>::iterator itt;
	itt = m_jidarr.find(jiid);
	if (itt == m_jidarr.end())
	{
		m_jidarr.insert(make_pair(jiid,id));
	}
	return ;
}

int waios2android::getchatjid(wasqliteparsing* osqliteparser , const char* jid,int type,bool &isadd)
{

	int ret_id = -1;

	char selstr[256] = {0};
	sprintf(selstr,"select _id from jid where raw_string == \'%s\'",jid);
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser->SqlQuery(selstr, vmapstr))
	{
		if (vmapstr.size() == 1)
		{
			std::string _idstd = FindDataFromMap(vmapstr.at(0), "_id"); 
			ret_id = atoi(_idstd.c_str());

		}
		else
		{
			std::vector<std::string> sVector;
			std::string tmpstr = std::string(jid);
			SplitStoV((char*)tmpstr.c_str(), tmpstr.length(),"@", sVector);
			if (sVector.size() == 2)
			{

				std::string user = sVector.at(0);

				std::string userverser = sVector.at(1);
				char injid[512] = { 0 };

				sprintf(injid, "INSERT INTO jid (user, server, agent, device, type, raw_string) VALUES (\'%s\', \'%s\', 0, 0, %d, \'%s\')", user.c_str(), userverser.c_str(), type, jid);
				osqliteparser->SqlExe(injid);


				char selstr_second[256] = { 0 };
				sprintf(selstr_second, "select _id from jid where raw_string == \'%s\'", jid);
				std::vector<std::map<std::string, std::string> > vmapstr_second;
				if (osqliteparser->SqlQuery(selstr_second, vmapstr_second))
				{
					if (vmapstr_second.size() == 1)
					{
						std::string _idstd = FindDataFromMap(vmapstr_second.at(0), "_id");
						ret_id = atoi(_idstd.c_str());
						isadd = true;
					}
				}
			}
		}
	}

	insertjjidmap(ret_id, jid);
	return ret_id;

}

int waios2android::getchatjiEx(wasqliteparsing* osqliteparser, const char* jid, int type, bool& isadd)
{

	int ret_id = -1;

	char selstr[256] = { 0 };
	sprintf(selstr, "select _id from jid where raw_string == \'%s\'", jid);
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser->SqlQuery(selstr, vmapstr))
	{
		if (vmapstr.size() == 1)
		{
			//
			std::string _idstd = FindDataFromMap(vmapstr.at(0), "_id");
			ret_id = atoi(_idstd.c_str());

		}
		else
		{//
			std::vector<std::string> sVector;
			std::string tmpstr = std::string(jid);
			SplitStoV((char*)tmpstr.c_str(), tmpstr.length(), "@", sVector);
			if (sVector.size() == 2)
			{

				std::string user = sVector.at(0);

				std::string userverser = sVector.at(1);
				char injid[512] = { 0 };

				sprintf(injid, "INSERT INTO jid (user, server, agent, device, type, raw_string) VALUES (\'%s\', \'%s\', 0, 0, %d, \'%s\')", user.c_str(), userverser.c_str(), type, jid);
				osqliteparser->SqlExe(injid);


				char selstr_second[256] = { 0 };
				sprintf(selstr_second, "select _id from jid where raw_string == \'%s\'", jid);
				std::vector<std::map<std::string, std::string> > vmapstr_second;
				if (osqliteparser->SqlQuery(selstr_second, vmapstr_second))
				{
					if (vmapstr_second.size() == 1)
					{
						std::string _idstd = FindDataFromMap(vmapstr_second.at(0), "_id");
						ret_id = atoi(_idstd.c_str());
						isadd = true;
					}
				}
			}

			insertjjidmap(ret_id, jid);

		}
	}


	return ret_id;

}


bool waios2android::checkchat_isexit(wasqliteparsing* osqliteparser,const char* dbfile,int jid_id)
{
	char updatesql[512] = { 0 };
	sprintf(updatesql, "select count(_id) from chat where jid_row_id==%d", jid_id);
	std::vector<std::map<std::string, std::string> > vmapstr;
	int mtalnum = 0;

	if (osqliteparser->SqlQuery(updatesql, vmapstr))
	{
		if (vmapstr.size() == 1)
			mtalnum = atoi(FindDataFromMap(vmapstr.at(0), "count(_id)").c_str());
	}

	if (mtalnum != 0)
	{
		return false;
	}
	return true;
}
int waios2android::update_chat_jid_table(const char* dbfile)
{
	wasqliteparsing* osqliteparser = new wasqliteparsing(dbfile);
	//
	std::map<int, sessioninfo>::iterator itt;
	for (itt = msessionmap.begin(); itt != msessionmap.end(); itt++)
	{


		if (itt->second.key_remote_jid == mselfjid)
		{
			continue;
		}

		Replace(itt->second.subject, "'", "''");
		bool isadd = false;
		itt->second.jid_id = getchatjid(osqliteparser, itt->second.key_remote_jid.c_str(), itt->second.gtype, isadd);
		if(!isadd)
			isadd=checkchat_isexit(osqliteparser,dbfile, itt->second.jid_id);
		if (isadd&& itt->second.jid_id!=-1)
		{

			char insql[4096*4] = {0};
			//
			sprintf(insql, "INSERT INTO chat(jid_row_id, hidden, subject, created_timestamp, display_message_row_id, last_message_row_id, last_read_message_row_id,last_read_receipt_sent_message_row_id,				last_important_message_row_id,				archived, sort_timestamp, mod_tag, gen, spam_detection, unseen_earliest_message_received_time, unseen_message_count, unseen_missed_calls_count, unseen_row_count,				plaintext_disabled, vcard_ui_dismissed, change_number_notified_message_row_id, show_group_description, ephemeral_expiration, last_read_ephemeral_message_row_id,ephemeral_setting_timestamp) VALUES(% d, 0, \'%s\', %lld, %d, %d, 0, 0,	1,%d, % lld, 0, 0.0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0)"

				, itt->second.jid_id, itt->second.subject.c_str(), itt->second.creation, itt->second.last_message_table_id, itt->second.last_message_table_id,
				itt->second.archived, itt->second.creation);
			osqliteparser->SqlExe(insql);

		}
		else if (itt->second.jid_id != -1)
		{
			//
			char updatesql[4096 * 4] = {0};
			sprintf(updatesql, "update chat set hidden=0, display_message_row_id=%d, last_message_row_id=%d, subject=\'%s\',created_timestamp=%lld,sort_timestamp=%lld,last_important_message_row_id=1,archived=0 where jid_row_id==%d", itt->second.last_message_table_id, itt->second.last_message_table_id, itt->second.subject.c_str(), itt->second.creation, itt->second.creation, itt->second.jid_id);

			osqliteparser->SqlExe(updatesql);
		}
	}
	delete osqliteparser;
	return 0;

}
int waios2android::write_chat_list_table(const char* dbfile)
{
	wasqliteparsing* osqliteparser = new wasqliteparsing(dbfile);

	const char* deltable = "delete from chat_list";
	osqliteparser->SqlExe((char*)deltable);

	std::map<int, sessioninfo>::iterator itt;
	for (itt = msessionmap.begin(); itt != msessionmap.end(); itt++)
	{
		if (itt->second.key_remote_jid == mselfjid)
		{
			continue;
		}
		Replace(itt->second.subject, "'", "''");
		int sqllen = itt->second.subject.length() + itt->second.key_remote_jid.length() + 512;
		char *sqlstr=new char[sqllen];

		sqllen = sprintf(sqlstr, "INSERT INTO chat_list ( \"_id\",\"key_remote_jid\", \"message_table_id\", \"subject\", \"creation\", \"last_message_table_id\") VALUES(  %d, \"%s\", %d, \"%s\",%lld, %d)",
			itt->second.id,itt->second.key_remote_jid.c_str(), itt->second.message_table_id, itt->second.subject.c_str(), itt->second.creation, itt->second.last_message_table_id);
		sqlstr[sqllen] = 0;
		osqliteparser->SqlExe(sqlstr);
		delete[] sqlstr;
	}
	delete osqliteparser;
	return 0;

}
int waios2android::write_group_participants_table(const char* dbfile)
{

	wasqliteparsing* fosqliteparser = new wasqliteparsing(dbfile);

	const char* deltablestr = "delete from group_participants";
	fosqliteparser->SqlExe((char*)deltablestr);
	delete fosqliteparser;
	wasqliteparsing* osqliteparser = new wasqliteparsing(dbfile);
	const char* deltable = "delete from group_participants";
	osqliteparser->SqlExe((char*)deltable);
	std::map<int, groupmemberinfo>::iterator itt;
	for (itt = mgroupmemberinfomap.begin(); itt != mgroupmemberinfomap.end(); itt++)
	{
		int sqllen = itt->second.jid.length()  + 512;
		char* sqlstr = new char[sqllen];

		sqllen = sprintf(sqlstr, "INSERT INTO group_participants (\"_id\", \"gjid\", \"jid\", \"admin\", \"pending\", \"sent_sender_key\") VALUES(%d, \"%s\", \"%s\", %d, %d, NULL)",
			itt->second.id, itt->second.gjid.c_str(), itt->second.jid.c_str(), itt->second.admin, itt->second.pending);

		sqlstr[sqllen] = 0;
		osqliteparser->SqlExe(sqlstr);
		delete[] sqlstr;
		
		bool isadd;
		getchatjiEx(osqliteparser, itt->second.jid.c_str(), 0, isadd);
	}
	delete osqliteparser;
	return 0;
}
int waios2android::Writetrandat(const char* scrfile,const char *dstfile)
{
	
	std::string tmpdstfile = std::string(dstfile);
#ifdef WIN32
	Replace(tmpdstfile, "/", "\\");
	if (_access(tmpdstfile.c_str(), 0) == 0)
	{
		return 0;
	}
#endif
	CreatePath(GetFilePath(tmpdstfile.c_str()).c_str());

	copyfile(scrfile, tmpdstfile.c_str());

	remove(scrfile);

	return 0;

}
int waios2android::getwhserialize(attserializeinfo oattserialize,std::string chatid, std::string& data)
{
	javaSerialize ojavaSerialize;
	int len = 0;
	char* fbuf = ojavaSerialize.getserilazedata(oattserialize.latitude.c_str(), oattserialize.longitude.c_str(), oattserialize.filename.c_str(), oattserialize.filesize.c_str(), len);
	if (fbuf)
	{
		data = bytestohexstring(fbuf, len);
		delete[] fbuf;

	}
	return 0;
}
int waios2android::sendpress(int step, int tol, int cur)
{
	printf("step:%d,t:%d,c:%d\r\n",step,tol,cur);
	return 0;
}
int waios2android::write_messages_media_table(const char* dbfile)
{
	std::map<int, messengesmediainfo>::iterator itt;
	wasqliteparsing* osqliteparser = new wasqliteparsing(dbfile);

	int totalsize = mchatmediasarr.size();

	int index = 0;
	for (itt = mchatmediasarr.begin(); itt != mchatmediasarr.end(); itt++)
	{
		Replace(itt->second.media_name, "'", "''");
		Replace(itt->second.file_path, "'", "''");
		//¡ª¡ªid
		itt->second.chat_row_id = getchatidfrommap(itt->second.rejid);
		if (itt->second.chat_row_id == -1)
			continue;
		int sqllen = itt->second.file_path.length() + itt->second.media_name.length() + 2048;
		char* sqlstr = new char[sqllen];
		sqllen = sprintf(sqlstr, "INSERT INTO message_media(message_row_id, chat_row_id, autotransfer_retry_enabled, transferred,transcoded, file_path, file_size, suspicious_content, trim_from, trim_to, face_x, face_y,width, height, has_streaming_sidecar, gif_attribution, thumbnail_height_width_ratio, first_scan_length, mime_type, file_length, media_name, media_duration, page_count, is_animated_sticker)VALUES(%d, %d, %d, %d, %d, \'%s\',\'%s\', %d, %d, %d, %d, %d, \'%s\', \'%s\', %d, %d, \'%s\', %d, \'%s\',\'%s\',\'%s\',%d,%d,%d);",
				itt->second.message_row_id, itt->second.chat_row_id, itt->second.autotransfer_retry_enable, itt->second.transferred, itt->second.transcoded, itt->second.file_path.c_str(), itt->second.file_size.c_str(), itt->second.suspicious_content, itt->second.trim_from, itt->second.trim_to, itt->second.face_x, itt->second.face_y
				, itt->second.width.c_str(), itt->second.height.c_str(), itt->second.has_streaming_sidecar, itt->second.git_attribution, itt->second.thumbnail_height_width_ratio.c_str(), itt->second.first_scan_length, itt->second.mime_type.c_str(), itt->second.file_length.c_str(),
				itt->second.media_name.c_str(), itt->second.media_duration, itt->second.page_count, itt->second.is_animated_sticker);
		sqlstr[sqllen] = 0;
		osqliteparser->SqlExe(sqlstr);
		delete[] sqlstr;
	}

	delete osqliteparser;
	return 0;

}

void waios2android::getsort(std::string jid, int& sort)
{
	std::string jiddr = "1";
	std::map<std::string, int>::iterator itt;
	itt = msorftmap.find(jiddr);
	if (itt == msorftmap.end())
	{
		sort = 1;
		msorftmap.insert(make_pair(jiddr, 1));

	}
	else
	{

		sort = ++itt->second;
	}

}
//message
int waios2android::write_message_table(const char* dbfile)
{
	std::map<int, messengesinfo>::iterator itt;
	wasqliteparsing* osqliteparser = new wasqliteparsing(dbfile);

	const char* deltable = "delete from message where _id != 1";
	osqliteparser->SqlExe((char*)deltable);


	int totalsize = mchatmessagesarr.size();
	sendpress(445, totalsize, 0);
	int index = 0;
	for (itt = mchatmessagesarr.begin(); itt != mchatmessagesarr.end(); itt++)
	{
		int cid = getchatidfrommap(itt->second.key_remote_jid);
		if (cid == -1)
		{

			continue;
		}
		int sender_jid_row_id = getjididfrommap(itt->second.sender_jid);
		int sort_id = 0;
		getsort(itt->second.key_remote_jid,sort_id);
		std::string th_dst = "";
		std::string raw_dst = "";
		//
		char* raw_data = NULL;
		if (!itt->second.thlocalfile.empty())
		{//

			Writetrandat(itt->second.thlocalfile.c_str(), itt->second.remotethfile.c_str());

			unsigned int thbuflen = 0;
			char* raw_data = CReadFromFile(itt->second.remotethfile.c_str(), thbuflen);
			if (raw_data)
			{
				raw_dst = bytestohexstring(raw_data, thbuflen);
				delete[] raw_data;
			}

		}
		if (!itt->second.attlocalfile.empty())
		{//

			Writetrandat( itt->second.attlocalfile.c_str(), itt->second.remoteattfile.c_str());

			attserializeinfo oattserialize;
			oattserialize.filename = itt->second.mediadatafile;
			oattserialize.filesize = itt->second.media_size;
			int findxb = itt->second.latitude.find(".");
			int findyb = itt->second.longitude.find(".");
			oattserialize.latitude = itt->second.latitude.substr(0, findxb);
			oattserialize.longitude = itt->second.latitude.substr(0, findyb);
			//
			getwhserialize(oattserialize, itt->second.key_id, th_dst);
		}
		Replace(itt->second.data, "'", "''");
		Replace(itt->second.media_caption, "'", "''");
		int sqllen = itt->second.data.length() + itt->second.key_remote_jid.length() + itt->second.media_hash.length() + itt->second.media_url.length() + itt->second.media_name.length() + itt->second.remote_resource.length()
			+ itt->second.media_caption.length() + th_dst.length() + raw_dst.length() + 2048;
		char* sqlstr = new char[sqllen];
		sqllen = sprintf(sqlstr, "INSERT INTO message(_id, chat_row_id, from_me, key_id,sender_jid_row_id,status, broadcast, recipient_count, origination_flags, origin, timestamp,received_timestamp, receipt_server_timestamp, message_type,\
			text_data, starred, lookup_tables,\
			message_add_on_flags, sort_id) VALUES\
			(%d, %d, %d, \'%s\', %d, %d, %d,%d, %d, %d, %lld, %lld, %lld, %d, \'%s\',%d,%d,%d,%d);",
				itt->second._id, cid, itt->second.key_from_me, itt->second.key_id.c_str(), sender_jid_row_id,
				itt->second.status, 0, 1, 0, itt->second.origin, itt->second.send_timestamp,
				itt->second.received_timestamp, itt->second.receipt_server_timestamp, itt->second.media_wa_type,
				itt->second.data.c_str(), itt->second.starred, 0,
				0, sort_id);
		sqlstr[sqllen] = 0;
		osqliteparser->SqlExe(sqlstr);

		delete[] sqlstr;
		index++;
		if (index % 1000 == 0)
		{
			sendpress(445, totalsize, index);
		}
	}
	delete osqliteparser;
	return 0;
}
bool waios2android::ismessagstableexist(const char* dbfile)
{
	
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "select count(_id) from messages");
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	wasqliteparsing osqliteparser(dbfile);
	osqliteparser.SqlQuery(slqstr, vmapstr);
	
	if (vmapstr.size() == 0)
	{
		return false;
	}
	
	return true;
}

int waios2android::write_messages_table(const char* dbfile)
{

	if (ismessagstableexist(dbfile) == false)
	{
		AWriteLog("waios2android", "messages table is not exist");
		write_message_table(dbfile);
		return 0;
	}

	std::map<int, messengesinfo>::iterator itt;
	wasqliteparsing* osqliteparser = new wasqliteparsing(dbfile);

	const char* deltable = "delete from messages where _id != 1";
	osqliteparser->SqlExe((char*)deltable);
	AWriteLog("waios2android", "mchatmessagesarr Size:%d", mchatmessagesarr.size());

	int totalsize = mchatmessagesarr.size();
	sendpress(445, totalsize,0);
	int index = 0;
	for (itt = mchatmessagesarr.begin(); itt != mchatmessagesarr.end(); itt++)
	{

		
		std::string th_dst = "";
		std::string raw_dst = "";
		//
		char* raw_data=NULL;
		if (!itt->second.thlocalfile.empty())
		{//

			Writetrandat(itt->second.thlocalfile.c_str(), itt->second.remotethfile.c_str());

			unsigned int thbuflen = 0;
			char* raw_data = CReadFromFile(itt->second.remotethfile.c_str(), thbuflen);
			if (raw_data)
			{
				raw_dst = bytestohexstring(raw_data, thbuflen);
				delete[] raw_data;
			}
			
		}
		if (!itt->second.attlocalfile.empty())
		{//

			Writetrandat( itt->second.attlocalfile.c_str(), itt->second.remoteattfile.c_str());

			attserializeinfo oattserialize;
			oattserialize.filename = itt->second.mediadatafile;
			oattserialize.filesize = itt->second.media_size;
			int findxb = itt->second.latitude.find(".");
			int findyb = itt->second.longitude.find(".");
			oattserialize.latitude = itt->second.latitude.substr(0, findxb);
			oattserialize.longitude = itt->second.latitude.substr(0, findyb);
			//
			getwhserialize(oattserialize, itt->second.key_id, th_dst);
		}
		Replace(itt->second.data, "'", "''");
		Replace(itt->second.media_caption, "'", "''");
		int sqllen = itt->second.data.length() + itt->second.key_remote_jid.length() + itt->second.media_hash.length() + itt->second.media_url.length() + itt->second.media_name.length() + itt->second.remote_resource.length()
			+ itt->second.media_caption.length()+th_dst.length() + raw_dst.length()+ 2048;
		char* sqlstr = new char[sqllen];
		sqllen = sprintf(sqlstr, "INSERT INTO messages(_id, key_remote_jid, key_from_me, key_id,status, needs_push, data, timestamp, media_url, media_mime_type, media_wa_type, media_size,	media_name, media_hash, media_duration, origin, latitude, longitude, thumb_image, remote_resource,received_timestamp, send_timestamp, receipt_server_timestamp, receipt_device_timestamp, raw_data,recipient_count, media_caption,starred) VALUES(%d, \'%s\', %d, \'%s\', %d, %d, \'%s\', %lld, \'%s\', \'%s\', %d, %s,\'%s\', \'%s\', %d, %d, %s, %s, X\'%s\', \'%s\',%lld, %lld, %lld, %lld, X\'%s\', NULL, \'%s\',%d);",
			itt->second._id, itt->second.key_remote_jid.c_str(), itt->second.key_from_me, itt->second.key_id.c_str(),
			itt->second.status, itt->second.need_push, itt->second.data.c_str(), itt->second.timestamp, itt->second.media_url.c_str(), itt->second.media_mime_type.c_str(), itt->second.media_wa_type, itt->second.media_size.c_str(),
			itt->second.media_name.c_str(), itt->second.media_hash.c_str(), itt->second.media_duration, itt->second.origin, itt->second.latitude.c_str(), itt->second.longitude.c_str(), th_dst.c_str(), itt->second.remote_resource.c_str(),
			itt->second.received_timestamp, itt->second.send_timestamp, itt->second.receipt_server_timestamp, itt->second.receipt_server_timestamp, raw_dst.c_str(),
			itt->second.media_caption.c_str(), itt->second.starred);
			sqlstr[sqllen] = 0;
			osqliteparser->SqlExe(sqlstr);

			delete[] sqlstr;
			index++;
			if ( index % 1000 ==0 )
			{
				sendpress(445, totalsize, index);
			}
	}
	delete osqliteparser;
	return 0;

}
//
int waios2android::copy_medaifile()
{

	return 0;
}