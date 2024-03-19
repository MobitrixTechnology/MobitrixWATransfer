
#include <iostream>
#include "android2ios.h"

int main(int argc, char* argv[]) {
    std::string androiddbpath;
    std::string androidmediapath;
    std::string iospath;
    std::string datapath;
    int watype = -1;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--androiddbpath") == 0) {
            if (i + 1 < argc) {
                androiddbpath = argv[++i];
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
    }

    if (androiddbpath.empty() || watype == -1 || datapath.empty()) {
        std::cerr << "Error: Must provide --androiddbpath[msgstore.db File], --datapath[Out Path] and --watype[0:WhatsApp 1:WhatsApp Business] arguments." << std::endl;
        return 1;
    }

    android2ios* oandroid2ios = new android2ios();
    int reti = oandroid2ios->start_work(androiddbpath.c_str(), androidmediapath.c_str(), iospath.c_str(), datapath.c_str(), watype);
    delete oandroid2ios;
}
