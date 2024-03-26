#include "analyiosapp.h"
#include "publicfun.h"
#include "BaseLog.h"
#include "mbdb.h"
int analyiosapp::toanalyiosapp(const char* backpath, const char* outpathpath, const char* domain, const char* package,int index,int &num)
{
	std::string maindb = backpath;

#ifdef WIN32
	maindb += "\\Manifest.db";
#else
	maindb += "/Manifest.db";
#endif
	
	if (!IsPathExist(maindb.c_str()))
	{
		return toanalyiosapp_mbdb(backpath, outpathpath, domain, package, index, num);
		

	}
	std::string packagepath = outpathpath;
	packagepath += package;
#ifdef WIN32
	packagepath += "\\";
#else
	packagepath += "/";
#endif
	CreatePath(packagepath.c_str());


	wasqliteparsing* osqliteparser = new wasqliteparsing(maindb.c_str());
	char slqstr[1024];
	int sqllen=sprintf(slqstr,"select fileID,relativePath from Files where domain==\'%s-%s\' and flags==\'1\'", domain, package);
	slqstr[sqllen] = 0;
	std::vector<std::map<std::string, std::string> > vmapstr;


	if (osqliteparser->SqlQuery(slqstr, vmapstr))
	{
		int ynum = 0;
		ynum = num;
		num += vmapstr.size();

		for (int n = 0; n < vmapstr.size(); n++)
		{
	
			std::string fileID = FindDataFromMap(vmapstr.at(n), "fileID");
			std::string relativePath = FindDataFromMap(vmapstr.at(n), "relativePath");
	
#ifdef WIN32
			Replace(relativePath,"/","\\");
#endif
			std::string dstfile = packagepath;
			dstfile += relativePath;


			std::string sonpath =std::string( fileID);
			sonpath=sonpath.substr(0, 2);

			std::string srcfile = backpath;

#ifdef WIN32
			srcfile += "\\";
			srcfile += sonpath;
			srcfile += "\\";
			srcfile += fileID;
#else
			srcfile += "/";
			srcfile += sonpath;
			srcfile += "/";
			srcfile += fileID;
#endif
#ifdef WIN32
			std::string filepath= GetFilePath(dstfile.c_str());
#else
			std::string filepath = GetFilePath_Unxi(dstfile.c_str());
#endif
			CreatePath(filepath.c_str());
			int crte = copyfile(srcfile.c_str(), dstfile.c_str());

	
		}
	}

	delete osqliteparser;
	return 0;
}
int analyiosapp::analyzerwaapp(const char* backuppath, const char* outpath, int apptype, int tol)
{
	int idex = 1;
	if (apptype == 0)
	{

		idex=toanalyiosapp(backuppath, outpath, "AppDomain","net.whatsapp.WhatsApp" , idex, tol);
		idex = toanalyiosapp(backuppath, outpath, "AppDomainGroup", "group.net.whatsapp.WhatsApp.shared", idex, tol);
	}
	else
	{
		idex = toanalyiosapp(backuppath, outpath, "AppDomain", "net.whatsapp.WhatsAppSMB", idex, tol);
		idex = toanalyiosapp(backuppath, outpath, "AppDomainGroup", "group.net.whatsapp.WhatsAppSMB.shared", idex, tol);
	}

	return 0;
}

int analyiosapp::toanalyiosapp_mbdb(const char* backuppath, const char* outpathpath, const char* domain, const char* package, int index, int& num)
{
	if (!package)
		return -1;
	std::string maindb = backuppath;

#ifdef WIN32
	maindb += "\\Manifest.mbdb";
#else
	maindb += "/Manifest.mbdb";
#endif

	if (!IsPathExist(maindb.c_str()))
	{
		return -1;
	}
	std::string packagepath = outpathpath;
	packagepath += package;
#ifdef WIN32
	packagepath += "\\";
#else
	packagepath += "/";
#endif
	int retp = 0;
	CreatePath(packagepath.c_str());
	map<string, string> myfiledb;
	mbdb unit;
	unit.DealMbdb(maindb.c_str(), myfiledb);     //Manifest Manifest.mbdb 
	std::map<string, string>::iterator itt;
	for (itt = myfiledb.begin(); itt != myfiledb.end(); itt++)
	{

		bool needdel = false;

		string sha1 = itt->first;         //
		string sha1key = GetFileName(sha1.c_str());
		string apppaht = itt->second;     //
	


		if (memcmp(apppaht.c_str(), "AppDomain-", 10) == 0|| memcmp(apppaht.c_str(), "AppDomainGroup-", 15) == 0)
		{
			std::string spstr = domain;
			spstr += "-";
			string minappname = GetMidData(apppaht.c_str(), spstr.c_str(), "-");
			bool isforce = false;
			if (std::string(package )== "group.net.whatsapp.WhatsApp.shared" && minappname=="net.whatsapp.WhatsApp-Documents")
			{
				isforce = true;
			
			}
			if (minappname ==std::string( package )|| isforce)
			{
			
				std::string sonpath = std::string(sha1);
				sonpath = sonpath.substr(0, 2);

				std::string srcfile = backuppath;
#ifdef WIN32
				
				srcfile += "\\";
				srcfile += sha1;
#else
				srcfile += "/";
				srcfile += sonpath;
				srcfile += "/";
				srcfile += fileID;
#endif
				std::string dstfile = packagepath;
				char tmpstr[64] = { 0 };
				int strlen = sprintf(tmpstr, "%s%s-", spstr.c_str(), package);
				tmpstr[strlen] = 0;
				if (memcmp(apppaht.c_str(), tmpstr, strlen) == 0)
				{

					char* p = (char*)apppaht.c_str() + strlen;
					dstfile += p;
				}
			
#ifdef WIN32
				Replace(dstfile, "/", "\\");

				std::string filepath = GetFilePath(dstfile.c_str());
#else
				std::string filepath = GetFilePath_Unxi(dstfile.c_str());
#endif
				CreatePath(filepath.c_str());
				copyfile(srcfile.c_str(), dstfile.c_str());

			}
		}
	}
	return retp;
}