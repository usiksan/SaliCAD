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
#include "SdWMain.h"
#include "library/SvDir.h"
#include "SdDGuiderPlayer.h"

#include <QUrl>
#include <QSettings>
#include <QFile>
#include <QCoreApplication>
#include <QStringList>
#include <QFileInfo>
#include <QDebug>


//Path where resides help system files
QString SdWHelp::mHelpPath;


SdWHelp::SdWHelp(QWidget *parent) :
  QTextBrowser( parent ),
  mMain(nullptr)
  {

  setOpenLinks(false);
  //Replace anchor clicked
  connect( this, &SdWHelp::anchorClicked, this, [this] ( QUrl url) {
    //Test special case for intro page
    //In intro page we can open project, create new project or open previously file
    QString path = url.toString();
    if( mMain ) {
      qDebug() << "help" << path;
      if( path.startsWith("open:") ) { mMain->cmFileOpen(); return; }
      else if( path.startsWith("new:") ) { mMain->cmFileNew(); return; }
      else if( path.startsWith("load:") ) { mMain->cmFileOpenFile( path.mid(5) ); return; }
      }
    if( path.endsWith( QStringLiteral(".guide")) ) {
      //Show guide player dialog
      SdDGuiderPlayer player( path, this );
      player.exec();
      }
    else {
      if( url.hasFragment() )
        setSource( pageConvert( url.fileName(), url.fragment() ) );
      else
        setSource( pageConvert( url.fileName(), QString() ) );
      }
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
  setSource( pageConvert( "contens.htm", QString() ) );
  }





//Show help topic
void SdWHelp::helpTopic(const QString topic)
  {
  qDebug() << "help topic" << topic;
  if( topic.endsWith( QStringLiteral(".guide")) ) {
    //Show guide player dialog
    SdDGuiderPlayer player( topic, this );
    player.exec();
    }
  else if( topic.contains(QChar('#')) ) {
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





//Show intro topic
//Here we injecting into html page previous file list
void SdWHelp::helpIntro(SdWMain *main)
  {
  mMain = main;

  QSettings s;
  //Interface language
  //Язык интерфейса
  QString lang = s.value( SDK_LANGUAGE, QVariant(QString("en")) ).toString();

  QString fname;
  //Test if file exist with language lang
  //Проверить наличие файла с языком
  if( QFile::exists( helpPath() + lang + "-startup.htm" ) )
    //File exist.
    fname = helpPath() + lang + "-intro.htm";
  //Test if file exist with english language
  //Проверить наличие файла с анлийским языком
  else fname = helpPath() + "en-startup.htm";

  QFile file(fname);
  if( file.open(QIODevice::ReadOnly) ) {
    //Load intro file contens
    QString html = QString::fromUtf8( file.readAll() );

    //Get previously file list
    QSettings settings;
    QStringList files = settings.value(SDK_PREVIOUS_FILES).toStringList();

    //Create html view previously file list
    QString prev;
    for( const QString &str : files ) {
      QFileInfo info(str);
      prev.append("<p><a href=\"load:").append(str).append("\">").append( info.completeBaseName() ).append("</a><br>(").append(str).append(")</p>");
      }

    //Show intro with injected previously file list
    setSearchPaths( {helpPath()} );
    setHtml( html.arg(prev) );
    }
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
