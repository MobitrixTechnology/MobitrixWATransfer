#include "javaSerialize.h"
#include <stdlib.h>
#include <cstdint>
#include <string.h>
#include <stdio.h>
#include <string>
#include "publicfun.h"


char* javaSerialize::getserilazedata(const char* latitude, const char* longitude, const char* filename, const char* filesize, int& len)
{
	std::string head = "ACED000573720016636F6D2E77686174" \
		"736170702E4D6564696144617461FFF4" \
		"96EDE1A230060200215A00186175746F" \
		"646F776E6C6F61645265747279456E61" \
		"626C65644A0015636163686564446F77" \
		"6E6C6F61646564427974657349000566" \
		"61636558490005666163655949000D66" \
		"61696C4572726F72436F64654A000866" \
		"696C6553697A6549000F666972737453" \
		"63616E4C656E67746849000E67696641" \
		"74747269627574696F6E5A0013686173" \
		"53747265616D696E6753696465636172" \
		"4900066865696768744A00136D656469" \
		"614B657954696D657374616D704D734A" \
		"000870726F67726573735A001373686F" \
		"77446F776E6C6F616465644279746573" \
		"490011737573706963696F7573436F6E" \
		"74656E744600197468756D626E61696C" \
		"4865696768745769647468526174696F" \
		"5A000A7472616E73636F6465645A000B" \
		"7472616E736665727265644A00087472" \
		"696D46726F6D4A00067472696D546F5A" \
		"000B75706C6F61645265747279490005" \
		"77696474685B00096369706865724B65" \
		"797400025B424C000A64697265637450" \
		"6174687400124C6A6176612F6C616E67" \
		"2F537472696E673B4C0008646F6F646C" \
		"65496471007E00024C000466696C6574" \
		"000E4C6A6176612F696F2F46696C653B" \
		"5B001066697273745363616E53696465" \
		"63617271007E00015B0007686D61634B" \
		"657971007E00015B0016696E74657261" \
		"6374697665416E6E6F746174696F6E73" \
		"7400255B4C636F6D2F77686174736170" \
		"702F496E746572616374697665416E6E" \
		"6F746174696F6E3B5B0002697671007E" \
		"00014C000C6D656469614A6F62557569" \
		"6471007E00025B00086D656469614B65" \
		"7971007E00015B00067265664B657971" \
		"007E00014C000975706C6F616455726C" \
		"71007E00027870010000000000000000";
		char* headbuf;
		size_t headlen = hexs2bin(head.c_str(), &headbuf);

	
		int latitudei = atoi(latitude);
		int longitudei = atoi(longitude);
	

		char latitudebuf[4] = {0};
		latitudebuf[0] = latitudei >> 24;
		latitudebuf[1] = latitudei >> 16;
		latitudebuf[2] = latitudei >> 8;
		latitudebuf[3] = latitudei;

		char longitudeibuf[4] = { 0 };
		longitudeibuf[0] = longitudei >> 24;
		longitudeibuf[1] = longitudei >> 16;
		longitudeibuf[2] = longitudei >> 8;
		longitudeibuf[3] = longitudei;

		char failErrorCode[4] = { 0 };


		int64_t fsize = atoll(filesize);
		char fsizearr[8] = {0};
		int wei = 56;
		for (int i = 7; i >= 0; i--)
		{

			long long temp = fsize << wei;
			fsizearr[i] = temp >> 56;//
			wei -= 8;
		}
		
	
		std::string mid = "000000000000000000000000"\
			"00000000000000000000000000000000"\
			"00000000000000000000000100000000"\
			"00000000000000000000000000000000"\
			"007070707372000C6A6176612E696F2E"\
			"46696C65042DA4450E0DE4FF0300014C"\
			"00047061746871007E0002787074";
		char* midbuf;
		size_t midlen = hexs2bin(mid.c_str(), &midbuf);

	

		std::string mfilename = std::string(filename);
		Replace(mfilename,"/","\\");

		int filenamelen=strlen(filename);
		int16_t fnamelen = strlen(filename);
		
		char* fnamebuf=new char[filenamelen+1];
		sprintf(fnamebuf,"%s", mfilename.c_str());

		char fnamelenbuf[2] = {0};
		fnamelenbuf[1] = fnamelen & 0xFF;
		fnamelenbuf[0] = fnamelen >> 8;


		std::string end = "7702005C787070707070707070";
		char* endbuf;
		size_t endlen= hexs2bin(end.c_str(), &endbuf);


		int alllen = 797 + filenamelen;

		char* fbuf = new char[alllen+1];

		int index = 0;
		memcpy(fbuf, headbuf,656);
		index += 656;
		memcpy(fbuf+ index, latitudebuf, 4);
		index += 4;
		memcpy(fbuf + index, longitudeibuf, 4);
		index += 4;
		memcpy(fbuf + index, failErrorCode, 4);
		index += 4;
		memcpy(fbuf + index, fsizearr,8);
		index += 8;

		memcpy(fbuf + index, midbuf, 106);
		index += 106;

		memcpy(fbuf + index, fnamelenbuf, 2);
		index += 2;

		memcpy(fbuf + index, fnamebuf, filenamelen);
		index += filenamelen;

		memcpy(fbuf + index, endbuf, 13);
		index += 13;

		if (index != alllen)
		{

		}
		len = alllen;
		
		free(headbuf);
		free(midbuf);
		free(endbuf);
		delete[] fnamebuf;
		return fbuf;
		
}