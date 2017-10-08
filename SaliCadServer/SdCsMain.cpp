/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Entry point.
  Start application, creation server
*/
#include "SdCsConfig.h"
#include "SdCsServer.h"

#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
  {
  //Intro
  qDebug() << "SaliCadServer " SALI_CAD_VERSION;

  QCoreApplication a(argc, argv);

  //Create server. It will receiv all connect requests and
  // create link channel. Link channel is connection element with client.
  new SdCsServer();

  return a.exec();
  }
