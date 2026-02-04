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
#include "objects/SdEnvir.h"
#include "objects/SdProp.h"
#include "library/SdLibraryStorage.h"
//#include "objects/Sd3dModel.h"
#include "import/iges/IgesReader.h"
#include <QApplication>
#include <QSettings>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include <QFile>




int main(int argc, char *argv[])
  {
//  Sd3dModel model;
//  Sd3drFace face = model.faceFlat( model.vertexAppend( QVector3D(0,0,0) ), { 0,10, 30,0, 0,10, 40,0, 0,-10, 30,0, 0,-10 }, 0 );
//  Sd3drFace hole1 = model.faceFlat( model.vertexAppend( QVector3D(2,2,0) ), { 0,1, 1,0, 0,-1 }, 0 );
//  Sd3drFace hole2 = model.faceFlat( model.vertexAppend( QVector3D(2,4,0) ), { 0,1, 1,0, 0,-1 }, 0 );
//  qDebug() << face;
//  //qDebug() << model.faceEqudistanteXY( face, 0.1, QMatrix4x4() );
//  qDebug() << model.faceListHolesXY( face, { hole1, hole2 } );

//  IgesReader reader;
//  reader.scanFile( QStringLiteral("/home/asibilev/work/vrml/val.igs") );
//  reader.parse();

  // SdScanerKiCad reader;
  // reader.readFile( "/home/asibilev/info/kikad/SOT-23_MCC.kicad_mod" );
  // reader.parseTop( new SdProject() );


  //Setups for settings
  QCoreApplication::setOrganizationName("SalixEDA");
  QCoreApplication::setOrganizationDomain("http://salixeda.org/");
  QCoreApplication::setApplicationName("SalixEDA");

  //Creating application
  QApplication a(argc, argv);

  QSettings s;
  if( !s.contains(SDK_LANGUAGE) ) {
    //Language is not assigned yet, assign it

    //Get system language
    QStringList ui = QLocale::system().uiLanguages();
    QString lang;
    if( ui.count() )
      lang = ui.at(0);
    else
      lang = QStringLiteral("en");

    //Assign default language
    s.setValue( SDK_LANGUAGE, lang );
    }


  //Translation system
  QTranslator appTranslator;
  if( appTranslator.load( QString( QCoreApplication::applicationDirPath() + "/lang_%1.qm").arg( s.value( SDK_LANGUAGE ).toString() ) ) )
    a.installTranslator( &appTranslator );


  //Creating pulsar for signals distibution
  SdPulsar::sdPulsar = new SdPulsar();

  sdEnvir = new SdEnvir();  
  //Load environment
  sdEnvir->loadEnvir();
  //Global properties
  sdGlobalProp = new SdProp();
  sdGlobalProp->loadProp();

  //Open library for objectFactory system
  SdLibraryStorage::instance()->libraryInit();


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
  sdGlobalProp->saveProp();

  delete sdGlobalProp;
  delete sdEnvir;
  delete SdPulsar::sdPulsar;

  //Close library and store all changes
  SdLibraryStorage::instance()->libraryComplete();

  return res;
  }

