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

  //Open library for objectFactory system
  SdObjectFactory::openLibrary();

  //Check if registered
  if( !s.contains(SDK_GLOBAL_AUTHOR) || !s.contains(SDK_GLOBAL_PASSWORD) || !s.contains(SDK_SERVER_REPO) ) {
    //Store default ip
    s.setValue( QStringLiteral(SDK_SERVER_REPO), QStringLiteral(SD_DEFAULT_REPO) );
    //Not registered, show register dialog
    SdDRegistation rd(false);
    if( rd.exec() == 0 )
      return 0;
    }


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
  delete SdPulsar::sdPulsar;

  //Close library and store all changes
  SdObjectFactory::closeLibrary();

  return res;
  }


