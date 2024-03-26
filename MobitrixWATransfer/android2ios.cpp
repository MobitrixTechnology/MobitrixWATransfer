#include "android2ios.h"
#include "publicfun.h"

#include "wasqliteparsing.h"
#include <time.h>
#include "plistapi.h"
#include "BaseLog.h"
#ifdef WIN32
#include <process.h>
#endif

int android2ios::start_work(const char* androiddbpath, const char* androidmediapath, std::string iospath,  std::string datapath,int watype)
{
	m_iosdevice = "00000000-0000000000000000";
	ishadiosmsgdb = false;
	m_watype = watype;
	AWriteLog("android2ios", "Start Android Data to IOS");
	mmessageZDOCID = 0;

	if (!iospath.empty())
	{

		mChatStoragedbfile = iospath;
		mChatStoragedbfile += m_iosdevice;
		if(m_watype==0)
		{

		mChatStoragedbfile += "\\7c\\7c7fba66680ef796b916b067077cc246adacf01d";

		}
		else {
		

			mChatStoragedbfile += "\\72\\724bd3b98b18518b455a87c1f3ac3a0d189c4466";

		}
	}

	if (IsPathExist(mChatStoragedbfile.c_str()))
	{
		ishadiosmsgdb = true;
		mishadolddata = true;
	}

	mmessagezpk = 0;
	mideviceid = std::string(m_iosdevice);
	if (mishadolddata)
	{

		mManifestdb = iospath;
		mManifestdb += m_iosdevice;
		mManifestdb += "\\Manifest.db";
		miospath = std::string(iospath);
		mold_infoplist = iospath;
		mold_infoplist += m_iosdevice;
		mold_infoplist += "\\Info.plist";
		mold_Manifestplist = iospath;
		mold_Manifestplist += m_iosdevice;
		mold_Manifestplist += "\\Manifest.plist";
		mold_Statusplist = iospath;
		mold_Statusplist += m_iosdevice;
		mold_Statusplist += "\\Status.plist";


	}
	mmediazpk = 0;
	mInodeNumber = myrandom(1200000, 1900000);
	

	mrestorepath = datapath;


	char curtime[32];
	memset(curtime, 0, 32);
	MGetCurTime(curtime, false);

	mrestorepath += curtime;
	mrestorepath += "\\";

	datapath = std::string(mrestorepath.c_str());
	CreatePath(mrestorepath.c_str());

	mNewChatStoragedbfile = mrestorepath;
	mNewChatStoragedbfile += m_iosdevice;
	if (m_watype == 0)
	{

		mNewChatStoragedbfile += "\\7c\\";


	}
	else {

		mNewChatStoragedbfile += "\\72\\";

	
	}

	CreatePath(mNewChatStoragedbfile.c_str());
	if (m_watype == 0)
	{
		mNewChatStoragedbfile += "7c7fba66680ef796b916b067077cc246adacf01d";
	}
	else
	{
		mNewChatStoragedbfile += "724bd3b98b18518b455a87c1f3ac3a0d189c4466";
	}

	mnewManifestdb = mrestorepath;
	mnewManifestdb += m_iosdevice;

	mnewManifestdb += "\\Manifest.db";


	mnew_infoplist = mrestorepath;
	mnew_infoplist += m_iosdevice;

	mnew_infoplist += "\\Info.plist";

	mnew_Manifestplist = mrestorepath;
	mnew_Manifestplist += m_iosdevice;

	mnew_Manifestplist += "\\Manifest.plist";



	mnew_Statusplist = mrestorepath;
	mnew_Statusplist += m_iosdevice;

	mnew_Statusplist += "\\Status.plist";


	mpermediapath = androidmediapath;

	mpermediapath += "tmp\\";

	CreatePath(mpermediapath.c_str());

	mandroidpath = androidmediapath;
	mmsgstoredbfile = androiddbpath;

	androidmainpath = androidmediapath;
	if (m_watype == 0)
	{
		androidmainpath += "backup\\WhatsApp\\";
	}
	else {
		androidmainpath += "backup\\WhatsApp Business\\";
	}




	if (!IsPathExist(mmsgstoredbfile.c_str()))
	{
		return -1;

	}
#ifdef WIN32
	mwadbfile = mandroidpath;
	mwadbfile += "databases\\wa.db";

	mxmlfile = mandroidpath;
	mxmlfile += "shared_prefs\\com.whatsapp.registration.RegisterPhone.xml";
	mlimxmlfile = mandroidpath;
	if (m_watype == 0)
	{
		mlimxmlfile += "shared_prefs\\com.hwhatsapp_preferences_light.xml";
	}
	else {
		mlimxmlfile += "shared_prefs\\com.whatsapp.hw4b_preferences_light.xml";
	}
#else
	mwadbfile = mandroidpath;
	mwadbfile += "databases/wa.db";

	mxmlfile = mandroidpath;
	mxmlfile += "shared_prefs/com.whatsapp.registration.RegisterPhone.xml";
	mlimxmlfile = mandroidpath;
	if (m_watype == 0)
	{
		mlimxmlfile += "shared_prefs/com.hwhatsapp_preferences_light.xml";
	}
	else {
		mlimxmlfile += "shared_prefs/com.whatsapp.hw4b_preferences_light.xml";
	}


#endif
	readselfjid();
	initmprimarymap();

	readoldManifestdbA();
	if (mishadolddata)
	{
		readmetadata();
		readchatsessionfromiphone();
		readgrouinfofromiphone();
		readgroumemberinfofromiphone();
		readmediafromiphone();
		readmessagechatfromiphone();
	}
	readchatsesisonfromandroid();
	readchatsesisonfromandroidEx();
	//读取Jid
	readjidfromdb();
	readgrouinffromandroid();
	readgrouinffromandroidEx();
	readgroumemberinffromandroid();
	readgroumemberinffromandroidA();
	readgroumemberinffromandroidEx();
	readmthumbmap();
	readmthumbmapB();
	AWriteLog("android2ios", "Over");
	return 0;
}


int getmessagerownumber(const char* filename)
{

	//查询总聊天条数
	std::string m_chattable = "messages";
	int m_totalnum = 0;
	wasqliteparsing osqliteparser(filename);
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "select count(_id) from messages");
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser.SqlQuery(slqstr, vmapstr))
	{
		if (vmapstr.size() == 0)
		{
			sqllen = sprintf(slqstr, "select count(_id) from message");
			slqstr[sqllen] = 0;
			if (osqliteparser.SqlQuery(slqstr, vmapstr))
			{
				if (vmapstr.size() == 1)
				{
					m_totalnum = atoi(FindDataFromMap(vmapstr.at(0), "count(_id)").c_str());
					m_chattable = "message";
				}

			}
		}
		else
		{
			if (vmapstr.size() == 1)
				m_totalnum = atoi(FindDataFromMap(vmapstr.at(0), "count(_id)").c_str());
		}

	}
	return m_totalnum;
	
}

int android2ios::read_work()
{

	m_chattable = "messages";
	m_totalnum = 0;
	wasqliteparsing osqliteparser(mmsgstoredbfile.c_str());
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "select count(_id) from messages");
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	osqliteparser.SqlQuery(slqstr, vmapstr);

	if (vmapstr.size() == 0)
	{
		sqllen=sprintf(slqstr, "select count(_id) from message");
		slqstr[sqllen] = 0;
		if (osqliteparser.SqlQuery(slqstr, vmapstr))
		{
			if (vmapstr.size() == 1)
			{
				m_totalnum = atoi(FindDataFromMap(vmapstr.at(0), "count(_id)").c_str());
				m_chattable = "message";
			}

		}
	}
	else
	{
		if (vmapstr.size() == 1)
			m_totalnum = atoi(FindDataFromMap(vmapstr.at(0), "count(_id)").c_str());
	}




	initrestoredata();


	//Thread S
	m_chatlimitindex=0;
	m_runthreadnumber= 4;
	lpdealchatinfo olpdealchatinfo = new dealchatinfo;
	olpdealchatinfo->androidtoios = this;
	olpdealchatinfo->tablename = m_chattable;
	int tnum = m_runthreadnumber;
	for (int n = 0; n < tnum; n++)
	{

		olpdealchatinfo->tindex = n;
		AWriteLog("android2ios", "delandroidchatServer thread begin[%d]", n);
#ifdef WIN32

		_beginthreadex(NULL, 0, delandroidchatServer, (void*)olpdealchatinfo, 0, NULL);
#else
		pthread_t thread_id;
		pthread_create(&thread_id, NULL, delandroidchatServer, (void*)olpdealchatinfo);
#endif
		sleep(1);

	}
	while (m_runthreadnumber != 0)
	{
		sleep(5);
	}
	delete olpdealchatinfo;


	return 0;
}

int android2ios::dealmessagepk()
{
	int index = 1;

	std::map<int, ZWAMESSAGE*>::iterator itt;
	for (itt = mwamessagemap.begin(); itt != mwamessagemap.end(); itt++)
	{
		itt->second->Z_PK = index;
		index++;
	}


	std::map<int, ZWAMEDIAITEM*>::iterator itt1;
	for (itt1 = mwamediamap.begin(); itt1 != mwamediamap.end(); itt1++)
	{
		itt = mwamessagemap.find(itt1->second->ZMESSAGE);
		if (itt != mwamessagemap.end())
		{

			itt1->second->ZMESSAGE = itt->second->Z_PK;
		}
	}

	std::map<int, ZWACHATSESSION>::iterator itt2;
	for (itt2 = mchatsessionmap.begin(); itt2 != mchatsessionmap.end(); itt2++)
	{

		itt = mwamessagemap.find(itt2->second.ZLASTMESSAGE);
		if (itt != mwamessagemap.end())
		{
			itt2->second.ZLASTMESSAGE= itt->second->Z_PK;
		}
	}
	return 0;
}


int android2ios::write_work()
{


	dealmessagepk();
	writesessiontable();
	writegroupinfotable();
	writegroupmemberinfotable();
	writeamessageinfotable();

	writemediainfotable();

	writeprimaryinfo();
	writezmetadata();

	insertnewdbinfo();
	writeManifestdb();
	buildrestoreplist();
	return 0;

}

int android2ios::initrestoredata()
{

	copyfile("ChatStorage_eg.sqlite", mNewChatStoragedbfile.c_str());

	copyfile("Manifest_eg.db", mnewManifestdb.c_str());
	if(!ishadiosmsgdb)
	{
		mZ_METADATA.Z_VERSION ="1";
		mZ_METADATA.Z_UUID = generate();
		mZ_METADATA.Z_PLIST = "62706C6973743030DF10100102030405060708090A0B0C0D0E0F1011121314161711181913393A133C3D3E5477612E725D732E646F6349444F66667365745E4D6564696153656374696F6E49445F101E4E5353746F72654D6F64656C56657273696F6E4964656E746966696572735F10135643617264734D656E74696F6E73496E6465785F10125F4E534175746F56616375756D4C6576656C5F101577612E66757475726570726F6F6656657273696F6E5F101F4E5353746F72654D6F64656C56657273696F6E4861736865734469676573745F10194E5353746F72654D6F64656C56657273696F6E4861736865735E4C697374735265717565737465645F101D4E5350657273697374656E63654672616D65776F726B56657273696F6E5F10204E5353746F72654D6F64656C56657273696F6E48617368657356657273696F6E5F102442697A4368617453797374656D4D6573736167654164646564466F7254776F54696572735F100F47726F7570735265717565737465645F101277612E7265706169726564496E64657865735B4E5353746F7265547970651004100709A11557322E31322E313433000000000000000051325F10586C384A2B4B386270795846654A48767034497954757A684A4C746E6E4A6D4F457958396654333655566D2B31694D6C615070747A47444B30703146704D7653337A786C576F6953476778305044694633342B485063773D3DDF100F1A1B1C1D1E1F202122232425262728292A2B2C2D2E2F30313233343536375D574147726F75704D656D6265725F101057414368617450757368436F6E6669675F100F5741426C61636B4C6973744974656D5F1011574150726F66696C65507573684E616D655F101657415A315061796D656E745472616E73616374696F6E5D57414368617453657373696F6E5F101057414368617450726F706572746965735B57414D656469614974656D5B574147726F7570496E666F5957414D6573736167655D57414D657373616765496E666F5E574156436172644D656E74696F6E5F1014574147726F75704D656D626572734368616E67655F1014574150726F66696C65506963747572654974656D5F101157414D657373616765446174614974656D4F1020096E35F37392F6A41023984AEEBE0D7F4E187D31741F8286AEE4A22353330B1A4F102026C3AA5C2BA087DC9B9508B326983276AF4D3AAE2D451FFE2A407B373727C5D14F1020B54037A7EF644AE5878923E769AC33F762451B565B5F5958A974392D928A26B24F10206B4CA82FDCEFDA511BBAC4BD4FAB72F83490D60344974DF76384F60A6C0902DE4F1020A2ABB3FDDD16AC221D7D0DFD957F0479A7139C846E668EBF036D4279798166444F10207081F4CA101D4740DF9E9A68D6EA0A9F87CE245B3E3A59F1A6019EEE13C45F534F102017867F4DE0B75073448F040A60CEE3693290523B88212D5D185196F5C44AC9884F1020E40F4841A184D2295139AA314E19948E3994CC5DCD471A524299B81E6FBF553A4F1020AD1F229EC16B17E64C7BC3A88CE08F3DEBFBC5D52A4F345BECAE305D87C510804F10209B60946BD2974D60FF56AE59E59AF24236BE34E33A1AA4F76321BC1E5151AB854F1020C29F46AF01FF96A43A01EC296E76406BE6EA699AD193A308076474E9ECC9F8634F1020F9F1D3D6CDBC19E7CA7D440E3588CB89946B235068D2533724D037652F9984F84F102077556FC5BA7D6553534AB0D3BF8263D4DC12404A71965745BB925964F927AAA24F1020D61C57B40188730F47D39264FE1A8D6EBF43BA52907CEEB69D9AF2AEDF4436DD4F10207A08B5729E8C7A77ADC918405E407D7896A0A19643A859C6F8225822DC06E5CE091103D11003092341D79EAD5B18AD6410015653514C6974650008002B0030003E004D006E0084009900B100D300EF00FE011E01410168017A018F019B019D019F01A001A201AA01B301B502100231023F0252026402780291029F02B202BE02CA02D402E202F10308031F033303560379039C03BF03E204050428044B046E049104B404D704FA051D0540054105440546054705500552000000000000020100000000000003F00000000000000000000000000000559";

	}
	return 0;
}
int android2ios::readmetadata()
{
	if (!IsPathExist(mChatStoragedbfile.c_str()))
	{
		return 0;

	}

	wasqliteparsing osqliteparser(mChatStoragedbfile.c_str());
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "select Z_VERSION,Z_UUID,hex(Z_PLIST) from Z_METADATA");
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser.SqlQuery(slqstr, vmapstr))
	{
		for (int n = 0; n < vmapstr.size(); n++)
		{
			mZ_METADATA.Z_VERSION = FindDataFromMap(vmapstr.at(n), "Z_VERSION");
			mZ_METADATA.Z_UUID = FindDataFromMap(vmapstr.at(n), "Z_UUID");
			mZ_METADATA.Z_PLIST = FindDataFromMap(vmapstr.at(n), "hex(Z_PLIST)");


		}
	}
	return 0;
}
int android2ios::readselfjid()
{
	mselfjid = "";
	mphonestr = "";
	if (!IsPathExist(mxmlfile.c_str()))
	{
		return -1;

	}
	int isregphone = -1;
	unsigned int len;
	char* xmlbuf=CReadFromFile(mxmlfile.c_str(),len);
	if (xmlbuf)
	{
		std::string countrycode= GetMidData(xmlbuf, "<string name=\"com.whatsapp.registration.RegisterPhone.country_code\">", "</string>");
		std::string phonenmuber=GetMidData(xmlbuf, "<string name=\"com.whatsapp.registration.RegisterPhone.phone_number\">","</string>");

		if (phonenmuber.length() != 0)
		{
			mphonestr = countrycode + phonenmuber;
			mselfjid = countrycode + phonenmuber;

			mselfjid += "@s.whatsapp.net";
			isregphone = 0;
		}
		delete[] xmlbuf;
	}


	if (len == 0)
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
int android2ios::initmprimarymap()
{
	ZPRIMARYKEY onZPRIMARYKEY1;
	onZPRIMARYKEY1.Z_ENT = 1;
	onZPRIMARYKEY1.Z_NAME = "WABlackListItem";
	mprimarymap.insert(std::make_pair(onZPRIMARYKEY1.Z_ENT, onZPRIMARYKEY1));


	ZPRIMARYKEY onZPRIMARYKEY2;
	onZPRIMARYKEY2.Z_ENT = 2;
	onZPRIMARYKEY2.Z_NAME = "WAChatProperties";
	mprimarymap.insert(std::make_pair(onZPRIMARYKEY2.Z_ENT, onZPRIMARYKEY2));

	ZPRIMARYKEY onZPRIMARYKEY3;
	onZPRIMARYKEY3.Z_ENT = 3;
	onZPRIMARYKEY3.Z_NAME = "WAChatPushConfig";
	mprimarymap.insert(std::make_pair(onZPRIMARYKEY3.Z_ENT, onZPRIMARYKEY3));


	ZPRIMARYKEY onZPRIMARYKEY4;
	onZPRIMARYKEY4.Z_ENT = 4;
	onZPRIMARYKEY4.Z_NAME = "WAChatSession";
	mprimarymap.insert(std::make_pair(onZPRIMARYKEY4.Z_ENT, onZPRIMARYKEY4));


	ZPRIMARYKEY onZPRIMARYKEY5;
	onZPRIMARYKEY5.Z_ENT = 5;
	onZPRIMARYKEY5.Z_NAME = "WAGroupInfo";
	mprimarymap.insert(std::make_pair(onZPRIMARYKEY5.Z_ENT, onZPRIMARYKEY5));

	ZPRIMARYKEY onZPRIMARYKEY6;
	onZPRIMARYKEY6.Z_ENT = 6;
	onZPRIMARYKEY6.Z_NAME = "WAGroupMember";
	mprimarymap.insert(std::make_pair(onZPRIMARYKEY6.Z_ENT, onZPRIMARYKEY6));



	ZPRIMARYKEY onZPRIMARYKEY7;
	onZPRIMARYKEY7.Z_ENT = 7;
	onZPRIMARYKEY7.Z_NAME = "WAGroupMembersChange";
	mprimarymap.insert(std::make_pair(onZPRIMARYKEY7.Z_ENT, onZPRIMARYKEY7));

	ZPRIMARYKEY onZPRIMARYKEY8;
	onZPRIMARYKEY8.Z_ENT = 8;
	onZPRIMARYKEY8.Z_NAME = "WAMediaItem";
	mprimarymap.insert(std::make_pair(onZPRIMARYKEY8.Z_ENT, onZPRIMARYKEY8));

	ZPRIMARYKEY onZPRIMARYKEY9;
	onZPRIMARYKEY9.Z_ENT = 9;
	onZPRIMARYKEY9.Z_NAME = "WAMessage";
	mprimarymap.insert(std::make_pair(onZPRIMARYKEY9.Z_ENT, onZPRIMARYKEY9));



	ZPRIMARYKEY onZPRIMARYKEY10;
	onZPRIMARYKEY10.Z_ENT = 10;
	onZPRIMARYKEY10.Z_NAME = "WAMessageDataItem";
	mprimarymap.insert(std::make_pair(onZPRIMARYKEY10.Z_ENT, onZPRIMARYKEY10));

	ZPRIMARYKEY onZPRIMARYKEY11;
	onZPRIMARYKEY11.Z_ENT = 11;
	onZPRIMARYKEY11.Z_NAME = "WAMessageInfo";
	mprimarymap.insert(std::make_pair(onZPRIMARYKEY11.Z_ENT, onZPRIMARYKEY11));

	ZPRIMARYKEY onZPRIMARYKEY12;
	onZPRIMARYKEY12.Z_ENT = 12;
	onZPRIMARYKEY12.Z_NAME = "WAProfilePictureItem";
	mprimarymap.insert(std::make_pair(onZPRIMARYKEY12.Z_ENT, onZPRIMARYKEY12));



	ZPRIMARYKEY onZPRIMARYKEY13;
	onZPRIMARYKEY13.Z_ENT = 13;
	onZPRIMARYKEY13.Z_NAME = "WAProfilePushName";
	mprimarymap.insert(std::make_pair(onZPRIMARYKEY13.Z_ENT, onZPRIMARYKEY13));


	ZPRIMARYKEY onZPRIMARYKEY14;
	onZPRIMARYKEY14.Z_ENT = 14;
	onZPRIMARYKEY14.Z_NAME = "WAVCardMention";
	mprimarymap.insert(std::make_pair(onZPRIMARYKEY14.Z_ENT, onZPRIMARYKEY14));

	ZPRIMARYKEY onZPRIMARYKEY15;
	onZPRIMARYKEY15.Z_ENT = 15;
	onZPRIMARYKEY15.Z_NAME = "WAZ1PaymentTransaction";
	mprimarymap.insert(std::make_pair(onZPRIMARYKEY15.Z_ENT, onZPRIMARYKEY15));

	return 0;

}
int android2ios::updateprimarymap(int index, int max)
{
	std::map<int, ZPRIMARYKEY>::iterator itt;
	itt = mprimarymap.find(index);
	if (itt != mprimarymap.end())
	{
		itt->second.Z_MAX = max;

	}

	return 0;
}
std::string android2ios::getsubjectStr(const char* jid)
{
	if (!IsPathExist(mwadbfile.c_str()))
	{
		return "";
	}
	wasqliteparsing osqliteparser(mwadbfile.c_str());
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "select display_name from wa_contacts where jid==\'%s\'", jid);
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser.SqlQuery(slqstr, vmapstr))
	{
		if (vmapstr.size() == 0)
		{
			return "";
		}

		return  FindDataFromMap(vmapstr.at(0), "display_name");
	}
	return "";


}
int android2ios::readchatsessionfromiphone()
{
	if (!IsPathExist(mChatStoragedbfile.c_str()))
	{
		return 0;

	}
	wasqliteparsing osqliteparser(mChatStoragedbfile.c_str());
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "select Z_PK,ZMESSAGECOUNTER,ZSESSIONTYPE,ZGROUPINFO ,ZLASTMESSAGE,ZLASTMESSAGEDATE,ZCONTACTIDENTIFIER,ZCONTACTJID,ZPARTNERNAME from ZWACHATSESSION");
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser.SqlQuery(slqstr, vmapstr))
	{
		for (int n = 0; n < vmapstr.size(); n++)
		{
			printf("n:%d\r\n", n);
			ZWACHATSESSION oZWACHATSESSION;
			oZWACHATSESSION.Z_PK =atoi( FindDataFromMap(vmapstr.at(n), "Z_PK").c_str());
			oZWACHATSESSION.ZMESSAGECOUNTER = atoi(FindDataFromMap(vmapstr.at(n), "ZMESSAGECOUNTER").c_str());
			oZWACHATSESSION.ZSESSIONTYPE = atoi(FindDataFromMap(vmapstr.at(n), "ZSESSIONTYPE").c_str());

			oZWACHATSESSION.ZGROUPINFO = atoi(FindDataFromMap(vmapstr.at(n), "ZGROUPINFO").c_str());
			oZWACHATSESSION.ZLASTMESSAGE = atoi(FindDataFromMap(vmapstr.at(n), "ZLASTMESSAGE").c_str());
			oZWACHATSESSION.ZLASTMESSAGEDATE = atoi(FindDataFromMap(vmapstr.at(n), "ZLASTMESSAGEDATE").c_str());

			oZWACHATSESSION.ZCONTACTIDENTIFIER = FindDataFromMap(vmapstr.at(n), "ZCONTACTIDENTIFIER");
			oZWACHATSESSION.ZCONTACTJID = FindDataFromMap(vmapstr.at(n), "ZCONTACTJID");
			oZWACHATSESSION.ZPARTNERNAME = FindDataFromMap(vmapstr.at(n), "ZPARTNERNAME");

			mchatmaxzpk = oZWACHATSESSION.Z_PK > mchatmaxzpk ? oZWACHATSESSION.Z_PK : mchatmaxzpk;
			mcheckchatmessionma.insert(std::make_pair(oZWACHATSESSION.ZCONTACTJID, oZWACHATSESSION.Z_PK));
			//int64_t formatiostm_wm(std::string iostm)
			mchatsessionmap.insert(std::make_pair(oZWACHATSESSION.Z_PK, oZWACHATSESSION));
		}
	}
	return 0;
}
bool android2ios::isexitchatsession(std::string jid)
{
	std::map<std::string, int>::iterator itr;
	itr = mcheckchatmessionma.find(jid);
	if(itr!= mcheckchatmessionma.end()){
		return true;
		
	}
	return false;

}
int android2ios::getsessionpk(std::string jid)
{
	std::map<std::string, int>::iterator itr;
	itr = mcheckchatmessionma.find(jid);
	if (itr != mcheckchatmessionma.end()) {
		return itr->second;

	}
	return -1;

}
bool android2ios::updatechat(int zpk, int date, int chatpk)
{
	std::map<int, ZWACHATSESSION>::iterator itt;
	itt = mchatsessionmap.find(zpk);
	if (itt != mchatsessionmap.end())
	{
		itt->second.ZMESSAGECOUNTER++;
		if (itt->second.ZLASTMESSAGE < chatpk)
		{
			itt->second.ZLASTMESSAGE = chatpk;
			itt->second.ZLASTMESSAGEDATE = date;
		}
	

	}
	return 0;



}
int android2ios::readchatsesisonfromandroid()
{

	wasqliteparsing osqliteparser(mmsgstoredbfile.c_str());
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "select key_remote_jid,subject,creation,archived from chat_list");
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser.SqlQuery(slqstr, vmapstr))
	{

		for (int n = 0; n < vmapstr.size(); n++)
		{
			std::string key_remote_jid =FindDataFromMap(vmapstr.at(n), "key_remote_jid");
			if (isexitchatsession(key_remote_jid))
			{
				continue;
			}
		//	AWriteLog("android2ios", "key_remote_jid: %s", key_remote_jid.c_str());
			mchatmaxzpk++;
		//	printf("n:%d\r\n", n);
			ZWACHATSESSION oZWACHATSESSION;
			oZWACHATSESSION.Z_PK = mchatmaxzpk;

		  //oZWACHATSESSION.ZMESSAGECOUNTER = atoi(FindDataFromMap(vmapstr.at(n), "ZMESSAGECOUNTER").c_str());
		  //oZWACHATSESSION.ZGROUPINFO = atoi(FindDataFromMap(vmapstr.at(n), "ZGROUPINFO").c_str());
		  //oZWACHATSESSION.ZLASTMESSAGE = atoi(FindDataFromMap(vmapstr.at(n), "ZLASTMESSAGE").c_str());
		  //oZWACHATSESSION.ZLASTMESSAGEDATE = atoi(FindDataFromMap(vmapstr.at(n), "ZLASTMESSAGEDATE").c_str());

			if (ComParRight(key_remote_jid, "@s.whatsapp.net"))
			{

				oZWACHATSESSION.ZSESSIONTYPE = 0;
			}
			else if (ComParRight(key_remote_jid, "@g.us"))
			{
				oZWACHATSESSION.ZSESSIONTYPE =1;
			}
			else//????????????///
			{
				oZWACHATSESSION.ZSESSIONTYPE = 0;
				//continue;
			}
			if (oZWACHATSESSION.ZSESSIONTYPE == 0)
			{
				std::string guid = generate();

				oZWACHATSESSION.ZCONTACTIDENTIFIER = guid + ":ABPerson";
			}


			oZWACHATSESSION.ZCONTACTJID = key_remote_jid;
			
			std::string subjestStr = FindDataFromMap(vmapstr.at(n), "subject");
			if (subjestStr.empty())
			{
				subjestStr=getsubjectStr(key_remote_jid.c_str());

			}
			if (subjestStr.empty())
			{
				std::string tmpstr = std::string(key_remote_jid);
				std::vector<std::string> sVector;
				SplitStoV((char*)tmpstr.c_str(), tmpstr.length(), "@", sVector);
				if (sVector.size() == 2)
				{

					subjestStr = sVector.at(0);
				}
				else
				{
					subjestStr = key_remote_jid;
				}

			}
			oZWACHATSESSION.ZPARTNERNAME = subjestStr;

			std::map<std::string, int>::iterator itt;
			itt = mcheckchatmessionma.find(oZWACHATSESSION.ZCONTACTJID);
			if (itt == mcheckchatmessionma.end())
			{
				mcheckchatmessionma.insert(std::make_pair(oZWACHATSESSION.ZCONTACTJID, oZWACHATSESSION.Z_PK));
				mchatsessionmap.insert(std::make_pair(oZWACHATSESSION.Z_PK, oZWACHATSESSION));
			}
		}
	}
	return 0;

}
//SELECT subject, raw_string, created_timestamp, archived FROM chat u LEFT JOIN jid s ON u.jid_row_id = s._id;
int android2ios::readjidfromdb()
{
	wasqliteparsing osqliteparser(mmsgstoredbfile.c_str());
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "SELECT _id , raw_string FROM jid");
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser.SqlQuery(slqstr, vmapstr))
	{

		for (int n = 0; n < vmapstr.size(); n++)
		{
			std::string key_remote_jid = FindDataFromMap(vmapstr.at(n), "raw_string");
			std::string _id = FindDataFromMap(vmapstr.at(n), "_id");
			if (_id != "" && key_remote_jid != "")
			{
				int tid = atoi(_id.c_str());
				std::map<int, std::string>::iterator itt;
				itt = mjididmap.find(tid);
				if (itt == mjididmap.end())
				{
					mjididmap.insert(make_pair(tid, key_remote_jid));
				}
			}
				

		}
	}

}
int android2ios::readchatsesisonfromandroidEx()
{
	wasqliteparsing osqliteparser(mmsgstoredbfile.c_str());
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "SELECT u._id,subject,s._id as jjid, raw_string, created_timestamp, archived FROM chat u LEFT JOIN jid s ON u.jid_row_id = s._id where u.hidden==0");
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser.SqlQuery(slqstr, vmapstr))
	{

		for (int n = 0; n < vmapstr.size(); n++)
		{
			std::string key_remote_jid = FindDataFromMap(vmapstr.at(n), "raw_string");
			if (isexitchatsession(key_remote_jid))
			{
				continue;
			}
			mchatmaxzpk++;
			//	printf("n:%d\r\n", n);
			ZWACHATSESSION oZWACHATSESSION;
			oZWACHATSESSION.Z_PK = mchatmaxzpk;
			if (ComParRight(key_remote_jid, "@s.whatsapp.net"))
			{

				oZWACHATSESSION.ZSESSIONTYPE = 0;
			}
			else if (ComParRight(key_remote_jid, "@g.us"))
			{
				oZWACHATSESSION.ZSESSIONTYPE = 1;
			}
			else if (ComParRight(key_remote_jid, "@temp"))
			{
				continue;
			}
			else//????????????///
			{
				oZWACHATSESSION.ZSESSIONTYPE = 0;
				//continue;
			}
			if (oZWACHATSESSION.ZSESSIONTYPE == 0)
			{
				std::string guid = generate();

				oZWACHATSESSION.ZCONTACTIDENTIFIER = guid + ":ABPerson";
			}
			oZWACHATSESSION.ZCONTACTJID = key_remote_jid;
			std::string subjestStr = FindDataFromMap(vmapstr.at(n), "subject");
			if (subjestStr.empty())
			{
				subjestStr = getsubjectStr(key_remote_jid.c_str());

			}


			if (subjestStr.empty())
			{
				std::string tmpstr = std::string(key_remote_jid);
				std::vector<std::string> sVector;
				SplitStoV((char*)tmpstr.c_str(), tmpstr.length(), "@", sVector);
				if (sVector.size() == 2)
				{

					subjestStr = sVector.at(0);
				}
				else
				{
					subjestStr = key_remote_jid;
				}

			}
			std::string _id = FindDataFromMap(vmapstr.at(n), "_id");
			if(_id!="")
				oZWACHATSESSION.u_id = atoi(_id.c_str());

			std::string jjid = FindDataFromMap(vmapstr.at(n), "jjid");
			if (jjid != "")
				oZWACHATSESSION.jjid = atoi(jjid.c_str());

			oZWACHATSESSION.ZPARTNERNAME = subjestStr;

			std::string _archived = FindDataFromMap(vmapstr.at(n), "archived");
			if (_archived != "")
				oZWACHATSESSION.ZARCHIVED = atoi(_archived.c_str());


			std::map<std::string, int>::iterator itt;
			itt = mcheckchatmessionma.find(oZWACHATSESSION.ZCONTACTJID);
			if (itt == mcheckchatmessionma.end())
			{
				mcheckchatmessionma.insert(std::make_pair(oZWACHATSESSION.ZCONTACTJID, oZWACHATSESSION.Z_PK));
				mchatsessionmap.insert(std::make_pair(oZWACHATSESSION.Z_PK, oZWACHATSESSION));
				mchatidmap.insert(std::make_pair(oZWACHATSESSION.u_id, oZWACHATSESSION.ZCONTACTJID));
				mjididmap.insert(std::make_pair(oZWACHATSESSION.jjid, oZWACHATSESSION.ZCONTACTJID));
			}
		}
	}
	return 0;

}
std::string android2ios::getchatromte_row_jid(const char* cid)
{
	int a = atoi(cid);
	std::map<int, std::string>::iterator itt;
	itt = mchatidmap.find(a);
	if (itt != mchatidmap.end())
	{
		return itt->second;
	}
	return "";
}
std::string android2ios::getjid_romte_row_jid(const char* jjid)
{
	int a = atoi(jjid);
	std::map<int, std::string>::iterator itt;
	itt = mjididmap.find(a);
	if (itt != mjididmap.end())
	{
		return itt->second;
	}
	return "";
}
int android2ios::setgroupinftosession(int seid, int groupinfoid)
{
	std::map<int, ZWACHATSESSION>::iterator itt;
	itt = mchatsessionmap.find(seid);
	if (itt != mchatsessionmap.end())
	{
		itt->second.ZGROUPINFO = groupinfoid;

	}
	return 0;
}
int android2ios::updatesort(int sessionid, int msort)
{

	std::map<int, ZWACHATSESSION>::iterator itt;
	itt = mchatsessionmap.find(sessionid);

	if (itt != mchatsessionmap.end())
	{
	
		itt->second.sort = itt->second.sort < msort ? msort : itt->second.sort;
	}

	return 0;

}


int android2ios::readmthumbmap()
{
	wasqliteparsing osqliteparser(mmsgstoredbfile.c_str());
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "select thumbnail,key_id from message_thumbnails");
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, wablockbytes> > vmapstr;
	if (osqliteparser.SqlQuery(slqstr, vmapstr))
	{

		for (int n = 0; n < vmapstr.size(); n++)
		{

				wablockbytes o = vmapstr.at(n)["key_id"];
				wablockbytes o1 = vmapstr.at(n)["thumbnail"];
				wablockbytes thumdata;

				thumdata.pdata = new char[o1.plen];
				thumdata.plen = o1.plen;
				memcpy(thumdata.pdata, o1.pdata, o1.plen);
				delete[] o1.pdata ;
				std::string key_id = std::string(o.pdata, o.plen);
				delete[] o.pdata;
				std::map<std::string, wablockbytes>::iterator itt;
				itt = mthumbmap.find(key_id);
				if (itt == mthumbmap.end())
				{
					mthumbmap.insert(make_pair(key_id, thumdata));
				}

		}
	}
	vmapstr.clear();

	return 0;
}

int android2ios::readmthumbmapA()
{

	wasqliteparsing osqliteparser(mmsgstoredbfile.c_str());
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "select hex(thumbnail),message_row_id from message_thumbnail");
	slqstr[sqllen] = 0;

	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser.SqlQuery(slqstr, vmapstr))
	{
		for (int n = 0; n < vmapstr.size(); n++)
		{
			std::map<int, std::string>::iterator itt;
			std::string thumdata = FindDataFromMap(vmapstr.at(n), "hex(thumbnail)");
			std::string message_row_id = FindDataFromMap(vmapstr.at(n), "message_row_id");
			if (thumdata.size() != 0 && message_row_id.size() != 0)
			{
				int mid = atoi(message_row_id.c_str());
				itt = mthumbmap_a.find(mid);
				if (itt == mthumbmap_a.end())
				{
					mthumbmap_a.insert(make_pair(mid, thumdata));
				}
			}
		}
	}
	return 0;
}

int android2ios::readmthumbmapB()
{

	wasqliteparsing osqliteparser(mmsgstoredbfile.c_str());

	int cureindex = 1;
	int number = 0;
	while (true)
	{
		char slqstr[1024];
		int sqllen = sprintf(slqstr, "select hex(thumbnail),message_row_id from message_thumbnail order by  message_row_id desc limit  %d,10000", cureindex+ number*10000);
		slqstr[sqllen] = 0;

		std::vector<std::map<std::string, std::string> > vmapstr;
		if (osqliteparser.SqlQuery(slqstr, vmapstr))
		{
			if (vmapstr.size() == 0)
			{
				break;
			}
			for (int n = 0; n < vmapstr.size(); n++)
			{
				std::map<int, std::string>::iterator itt;
				std::string thumdata = FindDataFromMap(vmapstr.at(n), "hex(thumbnail)");
				std::string message_row_id = FindDataFromMap(vmapstr.at(n), "message_row_id");
				if (thumdata.size() != 0 && message_row_id.size() != 0)
				{
					int mid = atoi(message_row_id.c_str());
					itt = mthumbmap_a.find(mid);
					if (itt == mthumbmap_a.end())
					{
						mthumbmap_a.insert(make_pair(mid, thumdata));
					}
				}
			}
		}
		number++;
		if (number > 25)
			break;
	}
	return 0;
}

bool android2ios::getisgroupstatus(std::string kjid, bool& isgroup)
{


	std::map<int, ZWACHATSESSION>::iterator itt;
	for (itt = mchatsessionmap.begin(); itt != mchatsessionmap.end(); itt++)
	{

		if (itt->second.ZCONTACTJID == kjid)
		{
			itt->second.sort = itt->second.sort + 1;

			isgroup = itt->second.ZSESSIONTYPE == 0 ? false : true;
			return true;
		}
	}

	return false;

}

//
int android2ios::readgrouinfofromiphone()
{
	if (!IsPathExist(mChatStoragedbfile.c_str()))
	{
		return 0;

	}
	wasqliteparsing osqliteparser(mChatStoragedbfile.c_str());
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "select Z_PK,ZCHATSESSION,ZCREATIONDATE ,ZSUBJECTTIMESTAMP,ZSUBJECTOWNERJID from ZWAGROUPINFO");
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser.SqlQuery(slqstr, vmapstr))
	{
		for (int n = 0; n < vmapstr.size(); n++)
		{
		
			ZWAGROUPINFO oZWAGROUPINFO;

			oZWAGROUPINFO.Z_PK = atoi(FindDataFromMap(vmapstr.at(n), "Z_PK").c_str());
			oZWAGROUPINFO.ZCHATSESSION = atoi(FindDataFromMap(vmapstr.at(n), "ZCHATSESSION").c_str());
			oZWAGROUPINFO.ZCREATIONDATE =FindDataFromMap(vmapstr.at(n), "ZCREATIONDATE");

			oZWAGROUPINFO.ZSUBJECTTIMESTAMP = FindDataFromMap(vmapstr.at(n), "ZSUBJECTTIMESTAMP");
			oZWAGROUPINFO.ZSUBJECTOWNERJID = FindDataFromMap(vmapstr.at(n), "ZSUBJECTOWNERJID");
			
			mgrouinfomaxpk = oZWAGROUPINFO.Z_PK > mgrouinfomaxpk ? oZWAGROUPINFO.Z_PK : mgrouinfomaxpk;

			mcheckgroupinfoma.insert(std::make_pair(oZWAGROUPINFO.ZSUBJECTOWNERJID, oZWAGROUPINFO.Z_PK));

			//int64_t formatiostm_wm(std::string iostm)
			mgroupinfomap.insert(std::make_pair(oZWAGROUPINFO.Z_PK, oZWAGROUPINFO));
		}
	}
	return 0;


}
bool android2ios::isexitgrougid(std::string jid)
{
	std::map<std::string, int>::iterator itr;
	itr = mcheckgroupinfoma.find(jid);
	if (itr != mcheckgroupinfoma.end()) {
		return true;

	}
	return false;

}
int android2ios::readgrouinffromandroid()
{
	wasqliteparsing osqliteparser(mmsgstoredbfile.c_str());
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "select key_remote_jid,creation from chat_list");

	//
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser.SqlQuery(slqstr, vmapstr))
	{

		for (int n = 0; n < vmapstr.size(); n++)
		{

			std::string key_remote_jid = FindDataFromMap(vmapstr.at(n), "key_remote_jid");
			if (!ComParRight(key_remote_jid, "@g.us"))
			{
				continue;
			}

			if (isexitgrougid(key_remote_jid))
			{
				continue;
			}

			int sessionid = getsessionpk(key_remote_jid);
			if (sessionid == -1)
			{
				continue;
			}

			ZWAGROUPINFO oZWAGROUPINFO;
			mgrouinfomaxpk++;
			oZWAGROUPINFO.Z_PK = mgrouinfomaxpk;
			oZWAGROUPINFO.ZCHATSESSION = sessionid;

			std::string creationstr = FindDataFromMap(vmapstr.at(n), "creation");
			oZWAGROUPINFO.ZCREATIONDATE = getiostm(creationstr.c_str());


			oZWAGROUPINFO.ZSUBJECTTIMESTAMP = oZWAGROUPINFO.ZCREATIONDATE;
			oZWAGROUPINFO.ZSUBJECTOWNERJID = key_remote_jid;

			mgrouinfomaxpk = oZWAGROUPINFO.Z_PK > mgrouinfomaxpk ? oZWAGROUPINFO.Z_PK : mgrouinfomaxpk;

			mcheckgroupinfoma.insert(std::make_pair(oZWAGROUPINFO.ZSUBJECTOWNERJID, oZWAGROUPINFO.Z_PK));
			setgroupinftosession(sessionid, oZWAGROUPINFO.Z_PK);
			//int64_t formatiostm_wm(std::string iostm)
			mgroupinfomap.insert(std::make_pair(oZWAGROUPINFO.Z_PK, oZWAGROUPINFO));


		}
	}
	return 0;


}

int android2ios::readgrouinffromandroidEx()
{
	wasqliteparsing osqliteparser(mmsgstoredbfile.c_str());
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "SELECT subject, raw_string, created_timestamp, archived FROM chat u LEFT JOIN jid s ON u.jid_row_id = s._id where u.hidden == 0");

	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser.SqlQuery(slqstr, vmapstr))
	{

		for (int n = 0; n < vmapstr.size(); n++)
		{

			std::string key_remote_jid = FindDataFromMap(vmapstr.at(n), "raw_string");
			if (!ComParRight(key_remote_jid, "@g.us"))
			{
				continue;
			}

			if (isexitgrougid(key_remote_jid))
			{
				continue;
			}

			int sessionid = getsessionpk(key_remote_jid);
			if (sessionid == -1)
			{
				continue;
			}

			ZWAGROUPINFO oZWAGROUPINFO;
			mgrouinfomaxpk++;
			oZWAGROUPINFO.Z_PK = mgrouinfomaxpk;
			oZWAGROUPINFO.ZCHATSESSION = sessionid;

			std::string creationstr = FindDataFromMap(vmapstr.at(n), "created_timestamp");
			oZWAGROUPINFO.ZCREATIONDATE = getiostm(creationstr.c_str());

			oZWAGROUPINFO.ZSUBJECTTIMESTAMP = oZWAGROUPINFO.ZCREATIONDATE;
			oZWAGROUPINFO.ZSUBJECTOWNERJID = key_remote_jid;

			mgrouinfomaxpk = oZWAGROUPINFO.Z_PK > mgrouinfomaxpk ? oZWAGROUPINFO.Z_PK : mgrouinfomaxpk;

			mcheckgroupinfoma.insert(std::make_pair(oZWAGROUPINFO.ZSUBJECTOWNERJID, oZWAGROUPINFO.Z_PK));
			setgroupinftosession(sessionid, oZWAGROUPINFO.Z_PK);
			//int64_t formatiostm_wm(std::string iostm)
			mgroupinfomap.insert(std::make_pair(oZWAGROUPINFO.Z_PK, oZWAGROUPINFO));


		}
	}
	return 0;


}
int android2ios::readgroumemberinfofromiphone()
{
	if (!IsPathExist(mChatStoragedbfile.c_str()))
	{
		return 0;

	}
	wasqliteparsing osqliteparser(mChatStoragedbfile.c_str());
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "select Z_PK,Z_OPT,ISADMIN ,ZCHATSESSION,ZCONTACTIDENTIFIER,ZCONTACTNAME,ZMEMBERJID from ZWAGROUPMEMBER");
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser.SqlQuery(slqstr, vmapstr))
	{
		for (int n = 0; n < vmapstr.size(); n++)
		{
			
			ZWAGROUPMEMBER oZWAGROUPMEMBER;

			oZWAGROUPMEMBER.Z_PK = atoi(FindDataFromMap(vmapstr.at(n), "Z_PK").c_str());
			oZWAGROUPMEMBER.Z_OPT = atoi(FindDataFromMap(vmapstr.at(n), "Z_OPT").c_str());

			oZWAGROUPMEMBER.ZCHATSESSION = atoi(FindDataFromMap(vmapstr.at(n), "ZCHATSESSION").c_str());
			oZWAGROUPMEMBER.ISADMIN = atoi(FindDataFromMap(vmapstr.at(n), "ISADMIN").c_str());
			oZWAGROUPMEMBER.ZCONTACTIDENTIFIER = FindDataFromMap(vmapstr.at(n), "ZCONTACTIDENTIFIER");

			oZWAGROUPMEMBER.ZCONTACTNAME = FindDataFromMap(vmapstr.at(n), "ZCONTACTNAME");
			oZWAGROUPMEMBER.ZMEMBERJID = FindDataFromMap(vmapstr.at(n), "ZMEMBERJID");

			mgroupmemberindex = oZWAGROUPMEMBER.Z_PK > mgroupmemberindex ? oZWAGROUPMEMBER.Z_PK : mgroupmemberindex;
			mgroupmemberopt = oZWAGROUPMEMBER.Z_OPT > mgroupmemberopt? oZWAGROUPMEMBER.Z_OPT : mgroupmemberopt;
			std::string mkey = oZWAGROUPMEMBER.ZMEMBERJID;
			mkey += FindDataFromMap(vmapstr.at(n), "ZCHATSESSION");

		//	mgroupmembermap.insert(std::make_pair(oZWAGROUPINFO.ZSUBJECTOWNERJID, oZWAGROUPINFO.Z_PK));

			//int64_t formatiostm_wm(std::string iostm)
			mgroupmembermap.insert(std::make_pair(mkey, oZWAGROUPMEMBER));
		}
	}
	return 0;

}
int android2ios::readgroumemberinffromandroid()
{

	wasqliteparsing osqliteparser(mmsgstoredbfile.c_str());
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "select gjid,admin,jid from group_participants");
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser.SqlQuery(slqstr, vmapstr))
	{
		for (int n = 0; n < vmapstr.size(); n++)
		{
			std::string gjid = FindDataFromMap(vmapstr.at(n), "gjid");
			int sessionid = getsessionpk(gjid);
			if (sessionid == -1)
			{
				continue;
			}
			


			std::string jid = FindDataFromMap(vmapstr.at(n), "jid");
			if (jid.empty())
			{
				jid = mselfjid;
			}

			char jid_chatid[64] = {0}; 
			sprintf(jid_chatid,"%s%d", jid.c_str(), sessionid);
			if (isexitgrougmember(jid_chatid))
			{
				continue;
			}

			ZWAGROUPMEMBER oZWAGROUPMEMBER;
			int admintype = atoi(FindDataFromMap(vmapstr.at(n), "admin").c_str());
			if (admintype == 0)
			{
				oZWAGROUPMEMBER.ISADMIN = 0;
			}
			else
			{
				oZWAGROUPMEMBER.ISADMIN = 1;
			}
			std::vector<std::string> sVector;
			std::string tmpstr = std::string(jid);
			SplitStoV((char*)tmpstr.c_str(), tmpstr.length(), "@", sVector);
			if (sVector.size() == 2)
			{
				oZWAGROUPMEMBER.ZCONTACTNAME = sVector.at(0);


			}
			else
			{
				continue;
			}
			oZWAGROUPMEMBER.ZCHATSESSION = sessionid;
			mgroupmemberindex++;
			oZWAGROUPMEMBER.Z_PK = mgroupmemberindex;
			mgroupmemberopt++;
			oZWAGROUPMEMBER.Z_OPT = mgroupmemberopt;

			oZWAGROUPMEMBER.ZCONTACTIDENTIFIER = generate();
			oZWAGROUPMEMBER.ZMEMBERJID = jid;

			mgroupmembermap.insert(std::make_pair(jid_chatid, oZWAGROUPMEMBER));

		}
	}
	return 0;

}
int android2ios::readgroumemberinffromandroidA()
{

	wasqliteparsing osqliteparser(mmsgstoredbfile.c_str());
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "select gjid,jid from group_participants_history");
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser.SqlQuery(slqstr, vmapstr))
	{
		for (int n = 0; n < vmapstr.size(); n++)
		{
			std::string gjid = FindDataFromMap(vmapstr.at(n), "gjid");
			int sessionid = getsessionpk(gjid);
			if (sessionid == -1)
			{
				continue;
			}
			std::string jid = FindDataFromMap(vmapstr.at(n), "jid");
			if (jid.empty()|| jid=="")
			{
				continue;
			}
			if (strstr(jid.c_str(), ","))
			{
				continue;
			}
			char jid_chatid[64] = { 0 };
			sprintf(jid_chatid, "%s%d", jid.c_str(), sessionid);
			if (isexitgrougmember(jid_chatid))
			{
				continue;
			}
		
			ZWAGROUPMEMBER oZWAGROUPMEMBER;
			oZWAGROUPMEMBER.ISADMIN = 0;

			std::vector<std::string> sVector;
			std::string tmpstr = std::string(jid);
			SplitStoV((char*)tmpstr.c_str(), tmpstr.length(), "@", sVector);
			if (sVector.size() == 2)
			{
				oZWAGROUPMEMBER.ZCONTACTNAME = sVector.at(0);
			}
			else
			{
				continue;
			}
			oZWAGROUPMEMBER.ZCHATSESSION = sessionid;
			mgroupmemberindex++;
			oZWAGROUPMEMBER.Z_PK = mgroupmemberindex;
			mgroupmemberopt++;
			oZWAGROUPMEMBER.Z_OPT = mgroupmemberopt;

			oZWAGROUPMEMBER.ZCONTACTIDENTIFIER = generate();
			oZWAGROUPMEMBER.ZMEMBERJID = jid;

			mgroupmembermap.insert(std::make_pair(jid_chatid, oZWAGROUPMEMBER));

		}
	}
	return 0;

}

int android2ios::readgroumemberinffromandroidEx()
{
	wasqliteparsing osqliteparser(mmsgstoredbfile.c_str());
	std::map<std::string, int>::iterator itr;
	for (itr = mcheckchatmessionma.begin(); itr != mcheckchatmessionma.end(); itr++)
	{
		if (!ComParRight(itr->first, "@g.us"))
		{
			continue;
		}
		int sessionid = itr->second;
		char slqstr[1024];
		int sqllen = sprintf(slqstr, "select _id from jid where raw_string=\"%s\"", itr->first.c_str());
		slqstr[sqllen] = 0;

		std::vector<std::map<std::string, std::string> > vmapstr;
		if (osqliteparser.SqlQuery(slqstr, vmapstr))
		{
			if (vmapstr.size() == 1)
			{

				std::string _gjid = FindDataFromMap(vmapstr.at(0), "_id");

				sqllen = sprintf(slqstr, "select user_jid_row_id,raw_string from group_participant_user a LEFT JOIN jid s ON a.user_jid_row_id=s._id where a.group_jid_row_id==%s", _gjid.c_str());
				slqstr[sqllen] = 0;
				std::vector<std::map<std::string, std::string> > gmemvmapstr;
				if (osqliteparser.SqlQuery(slqstr, gmemvmapstr))
				{
					for (int n = 0; n < gmemvmapstr.size(); n++)
					{


						std::string jid = FindDataFromMap(gmemvmapstr.at(n), "raw_string");
						if (jid.empty() || jid == ""|| jid == "status_me" )
						{
							continue;
						}
						if (strstr(jid.c_str(), ","))
						{
							continue;
						}


						char jid_chatid[64] = { 0 };
						sprintf(jid_chatid, "%s%d", jid.c_str(), sessionid);
						if (isexitgrougmember(jid_chatid))
						{
							continue;
						}

						ZWAGROUPMEMBER oZWAGROUPMEMBER;
						oZWAGROUPMEMBER.ISADMIN = 0;

						std::vector<std::string> sVector;
						std::string tmpstr = std::string(jid);
						SplitStoV((char*)tmpstr.c_str(), tmpstr.length(), "@", sVector);
						if (sVector.size() == 2)
						{
							oZWAGROUPMEMBER.ZCONTACTNAME = sVector.at(0);

						}
						else
						{
							continue;
						}
						oZWAGROUPMEMBER.ZCHATSESSION = sessionid;
						mgroupmemberindex++;
						oZWAGROUPMEMBER.Z_PK = mgroupmemberindex;
						mgroupmemberopt++;
						oZWAGROUPMEMBER.Z_OPT = mgroupmemberopt;

						oZWAGROUPMEMBER.ZCONTACTIDENTIFIER = generate();
						oZWAGROUPMEMBER.ZMEMBERJID = jid;
						mgroupmembermap.insert(std::make_pair(jid_chatid, oZWAGROUPMEMBER));

					}

				}
			}



		}
	}
	return 0;

}
bool android2ios::isexitgrougmember(std::string jid_chatid)
{
	std::map<std::string, ZWAGROUPMEMBER>::iterator itt;
	itt = mgroupmembermap.find(jid_chatid);
	if (itt != mgroupmembermap.end())
	{
		return true;
	}
	return false;

}


int android2ios::addgroupmemeber(std::string gjid, std::string jid)
{

	int sessionid = getsessionpk(gjid);
	if (sessionid == -1)
	{
		return -1;
	}
	char jid_chatid[64] = { 0 };
	sprintf(jid_chatid, "%s%d", jid.c_str(), sessionid);

	ZWAGROUPMEMBER oZWAGROUPMEMBER;
	oZWAGROUPMEMBER.ISADMIN = 0;
	std::vector<std::string> sVector;
	std::string tmpstr = std::string(jid);
	SplitStoV((char*)tmpstr.c_str(), tmpstr.length(), "@", sVector);
	if (sVector.size() == 2)
	{
		oZWAGROUPMEMBER.ZCONTACTNAME = sVector.at(0);
	}
	else
	{
		return -1;
	}
	oZWAGROUPMEMBER.ZCHATSESSION = sessionid;
	mgroupmemberindex++;
	oZWAGROUPMEMBER.Z_PK = mgroupmemberindex;
	mgroupmemberopt++;
	oZWAGROUPMEMBER.Z_OPT = mgroupmemberopt;

	oZWAGROUPMEMBER.ZCONTACTIDENTIFIER = generate();
	oZWAGROUPMEMBER.ZMEMBERJID = jid;

	mgroupmembermap.insert(std::make_pair(jid_chatid, oZWAGROUPMEMBER));

	return mgroupmemberindex;

}
int android2ios::getzpkgrougmember(std::string jid_chatid)
{
	std::map<std::string, ZWAGROUPMEMBER>::iterator itt;
	itt = mgroupmembermap.find(jid_chatid);
	if (itt != mgroupmembermap.end())
	{
		return itt->second.Z_PK;
	}
	else
	{



	}
	return -1;

}

int android2ios::getzpkgrougmemberEx(std::string jid_chatid, std::string gjid, std::string jid)
{
	std::map<std::string, ZWAGROUPMEMBER>::iterator itt;
	itt = mgroupmembermap.find(jid_chatid);
	if (itt != mgroupmembermap.end())
	{
		return itt->second.Z_PK;
	}
	else
	{

		return addgroupmemeber(gjid, jid);

	}
	return -1;

}
int android2ios::insertothergroumemberEx(std::string jid,int sessionid)
{

	ZWAGROUPMEMBER oZWAGROUPMEMBER;
	oZWAGROUPMEMBER.ISADMIN = 0;

	std::vector<std::string> sVector;
	std::string tmpstr = std::string(jid);
	SplitStoV((char*)tmpstr.c_str(), tmpstr.length(), "@", sVector);
	if (sVector.size() == 2)
	{
		oZWAGROUPMEMBER.ZCONTACTNAME = sVector.at(0);

	}
	else
	{
		return -1;
	}
	char jid_chatid[64] = { 0 };
	sprintf(jid_chatid, "%s%d", jid.c_str(), sessionid);
	if (isexitgrougmember(jid_chatid))
	{

		return -1;
	}

	oZWAGROUPMEMBER.ZCHATSESSION = sessionid;
	mgroupmemberindex++;
	oZWAGROUPMEMBER.Z_PK = mgroupmemberindex;
	mgroupmemberopt++;
	oZWAGROUPMEMBER.Z_OPT = mgroupmemberopt;

	oZWAGROUPMEMBER.ZCONTACTIDENTIFIER = generate();
	oZWAGROUPMEMBER.ZMEMBERJID = jid;
	mgroupmembermap.insert(std::make_pair(jid_chatid, oZWAGROUPMEMBER));

	return oZWAGROUPMEMBER.Z_PK;

}

int android2ios::readmediafromiphone()
{
	if (!IsPathExist(mChatStoragedbfile.c_str()))
	{
		return 0;

	}
	wasqliteparsing osqliteparser(mChatStoragedbfile.c_str());
	char slqstr[1024];
	std::string	ZASPECTRATIO;//0.0
	int sqllen = sprintf(slqstr, "select Z_PK,Z_ENT,Z_OPT,ZFILESIZE ,ZMEDIAORIGIN ,ZMOVIEDURATION,ZMESSAGE,ZASPECTRATIO,ZHACCURACY,ZLATITUDE,ZLONGITUDE,ZAUTHORNAME,ZMEDIALOCALPATH,ZMEDIAURL,ZTITLE,ZVCARDNAME,ZVCARDSTRING,ZXMPPTHUMBPATH from ZWAMEDIAITEM");
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser.SqlQuery(slqstr, vmapstr))
	{
		for (int n = 0; n < vmapstr.size(); n++)
		{
			printf("n:%d\r\n", n);
			ZWAMEDIAITEM *oZWAMEDIAITEM=new ZWAMEDIAITEM;

			oZWAMEDIAITEM->Z_PK = atoi(FindDataFromMap(vmapstr.at(n), "Z_PK").c_str());
			oZWAMEDIAITEM->Z_OPT = atoi(FindDataFromMap(vmapstr.at(n), "Z_OPT").c_str());
			oZWAMEDIAITEM->ZFILESIZE = atoi(FindDataFromMap(vmapstr.at(n), "ZFILESIZE").c_str());
			oZWAMEDIAITEM->ZMEDIAORIGIN = atoi(FindDataFromMap(vmapstr.at(n), "ZMEDIAORIGIN").c_str());
			oZWAMEDIAITEM->ZMOVIEDURATION = atoi(FindDataFromMap(vmapstr.at(n), "ZMOVIEDURATION").c_str());
			oZWAMEDIAITEM->ZMESSAGE = atoi(FindDataFromMap(vmapstr.at(n), "ZMESSAGE").c_str());
			oZWAMEDIAITEM->ZASPECTRATIO = FindDataFromMap(vmapstr.at(n), "ZASPECTRATIO");
			oZWAMEDIAITEM->ZHACCURACY = FindDataFromMap(vmapstr.at(n), "ZHACCURACY");
			oZWAMEDIAITEM->ZLATITUDE = FindDataFromMap(vmapstr.at(n), "ZLATITUDE");
			oZWAMEDIAITEM->ZLONGITUDE = FindDataFromMap(vmapstr.at(n), "ZLONGITUDE");
			oZWAMEDIAITEM->ZAUTHORNAME = FindDataFromMap(vmapstr.at(n), "ZAUTHORNAME");
			oZWAMEDIAITEM->ZMEDIALOCALPATH = FindDataFromMap(vmapstr.at(n), "ZMEDIALOCALPATH");
			oZWAMEDIAITEM->ZMEDIAURL = FindDataFromMap(vmapstr.at(n), "ZMEDIAURL");
			oZWAMEDIAITEM->ZTITLE = FindDataFromMap(vmapstr.at(n), "ZTITLE");
			oZWAMEDIAITEM->ZVCARDNAME = FindDataFromMap(vmapstr.at(n), "ZVCARDNAME");
			oZWAMEDIAITEM->ZVCARDSTRING = FindDataFromMap(vmapstr.at(n), "ZVCARDSTRING");
			oZWAMEDIAITEM->ZXMPPTHUMBPATH = FindDataFromMap(vmapstr.at(n), "ZXMPPTHUMBPATH");

			mmediazpk = oZWAMEDIAITEM->Z_PK > mmediazpk ? oZWAMEDIAITEM->Z_PK : mmediazpk;


			mwamediamap.insert(std::make_pair(oZWAMEDIAITEM->Z_PK, oZWAMEDIAITEM));
		}
	}
	return 0;


}
int android2ios::readmessagechatfromiphone()
{
	if (!IsPathExist(mChatStoragedbfile.c_str()))
	{
		return 0;

	}

	wasqliteparsing osqliteparser(mChatStoragedbfile.c_str());
	char slqstr[1024];
	std::string	ZASPECTRATIO;//0.0
	int sqllen = sprintf(slqstr, "select Z_PK,Z_ENT,Z_OPT,ZDOCID,ZFILTEREDRECIPIENTCOUNT,ZFLAGS,ZGROUPEVENTTYPE,ZISFROMME,ZMESSAGESTATUS,ZMESSAGETYPE,ZSORT,ZSTARRED,ZCHATSESSION,ZGROUPMEMBER,ZMEDIAITEM,ZMESSAGEDATE,ZFROMJID ,ZMEDIASECTIONID ,ZSTANZAID,ZTEXT,ZTOJID from ZWAMESSAGE");
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser.SqlQuery(slqstr, vmapstr))
	{
		for (int n = 0; n < vmapstr.size(); n++)
		{
			
			ZWAMESSAGE* oZWAMESSAGE = new ZWAMESSAGE;

			oZWAMESSAGE->Z_PK = atoi(FindDataFromMap(vmapstr.at(n), "Z_PK").c_str());
			oZWAMESSAGE->Z_OPT = atoi(FindDataFromMap(vmapstr.at(n), "Z_OPT").c_str());
			oZWAMESSAGE->Z_ENT = atoi(FindDataFromMap(vmapstr.at(n), "Z_ENT").c_str());

			oZWAMESSAGE->ZDOCID = atoi(FindDataFromMap(vmapstr.at(n), "ZDOCID").c_str());
			oZWAMESSAGE->ZFILTEREDRECIPIENTCOUNT = atoi(FindDataFromMap(vmapstr.at(n), "ZFILTEREDRECIPIENTCOUNT").c_str());

			oZWAMESSAGE->ZFLAGS = atoi(FindDataFromMap(vmapstr.at(n), "ZFLAGS").c_str());
			oZWAMESSAGE->ZGROUPEVENTTYPE = atoi(FindDataFromMap(vmapstr.at(n), "ZGROUPEVENTTYPE").c_str());
			oZWAMESSAGE->ZISFROMME = atoi(FindDataFromMap(vmapstr.at(n), "ZISFROMME").c_str());
			oZWAMESSAGE->ZMESSAGESTATUS = atoi(FindDataFromMap(vmapstr.at(n), "ZMESSAGESTATUS").c_str());
			oZWAMESSAGE->ZMESSAGETYPE = atoi(FindDataFromMap(vmapstr.at(n), "ZMESSAGETYPE").c_str());
			oZWAMESSAGE->ZSORT = atoi(FindDataFromMap(vmapstr.at(n), "ZSORT").c_str());


			oZWAMESSAGE->ZSTARRED = atoi(FindDataFromMap(vmapstr.at(n), "ZSTARRED").c_str());
			oZWAMESSAGE->ZCHATSESSION = atoi(FindDataFromMap(vmapstr.at(n), "ZCHATSESSION").c_str());

			oZWAMESSAGE->ZGROUPMEMBER = FindDataFromMap(vmapstr.at(n), "ZGROUPMEMBER");
			oZWAMESSAGE->ZMEDIAITEM = FindDataFromMap(vmapstr.at(n), "ZMEDIAITEM");

			oZWAMESSAGE->ZMESSAGEDATE = FindDataFromMap(vmapstr.at(n), "ZMESSAGEDATE");

			oZWAMESSAGE->ZFROMJID = FindDataFromMap(vmapstr.at(n), "ZFROMJID");

			oZWAMESSAGE->ZMEDIASECTIONID = FindDataFromMap(vmapstr.at(n), "ZMEDIASECTIONID");

			oZWAMESSAGE->ZSTANZAID = FindDataFromMap(vmapstr.at(n), "ZSTANZAID");

			oZWAMESSAGE->ZTEXT = FindDataFromMap(vmapstr.at(n), "ZTEXT");

			oZWAMESSAGE->ZTOJID = FindDataFromMap(vmapstr.at(n), "ZTOJID");
			updatesort(oZWAMESSAGE->ZCHATSESSION, oZWAMESSAGE->ZSORT);

			mmessagezpk = oZWAMESSAGE->Z_PK > mmessagezpk ? oZWAMESSAGE->Z_PK : mmessagezpk;
			mmessageZDOCID = oZWAMESSAGE->ZDOCID > mmessageZDOCID ? oZWAMESSAGE->ZDOCID : mmessageZDOCID;
			//	mmessageZSORT = oZWAMESSAGE->ZSORT > mmessageZSORT ? oZWAMESSAGE->ZSORT : mmessageZSORT;
			mwamessagemap.insert(std::make_pair(oZWAMESSAGE->Z_PK, oZWAMESSAGE));

		}
	}
	return 0;


}

int android2ios::getmediadatailinfo(wasqliteparsing&osqliteparser,int message_id,std::string &filepath,std::string &width,std::string & height,std::string &media_name, std::string& file_size)
{

//	sqliteparsing osqliteparser(mmsgstoredbfile.c_str());
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "select media_job_uuid,transferred,file_path,file_size,width,height,media_name from message_media where message_row_id==%d", message_id);
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser.SqlQuery(slqstr, vmapstr))
	{
		if (vmapstr.size() == 1)
		{
			filepath = FindDataFromMap(vmapstr.at(0), "file_path");

			width = FindDataFromMap(vmapstr.at(0), "width");

			height = FindDataFromMap(vmapstr.at(0), "height");
			media_name = FindDataFromMap(vmapstr.at(0), "media_name");
			file_size= FindDataFromMap(vmapstr.at(0), "file_size");
			return 0;
		}
		else
		{

			return -1;
		}



	}

	return -1;


}
//
int android2ios::getmediadatailinfo_new(wasqliteparsing& osqliteparser, int message_id, std::string& filepath, std::string& width, std::string& height, std::string& media_name, std::string& file_size,std::string & media_duration,std::string &media_url,std::string &media_mime_type)
{

	//	sqliteparsing osqliteparser(mmsgstoredbfile.c_str());
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "select media_job_uuid,transferred,file_path,file_size,face_x,face_y,width,height,media_name,media_duration,message_url,mime_type from message_media where message_row_id==%d", message_id);
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser.SqlQuery(slqstr, vmapstr))
	{
		if (vmapstr.size() == 1)
		{
			filepath = FindDataFromMap(vmapstr.at(0), "file_path");
			media_duration = FindDataFromMap(vmapstr.at(0), "media_duration");
			width = FindDataFromMap(vmapstr.at(0), "width");
			height = FindDataFromMap(vmapstr.at(0), "height");
			media_url = FindDataFromMap(vmapstr.at(0), "message_url");
			media_mime_type = FindDataFromMap(vmapstr.at(0), "mime_type");

			std::string face_x = FindDataFromMap(vmapstr.at(0), "face_x");
			std::string  face_y = FindDataFromMap(vmapstr.at(0), "face_y");
			if (width == "0")
			{
				if (face_x != "-1")
				{
					width = face_x;
					height = face_y;

				}
			}

			media_name = FindDataFromMap(vmapstr.at(0), "media_name");
			file_size = FindDataFromMap(vmapstr.at(0), "file_size");
			return 0;
		}
		else
		{

			return -1;
		}



	}

	return -1;


}

int android2ios::getthumpicinfo(wasqliteparsing&osqliteparser,std::string keyid,std::string &thumdata)
{

	std::map<std::string, wablockbytes>::iterator itt;

	itt = mthumbmap.find(keyid);
	if (itt != mthumbmap.end())
	{
		thumdata =std::string( itt->second.pdata, itt->second.plen);
		return 0;
	}
	return -1;

}

char* android2ios::getthumpicinfo_B(std::string keyid, int &outlen)
{

	std::map<std::string, wablockbytes>::iterator itt;

	itt = mthumbmap.find(keyid);
	if (itt != mthumbmap.end())
	{
		if (itt->second.plen!=0)
		{
			outlen = itt->second.plen;
			char* outbuf = new char[itt->second.plen];
			memcpy(outbuf, itt->second.pdata, itt->second.plen);
			delete[]  itt->second.pdata;

			itt->second.plen = 0;
			return outbuf;
		}
}
	return 0;

}

int android2ios::getthumpicinfo_messagea( int message_rowid, std::string& thumdata)
{

	std::map<int, std::string>::iterator itt;

	itt = mthumbmap_a.find(message_rowid);
	if (itt != mthumbmap_a.end())
	{
		thumdata = itt->second;
		mthumbmap_a.erase(itt++);

		return 0;
	}
	return -1;

}


int android2ios::getstartindex()
{
	int retindex = -1;
	m_indexMutex.Lock();
	retindex = m_chatlimitindex;
	m_chatlimitindex += 1000;
	m_indexMutex.Unlock();

	return retindex;
}	

void android2ios::tostopthread()
{
	
	m_indexMutex.Lock();
	m_runthreadnumber--;
	m_indexMutex.Unlock();

}

int android2ios::dealmessagechatthread(int start)
{

	int msize = 0;
	wasqliteparsing osqliteparser(mmsgstoredbfile.c_str());
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "select _id,key_remote_jid,key_from_me,key_id,status,data,timestamp,media_wa_type,starred,remote_resource,received_timestamp,send_timestamp,media_url,media_mime_type,media_name,media_size,media_caption,media_hash,media_duration,origin,latitude,longitude,hex(raw_data),media_enc_hash from %s limit %d,1000", m_chattable.c_str(), start);
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser.SqlQuery(slqstr, vmapstr))
	{
		msize = vmapstr.size();

		for (int n = 0; n < vmapstr.size(); n++)
		{

			int m_id = atoi(FindDataFromMap(vmapstr.at(n), "_id").c_str());
			std::string media_wa_type = FindDataFromMap(vmapstr.at(n), "media_wa_type");
			if (media_wa_type.empty())
			{
				continue;
			}
			std::string key_remote_jid = FindDataFromMap(vmapstr.at(n), "key_remote_jid");
			if (key_remote_jid.empty() || key_remote_jid == "-1")
			{
				continue;
			}

			std::string statusstr = FindDataFromMap(vmapstr.at(n), "status");


			std::string key_id = FindDataFromMap(vmapstr.at(n), "key_id");
			if (key_id.empty())
			{
				continue;
			}

			std::string key_from_me = FindDataFromMap(vmapstr.at(n), "key_from_me");

			int type = atoi(media_wa_type.c_str());
			int status = atoi(statusstr.c_str());



			std::string ww_id = FindDataFromMap(vmapstr.at(n), "_id");
		
			if (status == 6)
			{

				continue;//
			}
			else {
				status = 0;
			}

			int itype = type;

			if (type == 0)
			{

				itype = 0;
		
			}
			else if (type == 1)
			{
			
				itype = 1;
			}
			else if (type == 2)
			{

				itype = 3;
			}
			else if (type == 3 || type == 13)
			{
		
				itype = 2;
			}
			else if (type == 4)
			{
			
				itype = 4;
			}

			else if (type == 5)
			{
			
				itype = 5;
			}

			else if (type == 9)
			{
				
				itype = 8;
			}

			else if (type == 10)
			{
				std::string tmpss = FindDataFromMap(vmapstr.at(n), "data");
				AWriteLog("android2ios", "error itype:%d  %s", itype, tmpss.c_str());
				continue;
			}

			else if (type == 20)
			{
		
				itype = 15;

			}
			else {
			
				continue;
			}


			//	
			int keyfromme = atoi(key_from_me.c_str());

			int sessionid = getsessionpk(key_remote_jid);
			int msort=0;
			bool isgroup;
			if (!getisgroupstatus(key_remote_jid,  isgroup))
			{
				continue;
			}

			std::string groupmemberjid;
			std::string greoupmember = "";
			if (isgroup)
			{

				if (keyfromme == 1)
				{
					groupmemberjid = mselfjid;
				}
				else
				{
					groupmemberjid = FindDataFromMap(vmapstr.at(n), "remote_resource");
					char jid_chatid[128] = { 0 };
					sprintf(jid_chatid, "%s%d", groupmemberjid.c_str(), sessionid);
					int gmemberid = getzpkgrougmember(jid_chatid);
					char gmjid[24] = { 0 };
					sprintf(gmjid, "%d", gmemberid);
					greoupmember = std::string(gmjid);
					if (greoupmember == "-1")
					{

						gmemberid = insertothergroumemberEx(groupmemberjid, sessionid);
						if (gmemberid == -1)
						{
							AWriteLog("android2ios", "code:10 groupmemberjid:%s sessionid:%d", groupmemberjid.c_str(), sessionid);
							continue;
						}
						else
						{
							sprintf(gmjid, "%d", gmemberid);
							greoupmember = std::string(gmjid);

						}
					}
				}

			}
			std::string creationstr = FindDataFromMap(vmapstr.at(n), "timestamp");
			if (creationstr.length() > 2 && creationstr[0] == '-')
			{
				continue;
			}
			std::string ZMEDIASECTIONIDstr=MGetIOSDateFromTimet(creationstr);
			if (itype == 8)
			{
				ZMEDIASECTIONIDstr = "doc-" + ZMEDIASECTIONIDstr;
			}
			std::string creattm = getiostm(creationstr.c_str());
			std::string starred = FindDataFromMap(vmapstr.at(n), "starred");


			mmessageZDOCID++;
			ZWAMESSAGE* oZWAMESSAGE = new ZWAMESSAGE;
			int myid = m_id + mmessagezpk;//
		//	int myid = mmessageZDOCID;//
			oZWAMESSAGE->Z_ID = myid;
			oZWAMESSAGE->ZMEDIASECTIONID = ZMEDIASECTIONIDstr;
			oZWAMESSAGE->Z_PK = myid;

			oZWAMESSAGE->key_remote_jid = key_remote_jid;
			oZWAMESSAGE->Z_OPT = 2;
			oZWAMESSAGE->Z_ENT = 9;
			if (starred == "1")
			{
				oZWAMESSAGE->ZSTARRED = 1;
			}
			oZWAMESSAGE->ZDOCID = mmessageZDOCID;
			oZWAMESSAGE->ZFILTEREDRECIPIENTCOUNT = 0;

			oZWAMESSAGE->ZFLAGS = 16777216;
		
			oZWAMESSAGE->ZISFROMME = keyfromme;
			if (keyfromme == 1)
			{
				oZWAMESSAGE->ZGROUPEVENTTYPE = 0;
			}
			else
			{
				oZWAMESSAGE->ZGROUPEVENTTYPE = 2;
			}

			oZWAMESSAGE->ZMESSAGESTATUS = 0;///??????

			oZWAMESSAGE->ZMESSAGETYPE = itype;

			oZWAMESSAGE->ZSORT = msort;

		//	oZWAMESSAGE->ZSTARRED = 0;
			oZWAMESSAGE->ZCHATSESSION = getsessionpk(key_remote_jid);




			oZWAMESSAGE->ZGROUPMEMBER = greoupmember;



			oZWAMESSAGE->ZMESSAGEDATE = creattm;
			if (keyfromme == 1)
			{
				oZWAMESSAGE->ZFROMJID = "";
				oZWAMESSAGE->ZTOJID = key_remote_jid;
			}
			else
			{
				oZWAMESSAGE->ZFROMJID = key_remote_jid;
				oZWAMESSAGE->ZTOJID = "";

			}
			oZWAMESSAGE->ZSTANZAID = key_id;
			std::string hasZMEDIAITEMindex = "";;
			std::string datatxt = "";
			if (itype == 0)
			{
				datatxt = FindDataFromMap(vmapstr.at(n), "data");
				oZWAMESSAGE->ZTEXT = datatxt;

			}
			else
			{
				if (itype == 8)
				{

					oZWAMESSAGE->ZTEXT = FindDataFromMap(vmapstr.at(n), "media_caption");;
				}

				std::string media_size = FindDataFromMap(vmapstr.at(n), "media_size");


				ZWAMEDIAITEM* oZWAMEDIAITEM = new ZWAMEDIAITEM;
				//	oZWAMEDIAITEM->Z_PK;
				oZWAMEDIAITEM->Z_ENT = 8;//8
				oZWAMEDIAITEM->Z_OPT = 4;//4
			//	std::string	ZCLOUDSTATUS;//""
				oZWAMEDIAITEM->ZFILESIZE = media_size;//
				if (oZWAMEDIAITEM->ZFILESIZE == ""|| oZWAMEDIAITEM->ZFILESIZE == " ")
				{
					oZWAMEDIAITEM->ZFILESIZE = "0";
				}

				oZWAMEDIAITEM->ZMEDIAORIGIN = atoi(FindDataFromMap(vmapstr.at(n), "origin").c_str());
				oZWAMEDIAITEM->ZMOVIEDURATION = atoi(FindDataFromMap(vmapstr.at(n), "media_duration").c_str());
				oZWAMEDIAITEM->ZMESSAGE = myid;
				oZWAMEDIAITEM->ZASPECTRATIO = "0.0";//0.0
				oZWAMEDIAITEM->ZHACCURACY = "0.0";//0.0
				oZWAMEDIAITEM->ZLATITUDE = FindDataFromMap(vmapstr.at(n), "latitude");//0.0
				oZWAMEDIAITEM->ZLONGITUDE = FindDataFromMap(vmapstr.at(n), "longitude");//0.0
			
				oZWAMEDIAITEM->ZMEDIAURLDATE = creattm;//""
				
				oZWAMEDIAITEM->ZAUTHORNAME = FindDataFromMap(vmapstr.at(n), "media_caption");//
			//	std::string	ZCOLLECTIONNAME;//""
				if (itype == 4 || itype == 5)
				{

					oZWAMEDIAITEM->ZMEDIALOCALPATH = "";
					oZWAMESSAGE->ZMEDIASECTIONID = "";

				}
				else
				{

					std::string iphonelocalpath = "/Media/";
					iphonelocalpath += key_remote_jid;
					std::string filepath = "";
					std::string width = "0.0";
					std::string height = "0.0";
					std::string media_name;
					std::string f_size;
					int reti = getmediadatailinfo(osqliteparser, m_id, filepath, width, height, media_name, f_size);


					if (itype != 0)
					{

						if (itype == 15&& height=="0")
						{
							height = "64";
							width = "64";
						}

						oZWAMEDIAITEM->ZLATITUDE = height;//0.0
						oZWAMEDIAITEM->ZLONGITUDE = width;//0.0

						if (!ComParRight(oZWAMEDIAITEM->ZLATITUDE, ".0"))
						{
							oZWAMEDIAITEM->ZLATITUDE += ".0";
						}
						if (!ComParRight(oZWAMEDIAITEM->ZLONGITUDE, ".0"))
						{
							oZWAMEDIAITEM->ZLONGITUDE += ".0";
						}
					}

					if (!filepath.empty())
					{
#ifdef WIN32
						Replace(filepath, "/", "\\");
#endif

						std::string localfilename = androidmainpath;
						localfilename += filepath;
				
#ifdef WIN32
					/*	if (IsPathExist(localfilename.c_str())==false)
						{
							char*  dttfilename= utf8Format((char*) localfilename.c_str(),localfilename.length());
							if (dttfilename)
							{
								

								localfilename = std::string(dttfilename);

								free(dttfilename);
							}

						}*/
#endif
						if (IsPathExist(localfilename.c_str()))
						{
	
							oZWAMEDIAITEM->attlocalfilename = localfilename;
							std::string fileuuid = generate();
							if (fileuuid.empty())
							{

								fileuuid = GetMd5c((unsigned char*)key_id.c_str(), key_id.length());
							}
							if (fileuuid.empty())
							{
								fileuuid = "4241552525266523652365236523625";
							}

							std::string fixx = GetSuffix(media_name.c_str());

							if (fixx.empty())
							{
								fixx = GetSuffix(localfilename.c_str());
							}

							//std::string tmp
							std::string fpath = fileuuid.substr(0, 1);
							std::string tpath = fileuuid.substr(1, 1);
							iphonelocalpath += "/";

							iphonelocalpath += fpath;
							iphonelocalpath += "/";
							iphonelocalpath += tpath;
							iphonelocalpath += "/";
							iphonelocalpath += fileuuid;
							iphonelocalpath += ".";
							iphonelocalpath += fixx;

							oZWAMEDIAITEM->iphonelocalpath = iphonelocalpath;

							oZWAMEDIAITEM->ZMEDIALOCALPATH = iphonelocalpath.substr(1, iphonelocalpath.length()-1);


							oZWAMEDIAITEM->manifest_relativePath = "Message";
							oZWAMEDIAITEM->manifest_relativePath += iphonelocalpath;


							std::string sha1hashstr = "";
							if (m_watype == 0)
							{
								 sha1hashstr = "AppDomainGroup-group.net.whatsapp.WhatsApp.shared-";
							}
							else {
								 sha1hashstr = "AppDomainGroup-group.net.whatsapp.WhatsAppSMB.shared-";
							}
				
							sha1hashstr += oZWAMEDIAITEM->manifest_relativePath;

							oZWAMEDIAITEM->fileID = GetSha1Str((char*)sha1hashstr.c_str(), sha1hashstr.length());

							oZWAMEDIAITEM->backupfilename = mrestorepath;
							std::string tmpfileid = std::string(oZWAMEDIAITEM->fileID);
							fpath = tmpfileid.substr(0, 2);
#ifdef WIN32
							oZWAMEDIAITEM->backupfilename += mideviceid;
							oZWAMEDIAITEM->backupfilename += "\\";

							oZWAMEDIAITEM->backupfilename += fpath;
							oZWAMEDIAITEM->backupfilename += "\\";
#else
							oZWAMEDIAITEM->backupfilename += mideviceid;
							oZWAMEDIAITEM->backupfilename += "/";

							oZWAMEDIAITEM->backupfilename += fpath;
							oZWAMEDIAITEM->backupfilename += "/";
#endif
							CreatePath(oZWAMEDIAITEM->backupfilename.c_str());
							oZWAMEDIAITEM->backupfilename += tmpfileid;
							//20210926 
							copyfile(oZWAMEDIAITEM->attlocalfilename.c_str(), oZWAMEDIAITEM->backupfilename.c_str());

						}
						else
						{
							AWriteLog("android2ios", "real file is miss:%s", localfilename.c_str());
						}
					}
				}
				oZWAMEDIAITEM->ZMEDIAURL = FindDataFromMap(vmapstr.at(n), "media_url"); ;//
				oZWAMEDIAITEM->ZTITLE = FindDataFromMap(vmapstr.at(n), "media_caption");;
				if (itype == 4|| itype == 5)
				{
					oZWAMEDIAITEM->ZVCARDNAME = FindDataFromMap(vmapstr.at(n), "media_name");

				}
				else
				{
					oZWAMEDIAITEM->ZVCARDNAME = FindDataFromMap(vmapstr.at(n), "media_hash");
				}

				if (itype == 4)
				{
					oZWAMEDIAITEM->ZVCARDSTRING = FindDataFromMap(vmapstr.at(n), "data");
				}
				else
				{
					oZWAMEDIAITEM->ZVCARDSTRING = FindDataFromMap(vmapstr.at(n), "media_mime_type");
				}

				//
				//std::string thumdata = FindDataFromMap(vmapstr.at(n), "raw_data"); hex(raw_data)

				char* thumdatabuf = NULL;
				int outlen = 0;
					std::string thumdata = FindDataFromMap(vmapstr.at(n), "hex(raw_data)");
				if (thumdata.empty())
				{
				//	getthumpicinfo(osqliteparser, key_id, thumdata);

					thumdatabuf = getthumpicinfo_B(key_id, outlen);
				}
				else
				{
		
					outlen = hexs2bin(thumdata.c_str(), &thumdatabuf);
				}
				if (thumdatabuf)
				{
					std::string thumfileuuid = generate();
					if (!thumfileuuid.empty())
					{

						std::string thlocalfilename = mpermediapath + thumfileuuid;
						
						if (outlen != 0)
						{


							//if (WriteToFile(thlocalfilename.c_str(), out, thlen))
							{

								oZWAMEDIAITEM->thumlocalfilename = thlocalfilename;
								std::string thum_iphonelocalpath = "/Media/";
								thum_iphonelocalpath += key_remote_jid;
								std::string fpath = thumfileuuid.substr(0, 1);
								std::string tpath = thumfileuuid.substr(1, 1);
								thum_iphonelocalpath += "/";

								thum_iphonelocalpath += fpath;
								thum_iphonelocalpath += "/";
								thum_iphonelocalpath += tpath;
								thum_iphonelocalpath += "/";
								thum_iphonelocalpath += thumfileuuid;
								thum_iphonelocalpath += ".thumb";

								oZWAMEDIAITEM->thum_iphonelocalpath = thum_iphonelocalpath;
							//	oZWAMEDIAITEM->ZXMPPTHUMBPATH = thum_iphonelocalpath;


								oZWAMEDIAITEM->ZXMPPTHUMBPATH = thum_iphonelocalpath.substr(1, thum_iphonelocalpath.length() - 1);



								oZWAMEDIAITEM->thum_manifest_relativePath = "Message";
								oZWAMEDIAITEM->thum_manifest_relativePath += thum_iphonelocalpath;
								std::string thum_sha1hashstr = "";
								if (m_watype == 0)
								{
									thum_sha1hashstr = "AppDomainGroup-group.net.whatsapp.WhatsApp.shared-";
								}
								else {

									thum_sha1hashstr = "AppDomainGroup-group.net.whatsapp.WhatsAppSMB.shared-";
								}
		
								thum_sha1hashstr += oZWAMEDIAITEM->thum_manifest_relativePath;

								oZWAMEDIAITEM->thum_fileID = GetSha1Str((char*)thum_sha1hashstr.c_str(), thum_sha1hashstr.length());



								oZWAMEDIAITEM->thum_backupfilename = mrestorepath;
								std::string tmpfileid = std::string(oZWAMEDIAITEM->thum_fileID);
								fpath = tmpfileid.substr(0, 2);
#ifdef WIN32
								oZWAMEDIAITEM->thum_backupfilename += mideviceid;
								oZWAMEDIAITEM->thum_backupfilename += "\\";
								oZWAMEDIAITEM->thum_backupfilename += fpath;
								oZWAMEDIAITEM->thum_backupfilename += "\\";
#else
								oZWAMEDIAITEM->thum_backupfilename += mideviceid;
								oZWAMEDIAITEM->thum_backupfilename += "/";
								oZWAMEDIAITEM->thum_backupfilename += fpath;
								oZWAMEDIAITEM->thum_backupfilename += "/";
#endif
								CreatePath(oZWAMEDIAITEM->thum_backupfilename.c_str());

								oZWAMEDIAITEM->thum_backupfilename += tmpfileid;

								//20210926
								WriteToFile(oZWAMEDIAITEM->thum_backupfilename.c_str(), thumdatabuf, outlen);
								//copyfile(oZWAMEDIAITEM->thumlocalfilename.c_str(), oZWAMEDIAITEM->thum_backupfilename.c_str());


							}
							delete[] thumdatabuf;
						}

					}
				}

				oZWAMEDIAITEM->fandroid = 1;
				m_mediaindexMutex.Lock();
				oZWAMEDIAITEM->Z_PK = ++mmediazpk;
				mwamediamap.insert(std::make_pair(oZWAMEDIAITEM->Z_PK, oZWAMEDIAITEM));
				m_mediaindexMutex.Unlock();


				char buf[24] = { 0 };
				sprintf(buf, "%d", oZWAMEDIAITEM->Z_PK);
				hasZMEDIAITEMindex = std::string(buf);

			}
			oZWAMESSAGE->ZMEDIAITEM = hasZMEDIAITEMindex;

			m_chatsessionMutex.Lock();
			updatechat(sessionid, atoi(creattm.c_str()), myid);

			mwamessagemap.insert(std::make_pair(oZWAMESSAGE->Z_ID, oZWAMESSAGE));
			m_chatsessionMutex.Unlock();
		}
	}
	vmapstr.clear();


	if (msize > 0)
	{
		return msize;

	}
	return 0;
}

int android2ios::dealmessagechatthread_newmessage(int start)
{

	int msize = 0;
	wasqliteparsing osqliteparser(mmsgstoredbfile.c_str());
	char slqstr[1024];
	int sqllen = sprintf(slqstr, "select _id,chat_row_id,from_me,key_id,status,text_data,timestamp,message_type,starred,sender_jid_row_id,received_timestamp from %s limit %d,1000", m_chattable.c_str(), start);
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;
	if (osqliteparser.SqlQuery(slqstr, vmapstr))
	{
		msize = vmapstr.size();

		for (int n = 0; n < vmapstr.size(); n++)
		{
			
			int m_id = atoi(FindDataFromMap(vmapstr.at(n), "_id").c_str());
			std::string media_wa_type = FindDataFromMap(vmapstr.at(n), "message_type");
			if (media_wa_type.empty())
			{
				continue;
			}
			std::string chat_row_id = FindDataFromMap(vmapstr.at(n), "chat_row_id");
			if (chat_row_id.empty() || chat_row_id == "-1")
			{
				continue;
			}
			std::string key_remote_jid=getchatromte_row_jid(chat_row_id.c_str());
			if (key_remote_jid == "")
			{
				continue;
			}
			std::string statusstr = FindDataFromMap(vmapstr.at(n), "status");
		

			std::string key_id = FindDataFromMap(vmapstr.at(n), "key_id");
			if (key_id.empty())
			{
				continue;
			}

			std::string key_from_me = FindDataFromMap(vmapstr.at(n), "from_me");

			//	std::string data = FindDataFromMap(vmapstr.at(n), "data");

			int type = atoi(media_wa_type.c_str());
			int status = atoi(statusstr.c_str());

			if (status == 6)
			{

				continue;
			}
			else {
				status = 0;
			}

			int itype = type;

			if (type == 0)
			{

				itype = 0;
				
			}
			else if (type == 1)
			{

				itype = 1;
			}
			else if (type == 2)
			{
								/// <summary>

				itype = 3;
			}
			else if (type == 3 || type == 13)
			{
		
				itype = 2;
			}
			else if (type == 4)
			{
		
				itype = 4;
			}

			else if (type == 5)
			{
		
				itype = 5;
			}

			else if (type == 9)
			{
		
				itype = 8;
			}

			else if (type == 10)
			{
				//crypt warning
				std::string tmpss = FindDataFromMap(vmapstr.at(n), "text_data");
				AWriteLog("android2ios", "error itype:%d  %s", itype, tmpss.c_str());
				continue;
			}

			else if (type == 20)
			{
				itype = 15;
				//continue;
			}
			else {

				continue;
			}


			//	
			int keyfromme = atoi(key_from_me.c_str());

			int sessionid = getsessionpk(key_remote_jid);
			int msort = 0;
			bool isgroup;
			if (!getisgroupstatus(key_remote_jid, isgroup))
			{
				continue;
			}

			std::string groupmemberjid;
			std::string greoupmember = "";
			if (isgroup)
			{

				if (keyfromme == 1)
				{
					groupmemberjid = mselfjid;
				}
				else
				{
					std::string sender_jid_row_id = FindDataFromMap(vmapstr.at(n), "sender_jid_row_id");

					groupmemberjid=getjid_romte_row_jid(sender_jid_row_id.c_str());

					char jid_chatid[128] = { 0 };
					sprintf(jid_chatid, "%s%d", groupmemberjid.c_str(), sessionid);
					int gmemberid = getzpkgrougmember(jid_chatid);
					char gmjid[24] = { 0 };
					sprintf(gmjid, "%d", gmemberid);
					greoupmember = std::string(gmjid);
					if (greoupmember == "-1")
					{
						gmemberid = insertothergroumemberEx(groupmemberjid, sessionid);
						if (gmemberid == -1)
						{

							continue;
						}
						else
						{
							sprintf(gmjid, "%d", gmemberid);
							greoupmember = std::string(gmjid);

						}

					}
				}

			}
			std::string creationstr = FindDataFromMap(vmapstr.at(n), "timestamp");
			if (creationstr.length() > 2 && creationstr[0] == '-')
			{
				continue;
			}
			std::string ZMEDIASECTIONIDstr = MGetIOSDateFromTimet(creationstr);
			if (itype == 8)
			{
				ZMEDIASECTIONIDstr = "doc-" + ZMEDIASECTIONIDstr;
			}
			std::string creattm = getiostm(creationstr.c_str());
			std::string starred = FindDataFromMap(vmapstr.at(n), "starred");
			mmessageZDOCID++;
			ZWAMESSAGE* oZWAMESSAGE = new ZWAMESSAGE;
			int myid = m_id + mmessagezpk;//
		//	int myid = mmessageZDOCID;//
			oZWAMESSAGE->Z_ID = myid;
			oZWAMESSAGE->ZMEDIASECTIONID = ZMEDIASECTIONIDstr;
			oZWAMESSAGE->Z_PK = myid;

			oZWAMESSAGE->key_remote_jid = key_remote_jid;
			oZWAMESSAGE->Z_OPT = 2;
			oZWAMESSAGE->Z_ENT = 9;
			if (starred == "1")
			{
				oZWAMESSAGE->ZSTARRED = 1;
			}
			oZWAMESSAGE->ZDOCID = mmessageZDOCID;
			oZWAMESSAGE->ZFILTEREDRECIPIENTCOUNT = 0;

			oZWAMESSAGE->ZFLAGS = 16777216;

			oZWAMESSAGE->ZISFROMME = keyfromme;
			if (keyfromme == 1)
			{
				oZWAMESSAGE->ZGROUPEVENTTYPE = 0;
			}
			else
			{
				oZWAMESSAGE->ZGROUPEVENTTYPE = 2;
			}

			oZWAMESSAGE->ZMESSAGESTATUS = 0;///??????

			oZWAMESSAGE->ZMESSAGETYPE = itype;

			oZWAMESSAGE->ZSORT = msort;

		//	oZWAMESSAGE->ZSTARRED = 0;
			oZWAMESSAGE->ZCHATSESSION = getsessionpk(key_remote_jid);




			oZWAMESSAGE->ZGROUPMEMBER = greoupmember;



			oZWAMESSAGE->ZMESSAGEDATE = creattm;
			if (keyfromme == 1)
			{
				oZWAMESSAGE->ZFROMJID = "";
				oZWAMESSAGE->ZTOJID = key_remote_jid;
			}
			else
			{
				oZWAMESSAGE->ZFROMJID = key_remote_jid;
				oZWAMESSAGE->ZTOJID = "";

			}
			oZWAMESSAGE->ZSTANZAID = key_id;
			std::string hasZMEDIAITEMindex = "";;
			std::string datatxt = "";
			datatxt = FindDataFromMap(vmapstr.at(n), "text_data");
			if (itype == 0)
			{
	
				oZWAMESSAGE->ZTEXT = datatxt;
			}
			else
			{
				if (itype == 8)
				{

					oZWAMESSAGE->ZTEXT = FindDataFromMap(vmapstr.at(n), "text_data");
				}

				ZWAMEDIAITEM* oZWAMEDIAITEM = new ZWAMEDIAITEM;
				//	oZWAMEDIAITEM->Z_PK;
				oZWAMEDIAITEM->Z_ENT = 8;//8
				oZWAMEDIAITEM->Z_OPT = 4;//4
			//	std::string	ZCLOUDSTATUS;//""
	

			//	oZWAMEDIAITEM->ZMEDIAORIGIN = atoi(FindDataFromMap(vmapstr.at(n), "origin").c_str());
	
				oZWAMEDIAITEM->ZMESSAGE = myid;
				oZWAMEDIAITEM->ZASPECTRATIO = "0.0";//0.0
				oZWAMEDIAITEM->ZHACCURACY = "0.0";//0.0
			//	oZWAMEDIAITEM->ZLATITUDE = FindDataFromMap(vmapstr.at(n), "latitude");//0.0
			//	oZWAMEDIAITEM->ZLONGITUDE = FindDataFromMap(vmapstr.at(n), "longitude");//0.0
				oZWAMEDIAITEM->ZMEDIAURLDATE = creattm;//""

			//	oZWAMEDIAITEM->ZAUTHORNAME = FindDataFromMap(vmapstr.at(n), "media_caption");//
			//	std::string	ZCOLLECTIONNAME;//""
				if (itype == 4 || itype == 5)
				{

					oZWAMEDIAITEM->ZMEDIALOCALPATH = "";
					oZWAMESSAGE->ZMEDIASECTIONID = "";

				}
				else
				{


					std::string iphonelocalpath = "/Media/";
					iphonelocalpath += key_remote_jid;
					std::string filepath = "";
					std::string width = "0.0";
					std::string height = "0.0";
					std::string media_name;
					std::string file_size;
					std::string media_duration;
					std::string media_url;
					std::string media_mime_type;
					int reti = getmediadatailinfo_new(osqliteparser, m_id, filepath, width, height, media_name, file_size, media_duration, media_url, media_mime_type);

					if (reti == -1)
					{
						delete oZWAMESSAGE;
						delete oZWAMEDIAITEM;
						continue;
					}
					oZWAMEDIAITEM->ZMOVIEDURATION = atoi(media_duration.c_str());
					oZWAMEDIAITEM->ZFILESIZE = file_size;//
					oZWAMEDIAITEM->ZMEDIAURL = media_url; ;//
					if (oZWAMEDIAITEM->ZFILESIZE == "" || oZWAMEDIAITEM->ZFILESIZE == " ")
					{
						oZWAMEDIAITEM->ZFILESIZE = "0";
					}

					if (itype == 4)
					{
						oZWAMEDIAITEM->ZVCARDSTRING = FindDataFromMap(vmapstr.at(n), "text_data");
					}
					else
					{
						oZWAMEDIAITEM->ZVCARDSTRING = media_mime_type;
					}

					oZWAMEDIAITEM->ZTITLE = datatxt;
					if (itype == 4 || itype == 5)
					{
						oZWAMEDIAITEM->ZVCARDNAME = media_name;

					}
					else
					{
						oZWAMEDIAITEM->ZVCARDNAME = FindDataFromMap(vmapstr.at(n), "text_data");
					}






					if (itype == 1 || itype == 2)
					{
					

						oZWAMEDIAITEM->ZLATITUDE = height;//0.0
						oZWAMEDIAITEM->ZLONGITUDE = width;//0.0

						if (!ComParRight(oZWAMEDIAITEM->ZLATITUDE, ".0"))
						{
							oZWAMEDIAITEM->ZLATITUDE += ".0";
						}
						if (!ComParRight(oZWAMEDIAITEM->ZLONGITUDE, ".0"))
						{
							oZWAMEDIAITEM->ZLONGITUDE += ".0";
						}
					}

					if (!filepath.empty())
					{

#ifdef WIN32
						Replace(filepath, "/", "\\");
#endif

						if (filepath[filepath.length() - 1] == '.')
						{

							filepath=filepath.substr(0, filepath.length() - 1);
						}
	

						std::string localfilename = androidmainpath;
						localfilename += filepath;


	



#ifdef WIN32
						if (IsPathExist(localfilename.c_str()) == false)
						{
							char* dttfilename = utf8Format((char*)localfilename.c_str(), localfilename.length());
							if (dttfilename)
							{


								localfilename = std::string(dttfilename);

								free(dttfilename);
							}

						}
#endif
						if (IsPathExist(localfilename.c_str()))
						{

							oZWAMEDIAITEM->attlocalfilename = localfilename;
							std::string fileuuid = generate();
							if (fileuuid.empty())
							{

								fileuuid = GetMd5c((unsigned char*)key_id.c_str(), key_id.length());
							}
							if (fileuuid.empty())
							{
								fileuuid = "4241552525266523652365236523625";
							}

							std::string fixx = GetSuffix(media_name.c_str());

							if (fixx.empty())
							{
								fixx = GetSuffix(localfilename.c_str());
							}

							//std::string tmp
							std::string fpath = fileuuid.substr(0, 1);
							std::string tpath = fileuuid.substr(1, 1);
							iphonelocalpath += "/";

							iphonelocalpath += fpath;
							iphonelocalpath += "/";
							iphonelocalpath += tpath;
							iphonelocalpath += "/";
							iphonelocalpath += fileuuid;
							iphonelocalpath += ".";
							iphonelocalpath += fixx;

							oZWAMEDIAITEM->iphonelocalpath = iphonelocalpath;

							//oZWAMEDIAITEM->ZMEDIALOCALPATH = iphonelocalpath;
							oZWAMEDIAITEM->ZMEDIALOCALPATH = iphonelocalpath.substr(1, iphonelocalpath.length() - 1);


							oZWAMEDIAITEM->manifest_relativePath = "Message";
							oZWAMEDIAITEM->manifest_relativePath += iphonelocalpath;

							std::string sha1hashstr = "";
							if (m_watype == 0)
							{
								sha1hashstr = "AppDomainGroup-group.net.whatsapp.WhatsApp.shared-";
							}
							else {
								sha1hashstr = "AppDomainGroup-group.net.whatsapp.WhatsAppSMB.shared-";
							}

	
							sha1hashstr += oZWAMEDIAITEM->manifest_relativePath;

							oZWAMEDIAITEM->fileID = GetSha1Str((char*)sha1hashstr.c_str(), sha1hashstr.length());

							oZWAMEDIAITEM->backupfilename = mrestorepath;
							std::string tmpfileid = std::string(oZWAMEDIAITEM->fileID);
							fpath = tmpfileid.substr(0, 2);
#ifdef WIN32
							oZWAMEDIAITEM->backupfilename += mideviceid;
							oZWAMEDIAITEM->backupfilename += "\\";

							oZWAMEDIAITEM->backupfilename += fpath;
							oZWAMEDIAITEM->backupfilename += "\\";
#else
							oZWAMEDIAITEM->backupfilename += mideviceid;
							oZWAMEDIAITEM->backupfilename += "/";

							oZWAMEDIAITEM->backupfilename += fpath;
							oZWAMEDIAITEM->backupfilename += "/";
#endif
							CreatePath(oZWAMEDIAITEM->backupfilename.c_str());
							oZWAMEDIAITEM->backupfilename += tmpfileid;
							//20210926 
							copyfile(oZWAMEDIAITEM->attlocalfilename.c_str(), oZWAMEDIAITEM->backupfilename.c_str());

						}
						else
						{
							AWriteLog("android2ios", "real file is miss:%s", localfilename.c_str());


						}
					}
				}

				std::string thumdata = "";
				
		
				getthumpicinfo(osqliteparser, key_id, thumdata);
				if (thumdata=="")
				{
					getthumpicinfo_messagea(m_id, thumdata);
				}
				if (!thumdata.empty())
				{
					std::string thumfileuuid = generate();
					if (!thumfileuuid.empty())
					{

						std::string thlocalfilename = mpermediapath + thumfileuuid;

						char* out;
						size_t thlen = hexs2bin(thumdata.c_str(), &out);
						if (thlen != 0)
						{


							//if (WriteToFile(thlocalfilename.c_str(), out, thlen))
							{

								oZWAMEDIAITEM->thumlocalfilename = thlocalfilename;
								std::string thum_iphonelocalpath = "/Media/";
								thum_iphonelocalpath += key_remote_jid;
								std::string fpath = thumfileuuid.substr(0, 1);
								std::string tpath = thumfileuuid.substr(1, 1);
								thum_iphonelocalpath += "/";

								thum_iphonelocalpath += fpath;
								thum_iphonelocalpath += "/";
								thum_iphonelocalpath += tpath;
								thum_iphonelocalpath += "/";
								thum_iphonelocalpath += thumfileuuid;
								thum_iphonelocalpath += ".thumb";

								oZWAMEDIAITEM->thum_iphonelocalpath = thum_iphonelocalpath;
								//	oZWAMEDIAITEM->ZXMPPTHUMBPATH = thum_iphonelocalpath;


								oZWAMEDIAITEM->ZXMPPTHUMBPATH = thum_iphonelocalpath.substr(1, thum_iphonelocalpath.length() - 1);



								oZWAMEDIAITEM->thum_manifest_relativePath = "Message";
								oZWAMEDIAITEM->thum_manifest_relativePath += thum_iphonelocalpath;
								std::string thum_sha1hashstr = "";
								if (m_watype == 0)
								{
									thum_sha1hashstr = "AppDomainGroup-group.net.whatsapp.WhatsApp.shared-";
								}
								else {
								    thum_sha1hashstr = "AppDomainGroup-group.net.whatsapp.WhatsAppSMB.shared-";
								}

							
								thum_sha1hashstr += oZWAMEDIAITEM->thum_manifest_relativePath;

								oZWAMEDIAITEM->thum_fileID = GetSha1Str((char*)thum_sha1hashstr.c_str(), thum_sha1hashstr.length());



								oZWAMEDIAITEM->thum_backupfilename = mrestorepath;
								std::string tmpfileid = std::string(oZWAMEDIAITEM->thum_fileID);
								fpath = tmpfileid.substr(0, 2);
#ifdef WIN32
								oZWAMEDIAITEM->thum_backupfilename += mideviceid;
								oZWAMEDIAITEM->thum_backupfilename += "\\";
								oZWAMEDIAITEM->thum_backupfilename += fpath;
								oZWAMEDIAITEM->thum_backupfilename += "\\";
#else
								oZWAMEDIAITEM->thum_backupfilename += mideviceid;
								oZWAMEDIAITEM->thum_backupfilename += "/";
								oZWAMEDIAITEM->thum_backupfilename += fpath;
								oZWAMEDIAITEM->thum_backupfilename += "/";
#endif
								CreatePath(oZWAMEDIAITEM->thum_backupfilename.c_str());

								oZWAMEDIAITEM->thum_backupfilename += tmpfileid;

								//20210926
								WriteToFile(oZWAMEDIAITEM->thum_backupfilename.c_str(), out, thlen);
								//copyfile(oZWAMEDIAITEM->thumlocalfilename.c_str(), oZWAMEDIAITEM->thum_backupfilename.c_str());


							}
							delete[] out;
						}

					}
				}
				//mmediazpk = oZWAMEDIAITEM->Z_PK > mmediazpk ? oZWAMEDIAITEM->Z_PK : mmediazpk;
				oZWAMEDIAITEM->fandroid = 1;
				//mmediazpk++;



				m_mediaindexMutex.Lock();
				oZWAMEDIAITEM->Z_PK = ++mmediazpk;
				mwamediamap.insert(std::make_pair(oZWAMEDIAITEM->Z_PK, oZWAMEDIAITEM));
				m_mediaindexMutex.Unlock();


				char buf[24] = { 0 };
				sprintf(buf, "%d", oZWAMEDIAITEM->Z_PK);
				hasZMEDIAITEMindex = std::string(buf);

			}
			oZWAMESSAGE->ZMEDIAITEM = hasZMEDIAITEMindex;
			
			m_chatsessionMutex.Lock();
			updatechat(sessionid, atoi(creattm.c_str()), myid);

			mwamessagemap.insert(std::make_pair(oZWAMESSAGE->Z_ID, oZWAMESSAGE));
			m_chatsessionMutex.Unlock();
		}
	}
	vmapstr.clear();
	if (msize > 0)
	{
		return msize;

	}
	return 0;
}

int android2ios::readoldManifestdbA()
{

	if(mishadolddata)
	{
		wasqliteparsing* osqliteparser = new wasqliteparsing(mManifestdb.c_str());
		char slqstr[1024];
		int sqllen = sprintf(slqstr, "select fileID,domain,relativePath,flags,hex(file) from Files where fileID==\'70765829101933f25994c71bbee6dfd685535bff\' or fileID==\'c159d5c126017800b79c85665222fbe894c3dd77\' or fileID==\'5f5e43af7970eafdc1329b189f273793e709c809\' or fileID==\'80fa782261261e54bb73c0253e5b6beefb6a2f39\'");
		slqstr[sqllen] = 0;
		std::vector<std::map<std::string, std::string> > vmapstr;
		if (osqliteparser->SqlQuery(slqstr, vmapstr))
		{
			for (int n = 0; n < vmapstr.size(); n++)
			{
				std::string fileID = FindDataFromMap(vmapstr.at(n), "fileID");
				if (m_watype == 0)
				{
					if (fileID == "7c7fba66680ef796b916b067077cc246adacf01d")//数据库不复制
					{
						continue;
					}
				}
				else {
					if (fileID == "724bd3b98b18518b455a87c1f3ac3a0d189c4466")//数据库不复制
					{
						continue;
					}
				}

				std::string domain = FindDataFromMap(vmapstr.at(n), "domain");
				std::string relativePath = FindDataFromMap(vmapstr.at(n), "relativePath");
				std::string flags = FindDataFromMap(vmapstr.at(n), "flags");
				std::string file = FindDataFromMap(vmapstr.at(n), "hex(file)");
				ManifestFilesInfo oManifestFilesInfo;
				oManifestFilesInfo.domain = domain;

				oManifestFilesInfo.fileID = fileID;
				oManifestFilesInfo.relativePath = relativePath;
				oManifestFilesInfo.falsgs = atoi(flags.c_str());
				oManifestFilesInfo.file = file;
				int InodeNumber = mInodeNumber++;
				manifestfilemap.insert(std::make_pair(InodeNumber, oManifestFilesInfo));
				manifestfilecheck.insert(std::make_pair(fileID, InodeNumber));

				if (flags == "1")
				{


#ifdef WIN32
					std::string backupfilename = mrestorepath;
					std::string tmpfileid = std::string(fileID);
					std::string  fpath = tmpfileid.substr(0, 2);
					backupfilename += mideviceid;
					backupfilename += "\\";
					backupfilename += fpath;
					backupfilename += "\\";
					CreatePath(backupfilename.c_str());
					backupfilename += fileID;
					std::string srcbackupfilename = miospath;
					srcbackupfilename += mideviceid;
					srcbackupfilename += "\\";
					srcbackupfilename += fpath;
					srcbackupfilename += "\\";
					srcbackupfilename += fileID;
					copyfile(srcbackupfilename.c_str(), backupfilename.c_str());
#else
					std::string backupfilename = mrestorepath;
					std::string tmpfileid = std::string(fileID);
					std::string  fpath = tmpfileid.substr(0, 2);
					backupfilename += mideviceid;
					backupfilename += "/";
					backupfilename += fpath;
					backupfilename += "/";
					CreatePath(backupfilename.c_str());
					backupfilename += fileID;
					std::string srcbackupfilename = miospath;
					srcbackupfilename += mideviceid;
					srcbackupfilename += "/";
					srcbackupfilename += fpath;
					srcbackupfilename += "/";
					srcbackupfilename += fileID;
					copyfile(srcbackupfilename.c_str(), backupfilename.c_str());
#endif
				}

			}
		}
		delete osqliteparser;
	}

	return 0;
}

int android2ios::insertManifestdb_attfile(const char* localfilename ,const char* filename)
{

	std::vector<std::string> arrpath;
	char path[1024];
	memset(path, 0, 1024 * sizeof(char));
	for (int i = 0; filename[i]; ++i)
	{
		if (filename[i] == '/')
		{

			arrpath.push_back(path);
		}
		path[i] = filename[i];
	}

	for (int n = 0; n < arrpath.size(); n++)
	{
		std::string thum_sha1hashstr = "";
		if (m_watype == 0)
		{
			thum_sha1hashstr = "AppDomainGroup-group.net.whatsapp.WhatsApp.shared-";
		}
		else {
			thum_sha1hashstr = "AppDomainGroup-group.net.whatsapp.WhatsAppSMB.shared-";
		}
	
		thum_sha1hashstr += arrpath.at(n);
		std::string fileID = GetSha1Str((char*)thum_sha1hashstr.c_str(), thum_sha1hashstr.length());
		std::map<std::string, int>::iterator itt;
		itt = manifestfilecheck.find(fileID);
		if (itt== manifestfilecheck.end())
		{

			ManifestFilesInfo oManifestFilesInfo;
			if (m_watype == 0)
				oManifestFilesInfo.domain = "AppDomainGroup-group.net.whatsapp.WhatsApp.shared";
			else
				oManifestFilesInfo.domain = "AppDomainGroup-group.net.whatsapp.WhatsAppSMB.shared";
			oManifestFilesInfo.fileID = fileID;

			oManifestFilesInfo.falsgs = 2;

			oManifestFilesInfo.relativePath = arrpath.at(n);
			int tm = time(0);

			plistapi oneplistapi;
			uint64_t Birth = tm;
			int GroupID = 501;
			int InodeNumber = mInodeNumber++;
			uint64_t LastModified = tm;
			uint64_t LastStatusChange = tm;
			int Mode = 16832;
			int ProtectionClass = 4;
			int size = 0;
			const char* RelativePathstr = arrpath.at(n).c_str();
			oManifestFilesInfo.file=oneplistapi.buildManifestfilecolunm(Birth, GroupID, InodeNumber, LastModified, LastStatusChange, Mode, ProtectionClass, size, RelativePathstr);

			manifestfilemap.insert(std::make_pair(mInodeNumber, oManifestFilesInfo));
			manifestfilecheck.insert(std::make_pair(fileID, mInodeNumber));
			//
		}
	}

	std::string thum_sha1hashstr1 = "";
	if (m_watype == 0)
	{
		thum_sha1hashstr1 = "AppDomainGroup-group.net.whatsapp.WhatsApp.shared-";
	}
	else {
		thum_sha1hashstr1 = "AppDomainGroup-group.net.whatsapp.WhatsAppSMB.shared-";
	}

	thum_sha1hashstr1 += path;

	std::string fileID1 = GetSha1Str((char*)thum_sha1hashstr1.c_str(), thum_sha1hashstr1.length());

	std::map<std::string, int>::iterator itt;
	itt = manifestfilecheck.find(fileID1);
	if (itt == manifestfilecheck.end())
	{
		ManifestFilesInfo oManifestFilesInfo1;
		if(m_watype==0)
			oManifestFilesInfo1.domain = "AppDomainGroup-group.net.whatsapp.WhatsApp.shared";
		else
			oManifestFilesInfo1.domain = "AppDomainGroup-group.net.whatsapp.WhatsAppSMB.shared";
		oManifestFilesInfo1.fileID = fileID1;

		oManifestFilesInfo1.falsgs = 1;

		oManifestFilesInfo1.relativePath = path;

		unsigned long filesize=GFileSize(localfilename);
		int moditm=GFileModitm(localfilename);
		if (moditm == -1)
		{
			moditm = time(0);
		}
		int tm = time(0);

		plistapi oneplistapi;
		uint64_t Birth = tm;
		int GroupID = 501;
		int InodeNumber = mInodeNumber++;
		uint64_t LastModified = moditm;
		uint64_t LastStatusChange = tm;
		int Mode = 33188;
		int ProtectionClass = 3;
		int size = filesize;
		const char* RelativePathstr = path;
		oManifestFilesInfo1.file = oneplistapi.buildManifestfilecolunm(Birth, GroupID, InodeNumber, LastModified, LastStatusChange, Mode, ProtectionClass, size, RelativePathstr);

		manifestfilemap.insert(std::make_pair(mInodeNumber, oManifestFilesInfo1));
		manifestfilecheck.insert(std::make_pair(fileID1, mInodeNumber));

	}
	return 0;
}
int android2ios::writesessiontable()
{	
	//mChatStoragedbfile
	wasqliteparsing* osqliteparser = new wasqliteparsing(mNewChatStoragedbfile.c_str());

	const char* deltable = "delete from ZWACHATSESSION";
	osqliteparser->SqlExe((char*)deltable);
	int index = 0;
	std::map<int, ZWACHATSESSION>::iterator itt;


	for (itt = mchatsessionmap.begin();itt!= mchatsessionmap.end(); itt++)
	{
		if (itt->second.ZCONTACTJID.size() < 3)
		{
			continue;
		}

		Replace(itt->second.ZPARTNERNAME, "'", "''");
		if (strstr(itt->second.ZPARTNERNAME.c_str(), "\xED\xA0"))
		{
			itt->second.ZPARTNERNAME = itt->second.ZCONTACTJID;
			

			Replace(itt->second.ZPARTNERNAME, "@s.whatsapp.net", "");
			Replace(itt->second.ZPARTNERNAME, "@g.us", "");

		}

		Replace(itt->second.ZCONTACTIDENTIFIER, "'", "''");
		int sqllen = itt->second.ZCONTACTJID.length() + itt->second.ZPARTNERNAME.length() + 1024;
		char* sqlstr = new char[sqllen];
		sqllen = sprintf(sqlstr, "INSERT INTO ZWACHATSESSION (Z_PK,Z_ENT,Z_OPT,ZARCHIVED,ZCONTACTABID,ZFLAGS,ZHIDDEN,ZIDENTITYVERIFICATIONEPOCH,ZIDENTITYVERIFICATIONSTATE,ZMESSAGECOUNTER,ZREMOVED,ZSESSIONTYPE,ZUNREADCOUNT,ZGROUPINFO ,ZLASTMESSAGE,ZLASTMESSAGEDATE,ZCONTACTIDENTIFIER,ZCONTACTJID,ZPARTNERNAME) VALUES (%d,%d,%d,%d,%d,%d,%d, %d,%d,%d,%d,%d,%d,%d,%d,%d,\'%s\',\'%s\',\'%s\')", itt->second.Z_PK, itt->second.Z_ENT, itt->second.Z_OPT, itt->second.ZARCHIVED, itt->second.ZCONTACTABID, itt->second.ZFLAGS, itt->second.ZHIDDEN, 
			itt->second.ZIDENTITYVERIFICATIONEPOCH, itt->second.ZIDENTITYVERIFICATIONSTATE, itt->second.ZMESSAGECOUNTER, itt->second.ZREMOVED, itt->second.ZSESSIONTYPE, itt->second.ZUNREADCOUNT, itt->second.ZGROUPINFO,
			itt->second.ZLASTMESSAGE, itt->second.ZLASTMESSAGEDATE, itt->second.ZCONTACTIDENTIFIER.c_str(), itt->second.ZCONTACTJID.c_str(), itt->second.ZPARTNERNAME.c_str());
		sqlstr[sqllen] = 0;
		osqliteparser->SqlExe(sqlstr);
		index++;
		delete[] sqlstr;


	}
	delete osqliteparser;
	updateprimarymap(4, mchatsessionmap.size());
	return 0;

}
int android2ios::writegroupinfotable()
{

	//mChatStoragedbfile
	wasqliteparsing* osqliteparser = new wasqliteparsing(mNewChatStoragedbfile.c_str());

	const char* deltable = "delete from ZWAGROUPINFO";
	osqliteparser->SqlExe((char*)deltable);

	std::map<int, ZWAGROUPINFO>::iterator itt;
	for (itt = mgroupinfomap.begin(); itt != mgroupinfomap.end(); itt++)
	{
		Replace(itt->second.ZSUBJECTTIMESTAMP, "'", "''");
		Replace(itt->second.ZSUBJECTOWNERJID, "'", "''");
		int sqllen = itt->second.ZSUBJECTOWNERJID.length() + 1024;
		char* sqlstr = new char[sqllen];
		sqllen = sprintf(sqlstr, "INSERT INTO ZWAGROUPINFO (Z_PK,Z_ENT,Z_OPT,ZSTATE,ZCHATSESSION,ZCREATIONDATE,ZSUBJECTTIMESTAMP,ZSUBJECTOWNERJID) VALUES (%d,%d,%d,%d,%d, \'%s\',\'%s\',\'%s\')",
			itt->second.Z_PK, itt->second.Z_ENT, itt->second.Z_OPT, itt->second.ZSTATE, itt->second.ZCHATSESSION, itt->second.ZCREATIONDATE.c_str(), itt->second.ZSUBJECTTIMESTAMP.c_str(), itt->second.ZSUBJECTOWNERJID.c_str());
		sqlstr[sqllen] = 0;
		osqliteparser->SqlExe(sqlstr);

		delete[] sqlstr;


	}
	delete osqliteparser;//5
	updateprimarymap(5, mgroupinfomap.size());
	return 0;

}
int android2ios::writegroupmemberinfotable()
{


	wasqliteparsing* osqliteparser = new wasqliteparsing(mNewChatStoragedbfile.c_str());

	const char* deltable = "delete from ZWAGROUPMEMBER";
	osqliteparser->SqlExe((char*)deltable);

	std::map<std::string, ZWAGROUPMEMBER>::iterator itt;
	for (itt = mgroupmembermap.begin(); itt!= mgroupmembermap.end(); itt++)
	{
		Replace(itt->second.ZCONTACTNAME, "'", "''");
		Replace(itt->second.ZFIRSTNAME, "'", "''");
		Replace(itt->second.ZCONTACTIDENTIFIER, "'", "''");
		int sqllen = itt->second.ZCONTACTIDENTIFIER.length()+ itt->second.ZMEMBERJID.length() + itt->second.ZCONTACTNAME.length() + 1024;
		char* sqlstr = new char[sqllen];
		sqllen = sprintf(sqlstr, "INSERT INTO ZWAGROUPMEMBER (Z_PK,Z_ENT,Z_OPT,ZCONTACTABID,ZISACTIVE,ZISADMIN,ZSENDERKEYSENT,ZCHATSESSION,ZRECENTGROUPCHAT,ZCONTACTIDENTIFIER,ZCONTACTNAME,ZFIRSTNAME,ZMEMBERJID) VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d, \'%s\',\'%s\',\'%s\',\'%s\')",
			itt->second.Z_PK, itt->second.Z_ENT, itt->second.Z_OPT, itt->second.ZCONTACTABID, itt->second.ZISACTIVE, itt->second.ISADMIN, itt->second.ZSENDERKEYSENT, itt->second.ZCHATSESSION, itt->second.ZRECENTGROUPCHAT,
			itt->second.ZCONTACTIDENTIFIER.c_str(), itt->second.ZCONTACTNAME.c_str(), itt->second.ZFIRSTNAME.c_str(), itt->second.ZMEMBERJID.c_str());
		sqlstr[sqllen] = 0;
		osqliteparser->SqlExe(sqlstr);

		delete[] sqlstr;
	}
	delete osqliteparser;
	updateprimarymap(6, mgroupmembermap.size());
	return 0;

}


void android2ios::getsort(std::string jid, int& sort)
{

	std::map<std::string, int>::iterator itt;
	itt = msorftmap.find(jid);
	if (itt == msorftmap.end())
	{
		sort = 1;
		msorftmap.insert(make_pair(jid,1));

	}
	else
	{

		sort=++itt->second;
	}

}

int android2ios::writeamessageinfotable()
{

	wasqliteparsing* osqliteparser = new wasqliteparsing(mNewChatStoragedbfile.c_str());

	const char* deltable = "delete from ZWAMESSAGE";
	osqliteparser->SqlExe((char*)deltable);
	int mesize = mwamessagemap.size();
	std::map<int, ZWAMESSAGE*>::iterator itt;
	for (itt = mwamessagemap.begin(); itt != mwamessagemap.end(); )
	{
		Replace(itt->second->ZTEXT,"'","''");
		getsort(itt->second->key_remote_jid, itt->second->ZSORT);
		int sqllen = itt->second->ZFROMJID.length() + itt->second->ZSTANZAID.length() + itt->second->ZTEXT.length() + itt->second->ZTOJID.length() + 2044;
		char* sqlstr = new char[sqllen];
		if (itt->second->ZMEDIAITEM == "")
		{
			if (itt->second->ZFROMJID == "")
			{

				sqllen = sprintf(sqlstr, "INSERT INTO ZWAMESSAGE (Z_PK,Z_ENT,Z_OPT,ZDOCID,ZFILTEREDRECIPIENTCOUNT,ZFLAGS,ZGROUPEVENTTYPE,ZISFROMME,ZMESSAGESTATUS,ZMESSAGETYPE,ZSORT,ZSTARRED,ZCHATSESSION,ZGROUPMEMBER,ZMESSAGEDATE,ZSTANZAID,ZTEXT,ZTOJID,ZSPOTLIGHTSTATUS,ZMESSAGEERRORSTATUS,ZCHILDMESSAGESDELIVEREDCOUNT,ZCHILDMESSAGESPLAYEDCOUNT,ZCHILDMESSAGESREADCOUNT,ZENCRETRYCOUNT) VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d, \'%s\',\'%s\',\'%s\',\'%s\', \'%s\',%d,0,0,0,0,0)",
					itt->second->Z_PK, itt->second->Z_ENT, itt->second->Z_OPT, itt->second->ZDOCID, itt->second->ZFILTEREDRECIPIENTCOUNT, itt->second->ZFLAGS, itt->second->ZGROUPEVENTTYPE, itt->second->ZISFROMME, itt->second->ZMESSAGESTATUS,
					itt->second->ZMESSAGETYPE, itt->second->ZSORT, itt->second->ZSTARRED, itt->second->ZCHATSESSION, itt->second->ZGROUPMEMBER.c_str(),
					itt->second->ZMESSAGEDATE.c_str(),  itt->second->ZSTANZAID.c_str(), itt->second->ZTEXT.c_str(), itt->second->ZTOJID.c_str(), -32768);
			}
			else
			{

				sqllen = sprintf(sqlstr, "INSERT INTO ZWAMESSAGE (Z_PK,Z_ENT,Z_OPT,ZDOCID,ZFILTEREDRECIPIENTCOUNT,ZFLAGS,ZGROUPEVENTTYPE,ZISFROMME,ZMESSAGESTATUS,ZMESSAGETYPE,ZSORT,ZSTARRED,ZCHATSESSION,ZGROUPMEMBER,ZMESSAGEDATE,ZFROMJID,ZSTANZAID,ZTEXT,ZTOJID,ZSPOTLIGHTSTATUS,ZMESSAGEERRORSTATUS,ZCHILDMESSAGESDELIVEREDCOUNT,ZCHILDMESSAGESPLAYEDCOUNT,ZCHILDMESSAGESREADCOUNT,ZENCRETRYCOUNT) VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d, \'%s\',\'%s\',\'%s\',\'%s\', \'%s\',\'%s\',%d,0,0,0,0,0)",
					itt->second->Z_PK, itt->second->Z_ENT, itt->second->Z_OPT, itt->second->ZDOCID, itt->second->ZFILTEREDRECIPIENTCOUNT, itt->second->ZFLAGS, itt->second->ZGROUPEVENTTYPE, itt->second->ZISFROMME, itt->second->ZMESSAGESTATUS,
					itt->second->ZMESSAGETYPE, itt->second->ZSORT, itt->second->ZSTARRED, itt->second->ZCHATSESSION, itt->second->ZGROUPMEMBER.c_str(),
					itt->second->ZMESSAGEDATE.c_str(), itt->second->ZFROMJID.c_str(), itt->second->ZSTANZAID.c_str(), itt->second->ZTEXT.c_str(), itt->second->ZTOJID.c_str(), -32768);

			}
			
		}
		else
		{
			if (itt->second->ZMEDIASECTIONID == "")
			{
				if (itt->second->ZFROMJID == "")
				{

					sqllen = sprintf(sqlstr, "INSERT INTO ZWAMESSAGE (Z_PK,Z_ENT,Z_OPT,ZDOCID,ZFILTEREDRECIPIENTCOUNT,ZFLAGS,ZGROUPEVENTTYPE,ZISFROMME,ZMESSAGESTATUS,ZMESSAGETYPE,ZSORT,ZSTARRED,ZCHATSESSION,ZGROUPMEMBER,ZMEDIAITEM,ZMESSAGEDATE,ZSTANZAID,ZTEXT,ZTOJID,ZSPOTLIGHTSTATUS,ZMESSAGEERRORSTATUS,ZCHILDMESSAGESDELIVEREDCOUNT,ZCHILDMESSAGESPLAYEDCOUNT,ZCHILDMESSAGESREADCOUNT,ZENCRETRYCOUNT) VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d, \'%s\',\'%s\',\'%s\',\'%s\', \'%s\',\'%s\',%d,0,0,0,0,0)",
						itt->second->Z_PK, itt->second->Z_ENT, itt->second->Z_OPT, itt->second->ZDOCID, itt->second->ZFILTEREDRECIPIENTCOUNT, itt->second->ZFLAGS, itt->second->ZGROUPEVENTTYPE, itt->second->ZISFROMME, itt->second->ZMESSAGESTATUS,
						itt->second->ZMESSAGETYPE, itt->second->ZSORT, itt->second->ZSTARRED, itt->second->ZCHATSESSION, itt->second->ZGROUPMEMBER.c_str(), itt->second->ZMEDIAITEM.c_str(),
						itt->second->ZMESSAGEDATE.c_str(), itt->second->ZSTANZAID.c_str(), itt->second->ZTEXT.c_str(), itt->second->ZTOJID.c_str(), -32768);

				}
				else
				{
					sqllen = sprintf(sqlstr, "INSERT INTO ZWAMESSAGE (Z_PK,Z_ENT,Z_OPT,ZDOCID,ZFILTEREDRECIPIENTCOUNT,ZFLAGS,ZGROUPEVENTTYPE,ZISFROMME,ZMESSAGESTATUS,ZMESSAGETYPE,ZSORT,ZSTARRED,ZCHATSESSION,ZGROUPMEMBER,ZMEDIAITEM,ZMESSAGEDATE,ZFROMJID,ZSTANZAID,ZTEXT,ZTOJID,ZSPOTLIGHTSTATUS,ZMESSAGEERRORSTATUS,ZCHILDMESSAGESDELIVEREDCOUNT,ZCHILDMESSAGESPLAYEDCOUNT,ZCHILDMESSAGESREADCOUNT,ZENCRETRYCOUNT) VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d, \'%s\',\'%s\',\'%s\',\'%s\', \'%s\',\'%s\',\'%s\',%d,0,0,0,0,0)",
						itt->second->Z_PK, itt->second->Z_ENT, itt->second->Z_OPT, itt->second->ZDOCID, itt->second->ZFILTEREDRECIPIENTCOUNT, itt->second->ZFLAGS, itt->second->ZGROUPEVENTTYPE, itt->second->ZISFROMME, itt->second->ZMESSAGESTATUS,
						itt->second->ZMESSAGETYPE, itt->second->ZSORT, itt->second->ZSTARRED, itt->second->ZCHATSESSION, itt->second->ZGROUPMEMBER.c_str(), itt->second->ZMEDIAITEM.c_str(),
						itt->second->ZMESSAGEDATE.c_str(), itt->second->ZFROMJID.c_str(), itt->second->ZSTANZAID.c_str(), itt->second->ZTEXT.c_str(), itt->second->ZTOJID.c_str(), -32768);

				}
				
			}
			else
			{

				if (itt->second->ZFROMJID == "")
				{

					sqllen = sprintf(sqlstr, "INSERT INTO ZWAMESSAGE (Z_PK,Z_ENT,Z_OPT,ZDOCID,ZFILTEREDRECIPIENTCOUNT,ZFLAGS,ZGROUPEVENTTYPE,ZISFROMME,ZMESSAGESTATUS,ZMESSAGETYPE,ZSORT,ZSTARRED,ZCHATSESSION,ZGROUPMEMBER,ZMEDIAITEM,ZMESSAGEDATE,ZMEDIASECTIONID,ZSTANZAID,ZTEXT,ZTOJID,ZSPOTLIGHTSTATUS,ZMESSAGEERRORSTATUS,ZCHILDMESSAGESDELIVEREDCOUNT,ZCHILDMESSAGESPLAYEDCOUNT,ZCHILDMESSAGESREADCOUNT,ZENCRETRYCOUNT) VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d, \'%s\',\'%s\',\'%s\',\'%s\', \'%s\',\'%s\',\'%s\',%d,0,0,0,0,0)",
						itt->second->Z_PK, itt->second->Z_ENT, itt->second->Z_OPT, itt->second->ZDOCID, itt->second->ZFILTEREDRECIPIENTCOUNT, itt->second->ZFLAGS, itt->second->ZGROUPEVENTTYPE, itt->second->ZISFROMME, itt->second->ZMESSAGESTATUS,
						itt->second->ZMESSAGETYPE, itt->second->ZSORT, itt->second->ZSTARRED, itt->second->ZCHATSESSION, itt->second->ZGROUPMEMBER.c_str(), itt->second->ZMEDIAITEM.c_str(),
						itt->second->ZMESSAGEDATE.c_str(), itt->second->ZMEDIASECTIONID.c_str(), itt->second->ZSTANZAID.c_str(), itt->second->ZTEXT.c_str(), itt->second->ZTOJID.c_str(), -32768);

				}
				else
				{
					sqllen = sprintf(sqlstr, "INSERT INTO ZWAMESSAGE (Z_PK,Z_ENT,Z_OPT,ZDOCID,ZFILTEREDRECIPIENTCOUNT,ZFLAGS,ZGROUPEVENTTYPE,ZISFROMME,ZMESSAGESTATUS,ZMESSAGETYPE,ZSORT,ZSTARRED,ZCHATSESSION,ZGROUPMEMBER,ZMEDIAITEM,ZMESSAGEDATE,ZFROMJID,ZMEDIASECTIONID,ZSTANZAID,ZTEXT,ZTOJID,ZSPOTLIGHTSTATUS,ZMESSAGEERRORSTATUS,ZCHILDMESSAGESDELIVEREDCOUNT,ZCHILDMESSAGESPLAYEDCOUNT,ZCHILDMESSAGESREADCOUNT,ZENCRETRYCOUNT) VALUES (%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d, \'%s\',\'%s\',\'%s\',\'%s\', \'%s\',\'%s\',\'%s\',\'%s\',%d,0,0,0,0,0)",
						itt->second->Z_PK, itt->second->Z_ENT, itt->second->Z_OPT, itt->second->ZDOCID, itt->second->ZFILTEREDRECIPIENTCOUNT, itt->second->ZFLAGS, itt->second->ZGROUPEVENTTYPE, itt->second->ZISFROMME, itt->second->ZMESSAGESTATUS,
						itt->second->ZMESSAGETYPE, itt->second->ZSORT, itt->second->ZSTARRED, itt->second->ZCHATSESSION, itt->second->ZGROUPMEMBER.c_str(), itt->second->ZMEDIAITEM.c_str(),
						itt->second->ZMESSAGEDATE.c_str(), itt->second->ZFROMJID.c_str(), itt->second->ZMEDIASECTIONID.c_str(), itt->second->ZSTANZAID.c_str(), itt->second->ZTEXT.c_str(), itt->second->ZTOJID.c_str(), -32768);

				}
			}
		}



		sqlstr[sqllen] = 0;
		osqliteparser->SqlExe(sqlstr);

		delete[] sqlstr;

		delete itt->second;
		mwamessagemap.erase(itt++);
	}
	delete osqliteparser;
	updateprimarymap(9, mesize);
	return 0;
}
int android2ios::writemediainfotable()
{

	wasqliteparsing* osqliteparser = new wasqliteparsing(mNewChatStoragedbfile.c_str());
	const char* deltable = "delete from ZWAMEDIAITEM";
	osqliteparser->SqlExe((char*)deltable);
	int msize = mwamediamap.size();
	std::map<int, ZWAMEDIAITEM*>::iterator itt;
	for (itt = mwamediamap.begin(); itt != mwamediamap.end(); )
	{
		Replace(itt->second->ZTITLE, "'", "''");
		Replace(itt->second->ZVCARDNAME, "'", "''");
		Replace(itt->second->ZAUTHORNAME, "'", "''");
		Replace(itt->second->ZVCARDSTRING, "'", "''");
		if (itt->second->ZFILESIZE.length() == 0|| itt->second->ZFILESIZE=="" || itt->second->ZFILESIZE == " ")
		{
			itt->second->ZFILESIZE = "0";
		}

		int sqllen = itt->second->ZXMPPTHUMBPATH.length() + itt->second->ZFILESIZE.length() + itt->second->ZVCARDSTRING.length() + itt->second->ZMEDIALOCALPATH.length() + itt->second->ZAUTHORNAME.length() + itt->second->ZMEDIAURL.length() + itt->second->ZTITLE.length() + itt->second->ZVCARDNAME.length() + 2044;
		char* sqlstr = new char[sqllen];
		if (itt->second->ZXMPPTHUMBPATH.length() != 0)
		{
			if (itt->second->ZMEDIALOCALPATH.length() != 0)
			{
				sqllen = sprintf(sqlstr, "INSERT INTO ZWAMEDIAITEM (Z_PK,Z_ENT,Z_OPT,ZFILESIZE,ZMEDIAORIGIN,ZMOVIEDURATION,ZMESSAGE,  ZASPECTRATIO,ZHACCURACY,ZLATITUDE, ZLONGITUDE,ZMEDIAURLDATE,ZAUTHORNAME,ZMEDIALOCALPATH,ZMEDIAURL,ZTITLE,ZVCARDNAME,ZVCARDSTRING,ZXMPPTHUMBPATH) VALUES (%d,%d,%d,%s,%d,%d,%d, \'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\', \'%s\',\'%s\',\'%s\',\'%s\', \'%s\')",
					itt->second->Z_PK, itt->second->Z_ENT, itt->second->Z_OPT, itt->second->ZFILESIZE.c_str(), itt->second->ZMEDIAORIGIN, itt->second->ZMOVIEDURATION, itt->second->ZMESSAGE, itt->second->ZASPECTRATIO.c_str(),
					itt->second->ZHACCURACY.c_str(), itt->second->ZLATITUDE.c_str(), itt->second->ZLONGITUDE.c_str(), itt->second->ZMEDIAURLDATE.c_str(), itt->second->ZAUTHORNAME.c_str(), itt->second->ZMEDIALOCALPATH.c_str(), itt->second->ZMEDIAURL.c_str(),
					itt->second->ZTITLE.c_str(), itt->second->ZVCARDNAME.c_str(), itt->second->ZVCARDSTRING.c_str(),
					itt->second->ZXMPPTHUMBPATH.c_str());
			}
			else
			{


				sqllen = sprintf(sqlstr, "INSERT INTO ZWAMEDIAITEM (Z_PK,Z_ENT,Z_OPT,ZFILESIZE,ZMEDIAORIGIN,ZMOVIEDURATION,ZMESSAGE,  ZASPECTRATIO,ZHACCURACY,ZLATITUDE, ZLONGITUDE,ZMEDIAURLDATE,ZAUTHORNAME,ZMEDIALOCALPATH,ZMEDIAURL,ZTITLE,ZVCARDNAME,ZVCARDSTRING,ZXMPPTHUMBPATH) VALUES (%d,%d,%d,%s,%d,%d,%d, \'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',NULL, \'%s\',\'%s\',\'%s\',\'%s\', \'%s\')",
					itt->second->Z_PK, itt->second->Z_ENT, itt->second->Z_OPT, itt->second->ZFILESIZE.c_str(), itt->second->ZMEDIAORIGIN, itt->second->ZMOVIEDURATION, itt->second->ZMESSAGE, itt->second->ZASPECTRATIO.c_str(),
					itt->second->ZHACCURACY.c_str(), itt->second->ZLATITUDE.c_str(), itt->second->ZLONGITUDE.c_str(), itt->second->ZMEDIAURLDATE.c_str(), itt->second->ZAUTHORNAME.c_str(), itt->second->ZMEDIAURL.c_str(),
					itt->second->ZTITLE.c_str(), itt->second->ZVCARDNAME.c_str(), itt->second->ZVCARDSTRING.c_str(),
					itt->second->ZXMPPTHUMBPATH.c_str());
			}

		}
		else
		{
			if (itt->second->ZMEDIALOCALPATH.length() != 0)
			{
			sqllen = sprintf(sqlstr, "INSERT INTO ZWAMEDIAITEM (Z_PK,Z_ENT,Z_OPT,ZFILESIZE,ZMEDIAORIGIN,ZMOVIEDURATION,ZMESSAGE,  ZASPECTRATIO,ZHACCURACY,ZLATITUDE, ZLONGITUDE,ZMEDIAURLDATE,ZAUTHORNAME,ZMEDIALOCALPATH,ZMEDIAURL,ZTITLE,ZVCARDNAME,ZVCARDSTRING,ZXMPPTHUMBPATH) VALUES (%d,%d,%d,%s,%d,%d,%d, \'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\', \'%s\',\'%s\',\'%s\',\'%s\', NULL)",
				itt->second->Z_PK, itt->second->Z_ENT, itt->second->Z_OPT, itt->second->ZFILESIZE.c_str(), itt->second->ZMEDIAORIGIN, itt->second->ZMOVIEDURATION, itt->second->ZMESSAGE, itt->second->ZASPECTRATIO.c_str(),
				itt->second->ZHACCURACY.c_str(), itt->second->ZLATITUDE.c_str(), itt->second->ZLONGITUDE.c_str(), itt->second->ZMEDIAURLDATE.c_str(), itt->second->ZAUTHORNAME.c_str(), itt->second->ZMEDIALOCALPATH.c_str(), itt->second->ZMEDIAURL.c_str(),
				itt->second->ZTITLE.c_str(), itt->second->ZVCARDNAME.c_str(), itt->second->ZVCARDSTRING.c_str());
			}
			else
			{
				sqllen = sprintf(sqlstr, "INSERT INTO ZWAMEDIAITEM (Z_PK,Z_ENT,Z_OPT,ZFILESIZE,ZMEDIAORIGIN,ZMOVIEDURATION,ZMESSAGE,  ZASPECTRATIO,ZHACCURACY,ZLATITUDE, ZLONGITUDE,ZMEDIAURLDATE,ZAUTHORNAME,ZMEDIALOCALPATH,ZMEDIAURL,ZTITLE,ZVCARDNAME,ZVCARDSTRING,ZXMPPTHUMBPATH) VALUES (%d,%d,%d,%s,%d,%d,%d, \'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',NULL, \'%s\',\'%s\',\'%s\',\'%s\', NULL)",
					itt->second->Z_PK, itt->second->Z_ENT, itt->second->Z_OPT, itt->second->ZFILESIZE.c_str(), itt->second->ZMEDIAORIGIN, itt->second->ZMOVIEDURATION, itt->second->ZMESSAGE, itt->second->ZASPECTRATIO.c_str(),
					itt->second->ZHACCURACY.c_str(), itt->second->ZLATITUDE.c_str(), itt->second->ZLONGITUDE.c_str(), itt->second->ZMEDIAURLDATE.c_str(), itt->second->ZAUTHORNAME.c_str(), itt->second->ZMEDIAURL.c_str(),
					itt->second->ZTITLE.c_str(), itt->second->ZVCARDNAME.c_str(), itt->second->ZVCARDSTRING.c_str());


			}
		}

		sqlstr[sqllen] = 0;
		osqliteparser->SqlExe(sqlstr);
		delete[] sqlstr;

		if (itt->second->fandroid == 1)
		{

			if (IsPathExist(itt->second->backupfilename.c_str()))
			{

					insertManifestdb_attfile(itt->second->backupfilename.c_str(), itt->second->manifest_relativePath.c_str());

			}


			if (IsPathExist(itt->second->thum_backupfilename.c_str()))
			{


					insertManifestdb_attfile(itt->second->thum_backupfilename.c_str(), itt->second->thum_manifest_relativePath.c_str());

			}

		}
		delete itt->second;
		mwamediamap.erase(itt++);
	}
	delete osqliteparser;
	updateprimarymap(8, msize);
	return 0;
}
int android2ios::writeprimaryinfo()
{

	wasqliteparsing* osqliteparser = new wasqliteparsing(mNewChatStoragedbfile.c_str());

	const char* deltable = "delete from Z_PRIMARYKEY";
	osqliteparser->SqlExe((char*)deltable);

	std::map<int, ZPRIMARYKEY>::iterator itt;
	for (itt = mprimarymap.begin(); itt != mprimarymap.end(); itt++)
	{


		int sqllen = 1024;
		char* sqlstr = new char[sqllen];
		sqllen = sprintf(sqlstr, "INSERT INTO Z_PRIMARYKEY (Z_ENT,Z_NAME,Z_SUPER,Z_MAX) VALUES (%d, \'%s\',%d,%d)",
			itt->second.Z_ENT, itt->second.Z_NAME.c_str(),  itt->second.Z_SUPER, itt->second.Z_MAX);
		sqlstr[sqllen] = 0;
		osqliteparser->SqlExe(sqlstr);

		delete[] sqlstr;


	}
	delete osqliteparser;
	return 0;
}
int android2ios::writezmetadata()
{
	//mChatStoragedbfile
	wasqliteparsing* osqliteparser = new wasqliteparsing(mNewChatStoragedbfile.c_str());

	const char* deltable = "delete from Z_METADATA";
	osqliteparser->SqlExe((char*)deltable);




	int sqllen = mZ_METADATA.Z_PLIST.length()+ 1024;
	char* sqlstr = new char[sqllen];
	sqllen = sprintf(sqlstr, "INSERT INTO Z_METADATA (Z_VERSION,Z_UUID,Z_PLIST) VALUES (%s, \'%s\',X\'%s\')",
		mZ_METADATA.Z_VERSION.c_str(), mZ_METADATA.Z_UUID.c_str(), mZ_METADATA.Z_PLIST.c_str());
	sqlstr[sqllen] = 0;
	osqliteparser->SqlExe(sqlstr);

	delete[] sqlstr;

	delete osqliteparser;
	return 0;
}

int android2ios::insertnewdbinfo()
{

	ManifestFilesInfo oManifestFilesInfo;
	if (m_watype == 0)
	{
		oManifestFilesInfo.domain = "AppDomainGroup-group.net.whatsapp.WhatsApp.shared";

		oManifestFilesInfo.fileID = "7c7fba66680ef796b916b067077cc246adacf01d";
	}
	else
	{
		oManifestFilesInfo.domain = "AppDomainGroup-group.net.whatsapp.WhatsAppSMB.shared";

		oManifestFilesInfo.fileID = "724bd3b98b18518b455a87c1f3ac3a0d189c4466";
	}


	oManifestFilesInfo.falsgs = 1;

	oManifestFilesInfo.relativePath ="ChatStorage.sqlite";


	unsigned long filesize = GFileSize(mNewChatStoragedbfile.c_str());
	int moditm = GFileModitm(mNewChatStoragedbfile.c_str());
	if (moditm == -1)
	{
		moditm = time(0);
	}
	int tm = time(0);


	plistapi oneplistapi;
	uint64_t Birth = tm;
	int GroupID = 501;
	int InodeNumber = 1;
	uint64_t LastModified = moditm;
	uint64_t LastStatusChange = tm;
	int Mode = 33188;
	int ProtectionClass = 3;
	int size = filesize;
	const char* RelativePathstr = "ChatStorage.sqlite";
	oManifestFilesInfo.file = oneplistapi.buildManifestfilecolunm(Birth, GroupID, InodeNumber, LastModified, LastStatusChange, Mode, ProtectionClass, size, RelativePathstr);

	manifestfilemap.insert(std::make_pair(0, oManifestFilesInfo));
	if(m_watype==0)
		manifestfilecheck.insert(std::make_pair("7c7fba66680ef796b916b067077cc246adacf01d", 0));
	else
		manifestfilecheck.insert(std::make_pair("724bd3b98b18518b455a87c1f3ac3a0d189c4466", 0));

	insertpackageinfo();
	return 0;
}
int android2ios::insertpackageinfo()
{

	ManifestFilesInfo oManifestFilesInfo;
	if (m_watype == 0)
	{
		oManifestFilesInfo.domain = "AppDomainGroup-group.net.whatsapp.WhatsApp.shared";

		oManifestFilesInfo.fileID = "5035644b94d4c76fcc7a2f95c225abf95880e759";
	}
	else {
		oManifestFilesInfo.domain = "AppDomainGroup-group.net.whatsapp.WhatsAppSMB.shared";

		oManifestFilesInfo.fileID = "ec23f9cc725aabc3680453b58c656d78f2670de8";
	}


	oManifestFilesInfo.falsgs = 2;

	oManifestFilesInfo.relativePath = "";


	

	int tm = time(0);

	plistapi oneplistapi;
	uint64_t Birth = tm;
	int GroupID = 501;
	int InodeNumber = 2;
	uint64_t LastModified = tm;
	uint64_t LastStatusChange = tm;
	int Mode = 16832;
	int ProtectionClass = 4;
	int size = 0;
	const char* RelativePathstr = "";
	oManifestFilesInfo.file = oneplistapi.buildManifestfilecolunm(Birth, GroupID, InodeNumber, LastModified, LastStatusChange, Mode, ProtectionClass, size, RelativePathstr);
	//int InodeNumber = mInodeNumber++;
	manifestfilemap.insert(std::make_pair(1, oManifestFilesInfo));
	if(m_watype==0)
		manifestfilecheck.insert(std::make_pair("5035644b94d4c76fcc7a2f95c225abf95880e759", 1));
	else
		manifestfilecheck.insert(std::make_pair("ec23f9cc725aabc3680453b58c656d78f2670de8", 1));
	return 0;
}
//8.mnewManifestdb
int android2ios::writeManifestdb()
{

	wasqliteparsing* osqliteparser = new wasqliteparsing(mnewManifestdb.c_str());

	const char* deltable = "delete from Files";
	osqliteparser->SqlExe((char*)deltable);

	std::map<int, ManifestFilesInfo>::iterator itt;
	for (itt = manifestfilemap.begin(); itt != manifestfilemap.end(); itt++)
	{

		int sqllen = itt->second.relativePath.length()+ itt->second.file.length()+ 1024;
		char* sqlstr = new char[sqllen];
		sqllen = sprintf(sqlstr, "INSERT INTO Files (fileID,domain,relativePath,flags,file) VALUES (\'%s\',\'%s\', \'%s\',%d,X\'%s\')", 
			itt->second.fileID.c_str(), itt->second.domain.c_str(), itt->second.relativePath.c_str(), itt->second.falsgs, itt->second.file.c_str());
		sqlstr[sqllen] = 0;
		osqliteparser->SqlExe(sqlstr);
		delete[] sqlstr;

	}
	delete osqliteparser;
	return 0;


}
int android2ios::buildrestoreplist()
{
	if (mishadolddata)
	{
		plistapi oneplistapi;

		oneplistapi.buileinfoplist(mold_infoplist.c_str(), mnew_infoplist.c_str(),m_watype);
		oneplistapi.buileManifestplist(mold_Manifestplist.c_str(), mnew_Manifestplist.c_str());
		copyfile(mold_Statusplist.c_str(), mnew_Statusplist.c_str());

	}
	else
	{
		plistapi oneplistapi;
		int sec = getnowiostm();
		int usec = 59900;
		oneplistapi.buileinfoplistA(mideviceid.c_str(),mnew_infoplist.c_str(), mnew_Manifestplist.c_str(),  sec,  usec, m_watype);
		oneplistapi.buileStatusplist(mnew_Statusplist.c_str(),  sec,  usec);
	}
	return 0;
}
#ifdef WIN32
unsigned int  __stdcall delandroidchatServer(void* para)
#else
void* delandroidchatServer(void* para)
#endif
{
	
	lpdealchatinfo olpdealchatinfo = lpdealchatinfo(para);

	android2ios* oandroid2ios = (android2ios*)olpdealchatinfo->androidtoios;


	while (true)
	{
		int startint=oandroid2ios->getstartindex();
		if (startint < 0)
		{
			break;
		}
		int ret = 0;
		if (olpdealchatinfo->tablename=="message")
		{
			ret = oandroid2ios->dealmessagechatthread_newmessage(startint);
		}
		else
		{
			ret = oandroid2ios->dealmessagechatthread(startint);
		}
		if (ret != 1000)
		{
			break;
		}


	}

	oandroid2ios->tostopthread();
	return 0;
}