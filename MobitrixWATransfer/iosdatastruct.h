#pragma once
#include <string>

struct Z_METADATA {

	std::string Z_VERSION;
	std::string Z_UUID;

	std::string Z_PLIST;
};
struct ZPRIMARYKEY {
	int Z_ENT;
	std::string Z_NAME;
	int Z_SUPER;//0
	int Z_MAX;//
	ZPRIMARYKEY() {
		Z_SUPER = 0;
		Z_MAX = 0;
	};
};


struct ZWACHATSESSION {

	int Z_PK;//
	int Z_ENT;///4
	int Z_OPT;//10
	int ZARCHIVED;//.0
	int ZCONTACTABID;//0
	int ZFLAGS;//1296
	int ZHIDDEN;//0
	int ZIDENTITYVERIFICATIONEPOCH;//0
	int ZIDENTITYVERIFICATIONSTATE;//0
	int ZMESSAGECOUNTER;//
	int ZREMOVED;//0
	int ZSESSIONTYPE;// 0: 1:
	//int ZSPOTLIGHTSTATUS;
	int ZUNREADCOUNT;//0
	int ZGROUPINFO;
	int ZLASTMESSAGE;
	//	int ZPROPERTIES;
	int ZLASTMESSAGEDATE;
	//	int ZLOCATIONSHARINGENDDATE;
	std::string ZCONTACTIDENTIFIER;
	std::string ZCONTACTJID;
	//std::string ZETAG;
	//std::string ZLASTMESSAGETEXT;
	std::string ZPARTNERNAME;
	int sort;
	int u_id;
	int jjid;
	//std::string ZSAVEDINPUT;

	ZWACHATSESSION() {

		Z_ENT = 4;
		Z_OPT = 10;
		ZARCHIVED = 0;
		ZCONTACTABID = 0;
		ZFLAGS = 1296;
		ZHIDDEN = 0;
		ZIDENTITYVERIFICATIONEPOCH = 0;
		ZIDENTITYVERIFICATIONSTATE = 0;
		ZREMOVED = 0;
		ZUNREADCOUNT = 0;
		ZLASTMESSAGE = 0;
		ZMESSAGECOUNTER = 0;
		sort = 0;
		ZGROUPINFO = 0;
		ZLASTMESSAGEDATE = 0;

	};
};


struct ZWAGROUPINFO {
	int Z_PK;
	int Z_ENT;//5
	int Z_OPT;//10
	int ZSTATE;//0
	int ZCHATSESSION;
	//	std::string ZLASTMESSAGEOWNER;//""
	std::string ZCREATIONDATE;
	std::string ZSUBJECTTIMESTAMP;
	//	std::string ZCREATORJID;//
	//	std::string ZOWNERJID;
	//	std::string ZPICTUREID;
	//	std::string ZPICTUREPATH;
	//	std::string ZSOURCEJID;
	std::string ZSUBJECTOWNERJID;//
	ZWAGROUPINFO() {
		Z_ENT = 5;
		Z_OPT = 10;
		ZSTATE = 0;

	};

};
struct ZWAGROUPMEMBER {
	int Z_PK;
	int Z_ENT;//6
	int Z_OPT;//0++
	int ZCONTACTABID;//0
	int ZISACTIVE;//1
	int ISADMIN;//0-1
	int ZSENDERKEYSENT;//0
	int ZCHATSESSION;//
	int ZRECENTGROUPCHAT;//0
	std::string ZCONTACTIDENTIFIER;
	std::string ZCONTACTNAME;
	std::string ZFIRSTNAME;//""
	std::string ZMEMBERJID;
	ZWAGROUPMEMBER() {
		Z_ENT = 6;
		ZCONTACTABID = 0;
		ZISACTIVE = 1;
		ZSENDERKEYSENT = 0;
		ZRECENTGROUPCHAT = 0;
		ZFIRSTNAME = "";
	};

};
//
struct ZWAMEDIAITEM {
	int Z_PK;
	int	Z_ENT;//8
	int	Z_OPT;//4
//	std::string	ZCLOUDSTATUS;//""
	std::string	ZFILESIZE;//
	int	ZMEDIAORIGIN;
	int	ZMOVIEDURATION;
	int	ZMESSAGE;
	std::string	ZASPECTRATIO;//0.0
	std::string	ZHACCURACY;//0.0
	std::string	ZLATITUDE;//0.0
	std::string	ZLONGITUDE;//0.0
	std::string	ZMEDIAURLDATE;//""
	std::string	ZAUTHORNAME;//
//	std::string	ZCOLLECTIONNAME;//""
	std::string	ZMEDIALOCALPATH;
	std::string	ZMEDIAURL;//
	//std::string	ZTHUMBNAILLOCALPATH;//""
	std::string	ZTITLE;
	std::string	ZVCARDNAME;
	std::string	ZVCARDSTRING;
	std::string	ZXMPPTHUMBPATH;
	//	char*	ZMEDIAKEY;//""
	//	char* ZMETADATA;//""

	std::string attlocalfilename;
	std::string manifest_relativePath;
	std::string iphonelocalpath;
	std::string backupfilename;
	std::string fileID;


	std::string thumlocalfilename;
	std::string thum_manifest_relativePath;
	std::string thum_iphonelocalpath;
	std::string thum_backupfilename;
	std::string thum_fileID;

	int fandroid;


	ZWAMEDIAITEM() {
		Z_ENT = 8;
		Z_OPT = 4;
		//	ZCLOUDSTATUS = "";
		ZASPECTRATIO = "0.0";
		ZHACCURACY = "0.0";

		ZLATITUDE = "0.0";
		ZLONGITUDE = "0.0";
		//	ZMEDIAURLDATE = "";
		//	ZCOLLECTIONNAME = "";
		//	ZTHUMBNAILLOCALPATH = "";
		fandroid = 0;
	};
};

struct ZWAMESSAGE {
	int Z_ID;
	int Z_PK;//
	int 	Z_ENT;//9
	int 	Z_OPT;//4
	//int 	ZCHILDMESSAGESDELIVEREDCOUNT;
	//int 	ZCHILDMESSAGESPLAYEDCOUNT;
	//int 	ZCHILDMESSAGESREADCOUNT;
	//int 	ZDATAITEMVERSION;
	int 	ZDOCID;//0++
	//int 	ZENCRETRYCOUNT;
	int 	ZFILTEREDRECIPIENTCOUNT;//1
	int 	ZFLAGS;//0
	int 	ZGROUPEVENTTYPE;//0
	int 	ZISFROMME;//0-1
//	int 	ZMESSAGEERRORSTATUS;
	int 	ZMESSAGESTATUS;
	int 	ZMESSAGETYPE;
	int 	ZSORT;//1++
	//int 	ZSPOTLIGHTSTATUS;
	int 	ZSTARRED;//0
	int 	ZCHATSESSION;
	std::string key_remote_jid;
	std::string 	ZGROUPMEMBER;
	//	int 	ZLASTSESSION;
	std::string  	ZMEDIAITEM;
	//	int 	ZMESSAGEINFO;
	//	int 	ZPARENTMESSAGE;
	std::string 	ZMESSAGEDATE;
	//	std::string  	ZSENTDATE;
	std::string 	ZFROMJID;
	std::string 	ZMEDIASECTIONID;
	//	std::string 	ZPHASH;
	//	std::string 	ZPUSHNAME;
	std::string 	ZSTANZAID;
	std::string 	ZTEXT;
	std::string 	ZTOJID;

	ZWAMESSAGE() {
		Z_ENT = 9;
		Z_OPT = 4;
		ZFILTEREDRECIPIENTCOUNT = 1;
		ZFLAGS = 0;
		ZGROUPEVENTTYPE = 0;
		ZSTARRED = 0;
		ZMEDIAITEM = "";

	};

};
struct ManifestFilesInfo {
	std::string fileID;
	std::string domain;
	std::string relativePath;
	int falsgs;
	std::string file;

};