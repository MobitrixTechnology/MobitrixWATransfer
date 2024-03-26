**[English](README.md)** **[Deutsch](README-de.md)** **[Português](README-pt.md)**  **[Nederlands](README-nl.md)**

Un assistente alla migrazione di WhatsApp/WhatsApp Business

1、Converti i dati delle chat di WhatsApp/WhatsApp Business dal formato di backup di Android al formato di iPhone.

2、Converti i dati delle chat di WhatsApp/WhatsApp Business dal backup dell'iPhone al formato dati Android.

Puoi utilizzare iTunes, Mobitrix Toolkit, WA personalizzato di Mobitrix e altri strumenti per eseguire il ripristino direttamente sul tuo nuovo dispositivo.

## Ambiente di compilazione:
- C++
- Visual Studio 2019
##  Dipendenze:

- libplist
- sqlite3

## Utilizzo:
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


## Licenza:
Non è consentito utilizzare questo software o le sue opere derivate a scopi commerciali in alcuna forma. I fini commerciali includono, ma non si limitano a, vendere il software, utilizzare il software per fornire servizi commerciali o integrare il software in prodotti commerciali.
