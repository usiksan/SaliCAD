﻿/*
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
#include "SdDGuiderPlayer.h"
#include "objects/SdPulsar.h"
#include "objects/SdEnvir.h"
#include "SvLib/SvDir.h"

#include <QUrl>
#include <QSettings>
#include <QFile>
#include <QCoreApplication>
#include <QStringList>
#include <QFileInfo>
#include <QDebug>
#include <QMessageBox>



//Common constructor for help widgets
SdWHelp::SdWHelp() :
  QTextBrowser( nullptr ),
  mMain(nullptr)
  {

  setOpenLinks(false);
  //Replace anchor clicked
  connect( this, &SdWHelp::anchorClicked, this, [this] ( QUrl url) {
    //Test special case for intro page
    //In intro page we can open project, create new project or open previously file
    QString path = url.fileName();
    if( path.endsWith( QStringLiteral(".guide")) ) {
      if( SdDGuiderPlayer::guiderExist( path ) ) {
        //Show guide player dialog
        SdDGuiderPlayer player( path, this );
        player.exec();
        }
      else
        QMessageBox::warning( this, tr("Error!"), tr("Guider file '%1' not exist. Try reinstall SaliCAD").arg(path) );
      }
    else {
      if( url.hasFragment() )
        setSource( pageConvert( url.fileName(), url.fragment() ) );
      else
        setSource( pageConvert( url.fileName(), QString() ) );
      }
    });
  }





//Constructor for intro help page widget
SdWHelp::SdWHelp(SdWMain *main) :
  QTextBrowser( nullptr ),
  mMain(main)
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
      path = url.fileName();
      if( SdDGuiderPlayer::guiderExist( path ) ) {
        //Show guide player dialog
        SdDGuiderPlayer player( path, this );
        player.exec();
        }
      else
        QMessageBox::warning( this, tr("Error!"), tr("Guider file '%1' not exist. Try reinstall SaliCAD").arg(path) );
      }
    else if( mMain != nullptr ) {
      if( url.hasFragment() )
        mMain->cmHelpPage( url.fileName() + QStringLiteral("#") + url.fragment() );
      else
        mMain->cmHelpPage( url.fileName() );
      }
    });
  helpIntro();
  }





QUrl SdWHelp::pageConvert(const QString &page, const QString &fragment)
  {
  //Interface language
  //Язык интерфейса
  QString lang = SdEnvir::languageGet();

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
    //qDebug() << "Help on" << (helpPath() + "en-" + page);
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
  //Interface language
  //Язык интерфейса
  QString lang = SdEnvir::languageGet();
  //Test if exist error page with current language
  //Проверить наличие страницы с ошибкой на языке пользователя
  if( QFile::exists( helpPath() + lang + "-errorNoPage.htm" ) )
    //Page exist, build url and return it
    return QUrl::fromLocalFile( helpPath() + lang + "-errorNoPage.htm" );

  //Return url with english error page
  //Вернуть страницу с ошибкой на английском
  return QUrl::fromLocalFile( helpPath() + "en-errorNoPage.htm" );
  }








//Show help contents
void SdWHelp::contents()
  {
  //Setup home page
  //Выдать начальную страницу
  setSource( pageConvert( "contents.htm", QString() ) );
  }





//Show help topic
void SdWHelp::helpTopic(const QString topic)
  {
  //qDebug() << "help topic" << topic;
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
void SdWHelp::helpIntro()
  {
  //Interface language
  //Язык интерфейса
  QString lang = SdEnvir::languageGet();

  QString fname;
  //Test if file exist with language lang
  //Проверить наличие файла с языком
  if( QFile::exists( helpPath() + lang + "-startup.htm" ) )
    //File exist.
    fname = helpPath() + lang + "-startup.htm";
  //Else use english language file
  //Иначе использовать файла с анлийским языком
  else fname = helpPath() + "en-startup.htm";

  QFile file(fname);
  if( file.open(QIODevice::ReadOnly) ) {
    //Load intro file contents
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
  return QStringLiteral(":/help/");
  }



