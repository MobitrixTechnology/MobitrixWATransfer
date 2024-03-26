#include "mbdb.h"
#include "publicfun.h"

mbdb::mbdb()
{
}


mbdb::~mbdb()
{
}
bool mbdb::getint(const char* data, unsigned int& offset, int datalen, int size, unsigned int& value)
{
    if (data == NULL || offset > datalen - size) return 0;

    value = 0;
    while (size > 0)
    {
        unsigned char tmp = data[offset];
        value <<= 8;
        value |= tmp;
        offset++; size--;
    }
    //	printf("value==%u\n",value);
    return true;
}

bool mbdb::getstring(const char* data, unsigned int& offset, int datalen, string& outdata)
{

    outdata = "";
    if (data == NULL || offset > datalen - 2)
    {

        return false;
    }


    if (data[offset] == (char)0xff && data[offset + 1] == (char)0xff)
    {
        offset += 2;

        return true;
    }
    char* str = NULL;
    unsigned int  slen = 0;
    bool flg = getint(data, offset, datalen, 2, slen);   //
    unsigned int addnum = 0;
    if (flg)
    {
        if (offset + slen <= datalen)
        {

            str = new char[slen + 1];   //
            memcpy(str, (void*)(data + offset), slen);//
            str[slen] = 0; //
            offset += slen;
        
            outdata = str;
            delete str;

        }
        else
        {
      
            flg = false;
        }




    }
    offset += addnum;

    return flg;
}
void mbdb::DealMbdb(const char* filename, map<string, string>& db)
{
    unsigned int filelen;

    char* buf = CReadFromFile(filename, filelen);
    if (buf)
    {
        if (memcmp(buf, "mbdb", 4) == 0 && filelen > 6)
        {
            unsigned int    offset = 0;
            char* index = NULL;
            index = buf + 6;     
            filelen -= 6;
            offset = 0;

            while (offset < filelen)
            {
                fileinfo fi;
                bool flg = false;
                flg = getstring(index, offset, filelen, fi.domain);

                if (!flg)
                {
                    break;
                }

                flg = getstring(index, offset, filelen, fi.filename);

                if (!flg)
                {
                    printf("1\n");
                    break;
                }
                flg = getstring(index, offset, filelen, fi.linktarget);
                if (!flg)
                {
                    printf("2\n");
                    break;
                }

                flg = getstring(index, offset, filelen, fi.datahash);

                if (!flg)
                {
                    break;
                }
                flg = getstring(index, offset, filelen, fi.unknown1);
                if (!flg)
                {
                    printf("4\n");
                    break;
                }

                flg = getint(index, offset, filelen, 2, fi.mode);
                if (!flg)
                {
                    printf("5\n");
                    break;
                }


                flg = getint(index, offset, filelen, 4, fi.unknown2);
                if (!flg)
                {
                    printf("6\n");
                    break;
                }
                flg = getint(index, offset, filelen, 4, fi.inode);
                if (!flg)
                {
                    printf("7\n");
                    break;
                }
                flg = getint(index, offset, filelen, 4, fi.userid);
                if (!flg)
                {
                    break;
                }
                flg = getint(index, offset, filelen, 4, fi.groupid);
                if (!flg)
                {
                    break;
                }
                flg = getint(index, offset, filelen, 4, fi.mtime);
                if (!flg)
                {
                    break;
                }
                flg = getint(index, offset, filelen, 4, fi.atime);
                if (!flg)
                {
                    break;
                }
                flg = getint(index, offset, filelen, 4, fi.ctime);
                if (!flg)
                {
                    break;
                }
                flg = getint(index, offset, filelen, 8, fi.filelen);  //64
                if (!flg)
                {
                    break;
                }
                flg = getint(index, offset, filelen, 1, fi.flag);  //
                if (!flg)
                {
                    break;
                }

                flg = getint(index, offset, filelen, 1, fi.numprops);  
                if (!flg)
                {
                    break;
                }


                for (int m = 0; m < fi.numprops; m++)
                {
                    string str;
                    flg = getstring(index, offset, filelen, str);
                    if (!flg)
                    {
                        break;
                    }
                    flg = getstring(index, offset, filelen, str);
                    if (!flg)
                    {
                        break;
                    }
                }
                if (!flg)
                {
                    break;
                }
                string cmd = fi.domain;
                cmd += "-";
                cmd += fi.filename;

        
                char hash[48] = { 0 };
                SHA1Context sha;
                SHA1Reset(&sha);
                SHA1Input(&sha, (const unsigned char*)cmd.c_str(), cmd.size());

                if (SHA1Result(&sha))
                {

                    sprintf(hash, "%08x%08x%08x%08x%08x", sha.Message_Digest[0], sha.Message_Digest[1], sha.Message_Digest[2], sha.Message_Digest[3], sha.Message_Digest[4]);
                    db.insert(make_pair(hash, cmd));

                }

            }



        }

        delete buf;
    }

}




