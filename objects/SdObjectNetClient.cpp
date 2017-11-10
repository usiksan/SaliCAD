/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdObjectNetClient.h"

//Main object for remote database communication
SdObjectNetClient *sdObjectNetClient;


SdObjectNetClient::SdObjectNetClient(QObject *parent) : QObject(parent)
  {

  }
