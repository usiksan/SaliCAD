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
#include "windows/SdDRegistation.h"
#include "objects/SdPulsar.h"
#include "objects/SdEnvir.h"
#include "objects/SdProp.h"
#include "objects/SdObjectFactory.h"
#include "objects/SdObjectNetClient.h"
#include <QApplication>
#include <QSettings>
#include <QTranslator>
#include <QLocale>
#include <QDebug>

int main(int argc, char *argv[])
  {
  //Setups for settings
  QCoreApplication::setOrganizationName("SaliLAB");
  QCoreApplication::setOrganizationDomain("http://salilab.com/");
  QCoreApplication::setApplicationName("SaliCAD");

  //Creating application
  QApplication a(argc, argv);

  //Create remote database client
  sdObjectNetClient = new SdObjectNetClient();

  QSettings s;

  if( !s.contains(SDK_LANGUAGE) ) {
    //Language is not assigned yet, assign it

    //Get system language
    QString lang = QLocale::languageToString( QLocale::system().language() );

    //Assign default language
    s.setValue( SDK_LANGUAGE, lang );
    }


  //Translation system
  QTranslator appTranslator;
  if( appTranslator.load( QString( QCoreApplication::applicationDirPath() + "/lang_%1.qm").arg( s.value( SDK_LANGUAGE, QVariant(QString("English")) ).toString() ) ) )
    a.installTranslator( &appTranslator );


  //Check if registered
  if( !s.contains(SDK_GLOBAL_AUTHOR) ) {
    //Not registered, show register dialog
    SdDRegistation rd;
    if( rd.exec() == 0 )
      return 0;
    }
  //Задать каталог библиотек
  //SgImageProvider::mImageDir = PATH_VISUAL;


  //Creating pulsar for signals distibution
  SdPulsar::pulsar = new SdPulsar();

  sdEnvir = new SdEnvir();
  sdGlobalProp = new SdProp();

  //Load environment
  sdEnvir->loadEnvir();

  //Open library for objectFactory system
  SdObjectFactory::openLibrary();

  //Creating application main window
  SdWMain w( a.arguments() );
  if( s.value(QString(SDK_WMAIN_MAX), QVariant(true)).toBool() )
    w.showMaximized();
  else
    w.show();

  //w.restorePositions();

  int res = a.exec();

  //Save current environment
  sdEnvir->saveEnvir();

  delete sdGlobalProp;
  delete sdEnvir;
  delete SdPulsar::pulsar;
  return res;
  }


