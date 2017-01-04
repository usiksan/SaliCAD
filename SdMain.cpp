/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Entry point for programm
*/

#include "windows/SdWMain.h"
#include "objects/SdPulsar.h"
#include <QApplication>
#include <QSettings>
#include <QTranslator>

int main(int argc, char *argv[])
  {
  //Setups for settings
  QCoreApplication::setOrganizationName("SaliLAB");
  QCoreApplication::setOrganizationDomain("http://salilab.com/");
  QCoreApplication::setApplicationName("SaliCAD");

  //Creating application
  QApplication a(argc, argv);

  QSettings s;

  //Check if registered
  if( s.value(SDK_GLOBAL_ID_MACHINE).toInt() < 1 ) {
    //Not registered
    s.setValue(SDK_GLOBAL_ID_MACHINE, 0 );
    }
  //Задать каталог библиотек
  //SgImageProvider::mImageDir = PATH_VISUAL;

  //Translation system
  QTranslator appTranslator;
  if( appTranslator.load( QString( QCoreApplication::applicationDirPath() + "/lang_%1.qm").arg( s.value( SDK_LANGUAGE, QVariant(QString("en")) ).toString() ) ) )
    a.installTranslator( &appTranslator );

  //Creating pulsar for signals distibution
  SdPulsar::pulsar = new SdPulsar();

  //Creating application main window
  SdWMain w( a.arguments() );
  if( s.value(QString(SDK_WMAIN_MAX), QVariant(true)).toBool() )
    w.showMaximized();
  else
    w.show();

  //w.restorePositions();

  int res = a.exec();

  delete SdPulsar::pulsar;
  return res;
  }


