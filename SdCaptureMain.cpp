/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Entry point for programm SaliCaptureEditor
*/

#include "guider/SdGuiderWMain.h"

#include <QApplication>
#include <QSettings>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include <QFile>


int main(int argc, char *argv[])
  {
  //Setups for settings
  QCoreApplication::setOrganizationName("SaliLAB");
  QCoreApplication::setOrganizationDomain("http://salilab.com/");
  QCoreApplication::setApplicationName("SaliCaptureEditor");

  //Creating application
  QApplication a(argc, argv);


//  QSettings s;

//  if( !s.contains(SDK_LANGUAGE) ) {
//    //Language is not assigned yet, assign it

//    //Get system language
//    QString lang = QLocale::languageToString( QLocale::system().language() );

//    //Assign default language
//    s.setValue( SDK_LANGUAGE, lang );
//    }


  //Translation system
//  QTranslator appTranslator;
//  if( appTranslator.load( QString( QCoreApplication::applicationDirPath() + "/lang_%1.qm").arg( s.value( SDK_LANGUAGE, QVariant(QString("English")) ).toString() ) ) )
//    a.installTranslator( &appTranslator );



  //Creating application main window
  SdGuiderWMain w;
  w.show();

  return a.exec();
  }


