
#include <iostream>
#include "android2ios.h"
#include "waios2android.h"
int main(int argc, char* argv[]) {
    std::string androiddbpath;
    std::string androidmediapath;
    std::string iospath;
    std::string datapath;
    std::string iosbackpath;
    int watype = -1;
    int trantype = 0;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--androiddbpath") == 0) {
            if (i + 1 < argc) {
                androiddbpath = argv[++i];
            }
        }
        else if (strcmp(argv[i], "--iosbackpath") == 0) {
            if (i + 1 < argc) {
                iosbackpath = argv[++i];
            }
        }
        else if (strcmp(argv[i], "--androidmediapath") == 0) {
            if (i + 1 < argc) {
                androidmediapath = argv[++i];
            }
        }
        else if (strcmp(argv[i], "--iospath") == 0) {
            if (i + 1 < argc) {
                iospath = argv[++i];
            }
        }
        else if (strcmp(argv[i], "--datapath") == 0) {
            if (i + 1 < argc) {
                datapath = argv[++i];
            }
        }
        else if (strcmp(argv[i], "--watype") == 0) {
            if (i + 1 < argc) {
                watype = std::atoi(argv[++i]);
            }
        }
        else if (strcmp(argv[i], "--trantype") == 0) {
            if (i + 1 < argc) {
                trantype = std::atoi(argv[++i]);
            }
        }
    }
    if (trantype == 0)
    {
        if (iosbackpath.empty() || watype == -1 || datapath.empty()) {
            std::cerr << "Error: Must provide --androiddbpath[msgstore.db File], --datapath[Out Path] and --watype[0:WhatsApp 1:WhatsApp Business] arguments." << std::endl;
            return 1;
        }

        android2ios* oandroid2ios = new android2ios();
        int reti = oandroid2ios->start_work(androiddbpath.c_str(), androidmediapath.c_str(), iospath.c_str(), datapath.c_str(), watype);
        oandroid2ios->read_work();
        oandroid2ios->write_work();
        delete oandroid2ios;
    }
    else {

        if (androiddbpath.empty() || watype == -1 || datapath.empty()) {
            std::cerr << "Error: Must provide --iosbackpath[BackUp Path], --datapath[Out Path] and --watype[0:WhatsApp 1:WhatsApp Business] arguments." << std::endl;
            return 1;
        }
        waios2android* owaios2android = new waios2android();
        owaios2android->start_work(iosbackpath.c_str(), datapath.c_str(), watype);
        owaios2android->write_work();
        delete owaios2android;
    }

    return 0;
}
