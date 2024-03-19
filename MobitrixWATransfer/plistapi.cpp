#include "plistapi.h"
#include <plist\plist.h>
#include "publicfun.h"


void i_buffer_write_to_filename(const char* filename, const char* buffer, uint64_t length);
void i_buffer_read_from_filename(const char* filename, char** buffer, uint64_t* length);
int i_plist_read_from_filename(plist_t* plist, const char* filename);
int i_plist_write_to_filename(plist_t plist, const char* filename, enum plist_format_t format);


std::string plistapi::buildManifestfilecolunm(uint64_t Birth, int GroupID, int InodeNumber, uint64_t LastModified, uint64_t LastStatusChange, int Mode, int ProtectionClass, int size, const char* RelativePathstr)
{
	std::string retStr = "";

	plist_t opts = NULL;

	opts = plist_new_dict();

	plist_dict_set_item(opts, "$archiver", plist_new_string("NSKeyedArchiver"));

	plist_t topinfo = plist_new_dict();


	plist_dict_set_item(topinfo, "root", plist_new_uid(1));


	plist_dict_set_item(opts, "$top", topinfo);

	plist_t keys = plist_new_array();

	plist_array_append_item(keys, plist_new_string("$null"));


	plist_t fileinfo = plist_new_dict();


	plist_dict_set_item(fileinfo, "$class", plist_new_uid(3));

	plist_dict_set_item(fileinfo, "Birth", plist_new_uint(Birth));

	plist_dict_set_item(fileinfo, "GroupID", plist_new_uint(GroupID));
	plist_dict_set_item(fileinfo, "InodeNumber", plist_new_uint(InodeNumber));
	plist_dict_set_item(fileinfo, "LastModified", plist_new_uint(LastModified));


	plist_dict_set_item(fileinfo, "LastStatusChange", plist_new_uint(LastStatusChange));
	plist_dict_set_item(fileinfo, "Mode", plist_new_uint(Mode));
	plist_dict_set_item(fileinfo, "ProtectionClass", plist_new_uint(ProtectionClass));


	plist_dict_set_item(fileinfo, "RelativePath", plist_new_uid(2));

	plist_dict_set_item(fileinfo, "Size", plist_new_uint(size));

	plist_dict_set_item(fileinfo, "UserID", plist_new_uint(501));

	plist_array_append_item(keys, fileinfo);

	plist_array_append_item(keys, plist_new_string(RelativePathstr));


	plist_t classinfo = plist_new_dict();
	plist_t classarrs = plist_new_array();
	plist_array_append_item(classarrs, plist_new_string("MBFile"));
	plist_array_append_item(classarrs, plist_new_string("NSObject"));
	plist_dict_set_item(classinfo, "$classes", classarrs);

	plist_dict_set_item(classinfo, "$classname", plist_new_string("MBFile"));
	plist_array_append_item(keys, classinfo);

	plist_dict_set_item(opts, "$objects", keys);
	plist_dict_set_item(opts, "$version", plist_new_uint(100000));


	char* buffer = NULL;
	uint32_t length;


	plist_to_bin(opts, &buffer, &length);
	retStr = bytestohexstring(buffer, length);

	

	free(buffer);



	plist_free(topinfo);


	plist_free(classarrs);
	plist_free(classinfo);


	plist_free(fileinfo);

	plist_free(keys);
	plist_free(opts);



	return retStr;
}

void plistapi::SpliteIosDeviceInfo(const char* msgbuf, std::map<std::string, std::string>& dmap)
{

	std::vector<std::string> vect;

	SplitStoV((char*)msgbuf, strlen(msgbuf), "\n", vect, true);
	for (int m = 0; m < vect.size(); m++)
	{
		Replace(vect[m], "\r", "");

		std::vector<std::string> invect;
		SplitStoV((char*)vect[m].c_str(), vect[m].length(), ": ", invect, true);
		if (invect.size() == 2)
		{
			dmap.insert(make_pair(invect[0], invect[1]));
		}

	}

}

//读取IOS信息
int plistapi::readiosdeviceinfo(const char* deviceid, INFOPLIST& oINFOPLIST)
{

		oINFOPLIST.BuildVersion ="n2521";
		oINFOPLIST.DeviceName ="nophone";
		oINFOPLIST.DisplayName = oINFOPLIST.DeviceName;
		oINFOPLIST.IMEI ="0000000000000000";


		oINFOPLIST.ProductType ="16,5";
		oINFOPLIST.ProductVersion ="15.2";
		std::string tmpguid = generate();
		Replace(tmpguid,"-","");

		makeUpper(tmpguid);
		oINFOPLIST.GUID = tmpguid;
		oINFOPLIST.SerialNumber = "000000000000";
		oINFOPLIST.TargetType = "Device";
		oINFOPLIST.TargetIdentifier = deviceid;
		std::string UniqueIdentofier = std::string(deviceid);
		makeUpper(UniqueIdentofier);
		oINFOPLIST.UniqueIdentofier = UniqueIdentofier;

		oINFOPLIST.sec;
		oINFOPLIST.usec;


		oINFOPLIST.iTunesVersion = "12.7.3";



	return 0;

}
int plistapi::readiosinfoplistfromold(const char* filename, INFOPLIST& oINFOPLIST)
{
	plist_t info_plist = NULL;
	i_plist_read_from_filename(&info_plist, filename);
	if (!info_plist)
	{
		return -1;
	}
	plist_t node_tmp = plist_dict_get_item(info_plist, "Build Version");
	if (node_tmp && (plist_get_node_type(node_tmp) == PLIST_STRING)) {
		char* val=NULL;
		plist_get_string_val(node_tmp, &val);
		if (val)
		{

			oINFOPLIST.BuildVersion = std::string(val);
			free(val);
		}
	}

	node_tmp = plist_dict_get_item(info_plist, "Device Name");
	if (node_tmp && (plist_get_node_type(node_tmp) == PLIST_STRING)) {
		char* val=NULL;
		plist_get_string_val(node_tmp, &val);
		if (val)
		{
			oINFOPLIST.DeviceName = std::string(val);
			oINFOPLIST.DisplayName = std::string(val);
			free(val);
		}
		
	}

	node_tmp = plist_dict_get_item(info_plist, "GUID");
	if (node_tmp && (plist_get_node_type(node_tmp) == PLIST_STRING)) {
		char* val = NULL;
		plist_get_string_val(node_tmp, &val);
		if (val)
		{
			oINFOPLIST.GUID = std::string(val);

			free(val);
		}

	}

	node_tmp = plist_dict_get_item(info_plist, "IMEI");
	if (node_tmp && (plist_get_node_type(node_tmp) == PLIST_STRING)) {
		char* val = NULL;
		plist_get_string_val(node_tmp, &val);
		if (val)
		{
			oINFOPLIST.IMEI = std::string(val);
	
			free(val);
		}

	}

	node_tmp = plist_dict_get_item(info_plist, "Last Backup Date");
	if (node_tmp && (plist_get_node_type(node_tmp) == PLIST_DATE)) {

		plist_get_date_val(node_tmp,& oINFOPLIST.sec, &oINFOPLIST.usec);
		

	}


	node_tmp = plist_dict_get_item(info_plist, "Product Type");
	if (node_tmp && (plist_get_node_type(node_tmp) == PLIST_STRING)) {
		char* val = NULL;
		plist_get_string_val(node_tmp, &val);
		if (val)
		{
			oINFOPLIST.ProductType = std::string(val);

			free(val);
		}

	}


	node_tmp = plist_dict_get_item(info_plist, "Product Version");
	if (node_tmp && (plist_get_node_type(node_tmp) == PLIST_STRING)) {
		char* val = NULL;
		plist_get_string_val(node_tmp, &val);
		if (val)
		{
			oINFOPLIST.ProductVersion = std::string(val);

			free(val);
		}

	}

	node_tmp = plist_dict_get_item(info_plist, "Serial Number");
	if (node_tmp && (plist_get_node_type(node_tmp) == PLIST_STRING)) {
		char* val = NULL;
		plist_get_string_val(node_tmp, &val);
		if (val)
		{
			oINFOPLIST.SerialNumber = std::string(val);

			free(val);
		}

	}


	node_tmp = plist_dict_get_item(info_plist, "Target Identifier");
	if (node_tmp && (plist_get_node_type(node_tmp) == PLIST_STRING)) {
		char* val = NULL;
		plist_get_string_val(node_tmp, &val);
		if (val)
		{
			oINFOPLIST.TargetIdentifier = std::string(val);

			free(val);
		}

	}

	node_tmp = plist_dict_get_item(info_plist, "Target Type");
	if (node_tmp && (plist_get_node_type(node_tmp) == PLIST_STRING)) {
		char* val = NULL;
		plist_get_string_val(node_tmp, &val);
		if (val)
		{
			oINFOPLIST.TargetType = std::string(val);

			free(val);
		}

	}


	node_tmp = plist_dict_get_item(info_plist, "Unique Identifier");
	if (node_tmp && (plist_get_node_type(node_tmp) == PLIST_STRING)) {
		char* val = NULL;
		plist_get_string_val(node_tmp, &val);
		if (val)
		{
			oINFOPLIST.UniqueIdentofier = std::string(val);

			free(val);
		}

	}

	node_tmp = plist_dict_get_item(info_plist, "iTunes Version");
	if (node_tmp && (plist_get_node_type(node_tmp) == PLIST_STRING)) {
		char* val = NULL;
		plist_get_string_val(node_tmp, &val);
		if (val)
		{
			oINFOPLIST.iTunesVersion = std::string(val);

			free(val);
		}

	}
//	plist_free(node_tmp);

	plist_free(info_plist);
	return 0;
//	plist_write_to_filename(plist_t plist, const char* filename, enum plist_format_t format)
}

//无备份
int plistapi::buileinfoplistA(const char* deviceid,const char* dstfilename,const char* meplist,int sec,int usec,int apptype)
{
	mapptype=apptype;
	INFOPLIST oINFOPLIST;
	oINFOPLIST.sec = sec;
	oINFOPLIST.usec = usec;
	readiosdeviceinfo( deviceid,  oINFOPLIST);
	writenewiosinfplist(dstfilename, oINFOPLIST);
	buileManifestplistA(meplist,oINFOPLIST);
	return 0;

}
int plistapi::buileStatusplist(const char* dstfilename, int sec, int usec)
{
	plist_t infoplist = NULL;

	infoplist = plist_new_dict();
	plist_dict_set_item(infoplist, "BackupState", plist_new_string("new"));



	plist_dict_set_item(infoplist, "Date", plist_new_date(sec, usec));

	plist_dict_set_item(infoplist, "IsFullBackup", plist_new_bool(0));
	plist_dict_set_item(infoplist, "SnapshotState", plist_new_string("finished"));
	std::string tmpguid = generate();

	makeUpper(tmpguid);
	plist_dict_set_item(infoplist, "UUID", plist_new_string(tmpguid.c_str()));
	plist_dict_set_item(infoplist, "Version", plist_new_string("3.3"));

	i_plist_write_to_filename(infoplist, dstfilename, PLIST_FORMAT_XML);
	plist_free(infoplist);
	return 0;

}

//含有备份
int plistapi::buileinfoplist(const char* srcfilename,const char* dstfilename, int apptype )
{
	mapptype=apptype;
	INFOPLIST oINFOPLIST;
	readiosinfoplistfromold(srcfilename, oINFOPLIST);
	writenewiosinfplist(dstfilename, oINFOPLIST);
	//plist_write_to_filename(plist_t plist, const char* filename, enum plist_format_t format)

	return 0;
}

int plistapi::writenewiosinfplist(const char* dstfilename, INFOPLIST oINFOPLIST)
{
	plist_t infoplist = NULL;

	infoplist = plist_new_dict();
	plist_dict_set_item(infoplist, "Build Version", plist_new_string(oINFOPLIST.BuildVersion.c_str()));

	plist_dict_set_item(infoplist, "Device Name", plist_new_string(oINFOPLIST.DeviceName.c_str()));
	plist_dict_set_item(infoplist, "Display Name", plist_new_string(oINFOPLIST.DisplayName.c_str()));
	plist_dict_set_item(infoplist, "GUID", plist_new_string(oINFOPLIST.GUID.c_str()));
	plist_dict_set_item(infoplist, "IMEI", plist_new_string(oINFOPLIST.IMEI.c_str()));


	plist_dict_set_item(infoplist, "Last Backup Date", plist_new_date(oINFOPLIST.sec, oINFOPLIST.usec));


	plist_dict_set_item(infoplist, "Product Type", plist_new_string(oINFOPLIST.ProductType.c_str()));

	plist_dict_set_item(infoplist, "Product Version", plist_new_string(oINFOPLIST.ProductVersion.c_str()));
	plist_dict_set_item(infoplist, "Serial Number", plist_new_string(oINFOPLIST.SerialNumber.c_str()));
	plist_dict_set_item(infoplist, "Target Identifier", plist_new_string(oINFOPLIST.TargetIdentifier.c_str()));
	plist_dict_set_item(infoplist, "Target Type", plist_new_string(oINFOPLIST.TargetType.c_str()));
	plist_dict_set_item(infoplist, "Unique Identifier", plist_new_string(oINFOPLIST.UniqueIdentofier.c_str()));
	plist_dict_set_item(infoplist, "iTunes Version", plist_new_string(oINFOPLIST.iTunesVersion.c_str()));

	i_plist_write_to_filename(infoplist, dstfilename, PLIST_FORMAT_XML);
	plist_free(infoplist);
	return 0;
}


int i_plist_read_from_filename(plist_t* plist, const char* filename)
{
	char* buffer = NULL;
	uint64_t length;

	if (!filename)
		return 0;

	i_buffer_read_from_filename(filename, &buffer, &length);

	if (!buffer) {
		return 0;
	}

	if ((length > 8) && (memcmp(buffer, "bplist00", 8) == 0)) {
		plist_from_bin(buffer, length, plist);
	}
	else {
		plist_from_xml(buffer, length, plist);
	}

	free(buffer);

	return 1;
}

void i_buffer_read_from_filename(const char* filename, char** buffer, uint64_t* length)
{
	FILE* f;
	uint64_t size;

	*length = 0;

	f = fopen(filename, "rb");
	if (!f) {
		return;
	}

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	rewind(f);

	if (size == 0) {
		fclose(f);
		return;
	}

	*buffer = (char*)malloc(sizeof(char) * (size + 1));
	if (fread(*buffer, sizeof(char), size, f) != size) {
		fclose(f);
		return;
	}
	fclose(f);

	*length = size;
}

void i_buffer_write_to_filename(const char* filename, const char* buffer, uint64_t length)
{
	FILE* f;

	f = fopen(filename, "wb");
	if (f) {
		fwrite(buffer, sizeof(char), length, f);
		fclose(f);
	}
}

int i_plist_write_to_filename(plist_t plist, const char* filename, enum plist_format_t format)
{
	char* buffer = NULL;
	uint32_t length;

	if (!plist || !filename)
		return 0;

	if (format == PLIST_FORMAT_XML)
		plist_to_xml(plist, &buffer, &length);
	else if (format == PLIST_FORMAT_BINARY)
		plist_to_bin(plist, &buffer, &length);
	else
		return 0;

	i_buffer_write_to_filename(filename, buffer, length);

	free(buffer);

	return 1;
}
int plistapi::buileManifestplistA(const char* dstfilename, INFOPLIST oINFOPLIST)
{
	std::string srcfile = "Manifest_eg.plist";


	plist_t old_Manifes_plist = NULL;
	i_plist_read_from_filename(&old_Manifes_plist, srcfile.c_str());
	if (!old_Manifes_plist)
	{

		return -1;
	}
	plist_t new_Manifes_plist = NULL;

	new_Manifes_plist = plist_new_dict();
	plist_t new_Applications_dic = plist_new_dict();


	plist_t Applications_dic = NULL;
	Applications_dic = plist_dict_get_item(old_Manifes_plist, "Applications");
	if (Applications_dic && (plist_get_node_type(Applications_dic) == PLIST_DICT)) {
		plist_t tmp_dic = NULL;
		tmp_dic = plist_dict_get_item(Applications_dic, "com.apple.mobilesafari");
		if (tmp_dic)
		{
			plist_t n_tmp_dic = plist_copy(tmp_dic);
			plist_dict_set_item(new_Applications_dic, "com.apple.mobilesafari", n_tmp_dic);
			//	 plist_free(tmp_dic);
		}
		//	 tmp_dic = NULL;
		tmp_dic = plist_dict_get_item(Applications_dic, "group.com.apple.notes");
		if (tmp_dic)
		{
			plist_t n_tmp_dic = plist_copy(tmp_dic);
			plist_dict_set_item(new_Applications_dic, "group.com.apple.notes", n_tmp_dic);
			////	 plist_free(tmp_dic);
		}
		if(mapptype==0)
		{ 
			tmp_dic = plist_dict_get_item(Applications_dic, "net.whatsapp.WhatsApp");
			if (tmp_dic)
			{
				plist_t n_tmp_dic = plist_copy(tmp_dic);
				plist_dict_set_item(new_Applications_dic, "net.whatsapp.WhatsApp", n_tmp_dic);
				//	 plist_free(tmp_dic);
			}
			tmp_dic = plist_dict_get_item(Applications_dic, "group.net.whatsapp.WhatsApp.shared");
			if (tmp_dic)
			{
				plist_t n_tmp_dic = plist_copy(tmp_dic);
				plist_dict_set_item(new_Applications_dic, "group.net.whatsapp.WhatsApp.shared", n_tmp_dic);

			}
		}
		else
		{
			tmp_dic = plist_dict_get_item(Applications_dic, "net.whatsapp.WhatsAppSMB");
			if (tmp_dic)
			{
				plist_t n_tmp_dic = plist_copy(tmp_dic);
				plist_dict_set_item(new_Applications_dic, "net.whatsapp.WhatsAppSMB", n_tmp_dic);
				//	 plist_free(tmp_dic);
			}
			tmp_dic = plist_dict_get_item(Applications_dic, "group.net.whatsapp.WhatsAppSMB.shared");
			if (tmp_dic)
			{
				plist_t n_tmp_dic = plist_copy(tmp_dic);
				plist_dict_set_item(new_Applications_dic, "group.net.whatsapp.WhatsAppSMB.shared", n_tmp_dic);

			}

		}

		
	}
	plist_dict_set_item(new_Manifes_plist, "Applications", new_Applications_dic);
	plist_t BackupKeyBag_data = NULL;

	BackupKeyBag_data = plist_dict_get_item(old_Manifes_plist, "BackupKeyBag");
	if (BackupKeyBag_data) {

		plist_t n_tmp_dic = plist_copy(BackupKeyBag_data);
		plist_dict_set_item(new_Manifes_plist, "BackupKeyBag", n_tmp_dic);
	}

	plist_t Date_node = NULL;

	Date_node = plist_dict_get_item(old_Manifes_plist, "Date");
	if (Date_node) {

		plist_t n_tmp_dic = plist_copy(Date_node);
		plist_dict_set_item(new_Manifes_plist, "Date", n_tmp_dic);
	}


	plist_t IsEncrypted_node = NULL;

	IsEncrypted_node = plist_dict_get_item(old_Manifes_plist, "IsEncrypted");
	if (IsEncrypted_node) {


		plist_t n_tmp_dic = plist_copy(IsEncrypted_node);
		plist_dict_set_item(new_Manifes_plist, "IsEncrypted", n_tmp_dic);
		//	plist_free(IsEncrypted_node);
	}

	plist_t Lockdown_node = NULL;

	Lockdown_node = plist_dict_get_item(old_Manifes_plist, "Lockdown");
	if (Lockdown_node) {

		plist_t n_tmp_dic = plist_copy(Lockdown_node);
		plist_dict_set_item(n_tmp_dic, "BuildVersion", plist_new_string(oINFOPLIST.BuildVersion.c_str()));
		plist_dict_set_item(n_tmp_dic, "DeviceName", plist_new_string(oINFOPLIST.DeviceName.c_str()));
		plist_dict_set_item(n_tmp_dic, "ProductType", plist_new_string(oINFOPLIST.ProductType.c_str()));
		plist_dict_set_item(n_tmp_dic, "ProductVersion", plist_new_string(oINFOPLIST.ProductVersion.c_str()));
		plist_dict_set_item(n_tmp_dic, "SerialNumber", plist_new_string(oINFOPLIST.SerialNumber.c_str()));
		plist_dict_set_item(n_tmp_dic, "UniqueIdentifier", plist_new_string(oINFOPLIST.UniqueIdentofier.c_str()));
		plist_dict_set_item(new_Manifes_plist, "Lockdown", n_tmp_dic);
		
	}
	
	plist_t SystemDomainsVersion_node = NULL;

	SystemDomainsVersion_node = plist_dict_get_item(old_Manifes_plist, "SystemDomainsVersion");
	if (SystemDomainsVersion_node) {
		plist_t n_tmp_dic = plist_copy(SystemDomainsVersion_node);

		plist_dict_set_item(new_Manifes_plist, "SystemDomainsVersion", n_tmp_dic);
		//	plist_free(SystemDomainsVersion_node);
	}


	plist_t Version_node = NULL;

	Version_node = plist_dict_get_item(old_Manifes_plist, "Version");
	if (Version_node) {

		plist_t n_tmp_dic = plist_copy(Version_node);
		plist_dict_set_item(new_Manifes_plist, "Version", n_tmp_dic);
		//		plist_free(Version_node);
	}
	plist_t WasPasscodeSet_node = NULL;

	WasPasscodeSet_node = plist_dict_get_item(old_Manifes_plist, "WasPasscodeSet");
	if (WasPasscodeSet_node) {
		plist_t n_tmp_dic = plist_copy(WasPasscodeSet_node);

		plist_dict_set_item(new_Manifes_plist, "WasPasscodeSet", n_tmp_dic);
		//	plist_free(WasPasscodeSet_node);
	}


	i_plist_write_to_filename(new_Manifes_plist, dstfilename, PLIST_FORMAT_BINARY);
	plist_free(new_Applications_dic);
	plist_free(new_Manifes_plist);

	return 0;





}
int plistapi::buileManifestplist(const char* srcfilename, const char* dstfilename)
{

	plist_t old_Manifes_plist = NULL;
	i_plist_read_from_filename(&old_Manifes_plist, srcfilename);
	if (!old_Manifes_plist)
	{

		return -1;
	}
	plist_t new_Manifes_plist = NULL;

	new_Manifes_plist = plist_new_dict();
	plist_t new_Applications_dic = plist_new_dict();


	plist_t Applications_dic = NULL;
	Applications_dic = plist_dict_get_item(old_Manifes_plist, "Applications");
	if (Applications_dic && (plist_get_node_type(Applications_dic) == PLIST_DICT)) {
		plist_t tmp_dic = NULL;
		 tmp_dic=plist_dict_get_item(Applications_dic, "com.apple.mobilesafari");
		 if (tmp_dic)
		 {
			 plist_t n_tmp_dic= plist_copy(tmp_dic);
			 plist_dict_set_item(new_Applications_dic, "com.apple.mobilesafari", n_tmp_dic);
		//	 plist_free(tmp_dic);
		 }
	//	 tmp_dic = NULL;
		 tmp_dic = plist_dict_get_item(Applications_dic, "group.com.apple.notes");
		 if (tmp_dic)
		 {
			 plist_t n_tmp_dic = plist_copy(tmp_dic);
			 plist_dict_set_item(new_Applications_dic, "group.com.apple.notes", n_tmp_dic);
		////	 plist_free(tmp_dic);
		 }
		 if (mapptype == 0)
		 {

			 tmp_dic = plist_dict_get_item(Applications_dic, "net.whatsapp.WhatsApp");
			 if (tmp_dic)
			 {
				 plist_t n_tmp_dic = plist_copy(tmp_dic);
				 plist_dict_set_item(new_Applications_dic, "net.whatsapp.WhatsApp", n_tmp_dic);
				 //	 plist_free(tmp_dic);
			 }
			 tmp_dic = plist_dict_get_item(Applications_dic, "group.net.whatsapp.WhatsApp.shared");
			 if (tmp_dic)
			 {
				 plist_t n_tmp_dic = plist_copy(tmp_dic);
				 plist_dict_set_item(new_Applications_dic, "group.net.whatsapp.WhatsApp.shared", n_tmp_dic);

			 }
		 }
		 else {
		 
			 tmp_dic = plist_dict_get_item(Applications_dic, "net.whatsapp.WhatsAppSMB");
			 if (tmp_dic)
			 {
				 plist_t n_tmp_dic = plist_copy(tmp_dic);
				 plist_dict_set_item(new_Applications_dic, "net.whatsapp.WhatsAppSMB", n_tmp_dic);
				 //	 plist_free(tmp_dic);
			 }
			 tmp_dic = plist_dict_get_item(Applications_dic, "group.net.whatsapp.WhatsAppSMB.shared");
			 if (tmp_dic)
			 {
				 plist_t n_tmp_dic = plist_copy(tmp_dic);
				 plist_dict_set_item(new_Applications_dic, "group.net.whatsapp.WhatsAppSMB.shared", n_tmp_dic);

			 }
		 }
		
	//	 if (tmp_dic)
	//		 plist_free(tmp_dic);
		// plist_free(Applications_dic);

	
	}
	plist_dict_set_item(new_Manifes_plist, "Applications", new_Applications_dic);
	plist_t BackupKeyBag_data = NULL;

	BackupKeyBag_data = plist_dict_get_item(old_Manifes_plist, "BackupKeyBag");
	if (BackupKeyBag_data ) {

		plist_t n_tmp_dic = plist_copy(BackupKeyBag_data);
		plist_dict_set_item(new_Manifes_plist, "BackupKeyBag", n_tmp_dic);
	//	plist_free(BackupKeyBag_data);
	}

	plist_t Date_node = NULL;

	Date_node = plist_dict_get_item(old_Manifes_plist, "Date");
	if (Date_node ) {

		plist_t n_tmp_dic = plist_copy(Date_node);
		plist_dict_set_item(new_Manifes_plist, "Date", n_tmp_dic);
	//	plist_free(Date_node);
	}


	plist_t IsEncrypted_node = NULL;

	IsEncrypted_node = plist_dict_get_item(old_Manifes_plist, "IsEncrypted");
	if (IsEncrypted_node) {


		plist_t n_tmp_dic = plist_copy(IsEncrypted_node);
		plist_dict_set_item(new_Manifes_plist, "IsEncrypted", n_tmp_dic);
	//	plist_free(IsEncrypted_node);
	}

	plist_t Lockdown_node = NULL;

	Lockdown_node = plist_dict_get_item(old_Manifes_plist, "Lockdown");
	if (Lockdown_node) {

		plist_t n_tmp_dic = plist_copy(Lockdown_node);
		plist_dict_set_item(new_Manifes_plist, "Lockdown", n_tmp_dic);
	//	plist_free(Lockdown_node);
	}

	plist_t SystemDomainsVersion_node = NULL;

	SystemDomainsVersion_node = plist_dict_get_item(old_Manifes_plist, "SystemDomainsVersion");
	if (SystemDomainsVersion_node) {
		plist_t n_tmp_dic = plist_copy(SystemDomainsVersion_node);

		plist_dict_set_item(new_Manifes_plist, "SystemDomainsVersion", n_tmp_dic);
	//	plist_free(SystemDomainsVersion_node);
	}


	plist_t Version_node = NULL;

	Version_node = plist_dict_get_item(old_Manifes_plist, "Version");
	if (Version_node) {

		plist_t n_tmp_dic = plist_copy(Version_node);
		plist_dict_set_item(new_Manifes_plist, "Version", n_tmp_dic);
//		plist_free(Version_node);
	}
	plist_t WasPasscodeSet_node = NULL;

	WasPasscodeSet_node = plist_dict_get_item(old_Manifes_plist, "WasPasscodeSet");
	if (WasPasscodeSet_node) {
		plist_t n_tmp_dic = plist_copy(WasPasscodeSet_node);

		plist_dict_set_item(new_Manifes_plist, "WasPasscodeSet", n_tmp_dic);
	//	plist_free(WasPasscodeSet_node);
	}


	i_plist_write_to_filename(new_Manifes_plist, dstfilename, PLIST_FORMAT_BINARY);
	plist_free(new_Applications_dic);
	plist_free(new_Manifes_plist);

	return 0;
	
}

int plistapi::buileManifestplist_test(const char* srcfilename)
{

	plist_t old_Manifes_plist = NULL;
	i_plist_read_from_filename(&old_Manifes_plist, srcfilename);
	if (!old_Manifes_plist)
	{
		return -1;
	}






	plist_t Applications_dic = NULL;
	Applications_dic = plist_dict_get_item(old_Manifes_plist, "Applications");
	if (Applications_dic && (plist_get_node_type(Applications_dic) == PLIST_DICT)) {
		plist_t tmp_dic = NULL;
		tmp_dic = plist_dict_get_item(Applications_dic, "net.whatsapp.WhatsApp");
		if (tmp_dic)
		{

		//	plist_free(tmp_dic);
		}
		tmp_dic = NULL;
		tmp_dic = plist_dict_get_item(Applications_dic, "group.com.apple.notes");
		if (tmp_dic)
		{

			
		////	plist_free(tmp_dic);
		}
		tmp_dic = plist_dict_get_item(Applications_dic, "net.whatsapp.WhatsApp4");
		if (tmp_dic)
		{

			
		//	plist_free(tmp_dic);
		}
		tmp_dic = plist_dict_get_item(Applications_dic, "group.net.whatsapp.WhatsApp.shared");
		if (tmp_dic)
		{

		
			plist_free(tmp_dic);
		}


		plist_free(Applications_dic);


	}
	plist_t BackupKeyBag_data = NULL;

	BackupKeyBag_data = plist_dict_get_item(old_Manifes_plist, "BackupKeyBag");
	if (BackupKeyBag_data) {


	
		plist_free(BackupKeyBag_data);
	}

	plist_t Date_node = NULL;

	Date_node = plist_dict_get_item(old_Manifes_plist, "Date");
	if (Date_node) {


		
		plist_free(Date_node);
	}


	plist_t IsEncrypted_node = NULL;

	IsEncrypted_node = plist_dict_get_item(old_Manifes_plist, "IsEncrypted");
	if (IsEncrypted_node) {


	
		plist_free(IsEncrypted_node);
	}

	plist_t Lockdown_node = NULL;

	Lockdown_node = plist_dict_get_item(old_Manifes_plist, "Lockdown");
	if (Lockdown_node) {


	
		plist_free(Lockdown_node);
	}

	plist_t SystemDomainsVersion_node = NULL;

	SystemDomainsVersion_node = plist_dict_get_item(old_Manifes_plist, "SystemDomainsVersion");
	if (SystemDomainsVersion_node) {


		
		plist_free(SystemDomainsVersion_node);
	}


	plist_t Version_node = NULL;

	Version_node = plist_dict_get_item(old_Manifes_plist, "Version");
	if (Version_node) {


	
		plist_free(Version_node);
	}
	plist_t WasPasscodeSet_node = NULL;

	WasPasscodeSet_node = plist_dict_get_item(old_Manifes_plist, "WasPasscodeSet");
	if (WasPasscodeSet_node) {


		
		plist_free(WasPasscodeSet_node);
	}




	return 0;

}
