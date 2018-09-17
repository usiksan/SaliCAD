/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Widget to display help
*/
#include "SdConfig.h"
#include "SdWHelp.h"
#include "library/SvDir.h"

#include <QUrl>
#include <QSettings>
#include <QFile>
#include <QCoreApplication>
#include <QStringList>


//Path where resides help system files
QString SdWHelp::mHelpPath;


SdWHelp::SdWHelp(QWidget *parent) :
  QTextBrowser( parent )
  {

  //Replace anchor clicked
  connect( this, &SdWHelp::anchorClicked, this, [this] ( QUrl url) {
    if( url.hasFragment() )
      setSource( pageConvert( url.fileName(), url.fragment() ) );
    else
      setSource( pageConvert( url.fileName(), QString() ) );
    });
  }



QUrl SdWHelp::pageConvert(const QString &page, const QString &fragment)
  {
  QSettings s;
  //Interface language
  //Язык интерфейса
  QString lang = s.value( SDK_LANGUAGE, QVariant(QString("en")) ).toString();

  //Test if file exist with language lang
  //Проверить наличие файла с языком
  if( QFile::exists( helpPath() + lang + "-" + page ) ) {
    //File exist. Build url
    QUrl url = QUrl::fromLocalFile(helpPath() + lang + "-" + page);
    if( !fragment.isEmpty() )
      url.setFragment( fragment );
    return url;
    }
  //Test if file exist with english language
  //Проверить наличие файла с анлийским языком
  else if( QFile::exists( helpPath() + "en-" + page ) ) {
    //File exist. Build url
    QUrl url = QUrl::fromLocalFile(helpPath() + "en-" + page);
    if( !fragment.isEmpty() )
      url.setFragment( fragment );
    return url;
    }
  //No file exist, return error file
  //Никакого файла нету, выдать файл с ошибкой
  return pageError();
  }





QUrl SdWHelp::pageError()
  {
  QSettings s;
  //Interface language
  //Язык интерфейса
  QString lang = s.value( SDK_LANGUAGE, QVariant(QString("en")) ).toString();
  //Test if exist error page with current language
  //Проверить наличие страницы с ошибкой на языке пользователя
  if( QFile::exists( helpPath() + lang + "-errorNoPage.htm" ) )
    //Page exist, build url and return it
    return QUrl::fromLocalFile( helpPath() + lang + "-errorNoPage.htm" );

  //Return url with english error page
  //Вернуть страницу с ошибкой на английском
  return QUrl::fromLocalFile( helpPath() + "en-errorNoPage.htm" );
  }








//Show help contens
void SdWHelp::contens()
  {
  //Setup home page
  //Выдать начальную страницу
  setSource( pageConvert( "a-head.htm", QString() ) );
  }





//Show help topic
void SdWHelp::helpTopic(const QString topic)
  {
  if( topic.contains(QChar('#')) ) {
    //Topic contains local position
    int i = topic.indexOf( QChar('#') );
    //File part
    QString file = topic.left(i);
    //Local point part
    QString local = topic.mid( i + 1 );
    setSource( pageConvert( file, local ) );
    }
  else
    setSource( pageConvert( topic, QString() ) );
  }





QString SdWHelp::helpPath()
  {
  if( mHelpPath.isEmpty() ) {
    //Help path not assigned yet, build
    QSettings s;
    SvDir def( QCoreApplication::applicationDirPath() );
    //Help system path
    SvDir pth(s.value( SDK_HELP_PATH, QVariant( def.slashedPath() + QString("help/")) ).toString());
    mHelpPath = pth.slashedPath();
    }
  return mHelpPath;
  }



void SdWHelp::setHelpPath(const QString pth)
  {
  SvDir hp(pth);
  mHelpPath = hp.slashedPath();
  QSettings s;
  s.setValue( SDK_HELP_PATH, mHelpPath );
  }
