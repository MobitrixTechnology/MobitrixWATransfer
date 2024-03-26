**[Deutsch](README-de.md)**  **[Italiano](README-it.md)** **[Nederlands](README-nl.md)**  **[Português](README-pt.md)**

A WhatsApp/WhatsApp Business migration assistant

1. Convert WhatsApp/WhatsApp Business chat data from Android to iPhone backup format.
2. Convert WhatsApp/WhatsApp Business chat data from iPhone backup to Android data format.
  You can use iTunes, Mobitrix Toolkit, Mobitrix's customized WA and other tools to directly restore to your new device.

## Compilation environment:
- C++
- Visual Studio 2019
##  Dependencies:

- libplist
- sqlite3

## Usage:
MobitrixWATransfer.exe 

[Android->IOS]

--androiddbpath msgstore.db File[Must] 

--androidmediapath WhatsApp media path 

--iospath Old data in iPhone 

--datapath Out Path[Must] 

--watype 0:WhatsApp 1:WhatsApp Business[Must]

[IOS->Android]

--iosbackpath[BackUp Path]

--datapath[Out Path] 

--watype[0:WhatsApp 1:WhatsApp Business]

## License:
You may not use this software or its derivative works for commercial purposes in any form. Commercial purposes include, but are not limited to, selling the software, using the software to provide commercial services, or integrating the software into commercial products.

## Website

 **[Mobitrix](https://www.mobitrix.com/whatsapp-transfer-backup-restore/)**
