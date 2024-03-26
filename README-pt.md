**[English](README.md)** **[Deutsch](README-de.md)** **[Italiano](README-it.md)** **[Nederlands](README-nl.md)**


Um assistente de migração do WhatsApp/WhatsApp Business

1、Converta dados de bate-papo do WhatsApp/WhatsApp Business do Android para o formato de backup do iPhone.

2、Converta dados de bate-papo do WhatsApp/WhatsApp Business do backup do iPhone para o formato de dados do Android.

Você pode usar o iTunes, o Mobitrix Toolkit, o WA personalizado do Mobitrix e outras ferramentas para restaurar diretamente no seu novo dispositivo.

## Ambiente de compilação:
- C++
- Visual Studio 2019
##  Dependências:

- libplist
- sqlite3
  
## Uso:
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

## Licença:
Você não pode usar este software ou qualquer um dos seus trabalhos derivados para fins comerciais de qualquer forma. Fins comerciais incluem, mas não se limitam a, vender o software, usar o software para fornecer serviços comerciais ou integrar o software em produtos comerciais.
