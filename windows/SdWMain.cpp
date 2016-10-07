/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  SdWMain - Main application window
*/

#include "SdWMain.h"
#include "objects/SdProjectItem.h"
#include <QSettings>
#include <QCloseEvent>

SdWMain::SdWMain(QStringList args, QWidget *parent) :
  QMainWindow(parent)
  {
  mWProjectList = new SdWProjectList();
  mWEditors     = new QTabWidget();

  mWSplitter    = new QSplitter();
  mWSplitter->addWidget( mWProjectList );
  mWSplitter->addWidget( mWEditors );

  setCentralWidget( mWSplitter );

  //Restore splitter positions
  QSettings s;
  if( s.contains(QString(SDK_MAIN_SPLITTER)) )
    mWSplitter->restoreState( s.value(QString(SDK_MAIN_SPLITTER)).toByteArray() );


  //Open all files from command line
  args.removeFirst(); //First is executable name
  for( const QString file : args )
    mWProjectList->fileOpen( file );

  }




//Activate project with name and editing dirty
//Change title bar
void SdWMain::activateProjectName(const QString name, bool dirty)
  {
  setWindowTitle( QString( "SaliCAD v%1.%2 [%3%4]").arg(SD_VERSION_MAJOR).arg(SD_VERSION_MINOR).arg( dirty ? QChar(' ') : QChar('*') ).arg( name ) );
  }





void SdWMain::activateProjectItem(SdProjectItem *item)
  {
  //Find if item already open
  for( int i = 0; i < mWEditors->count(); i++ ) {
    SdWEditor *editor = getEditor(i);
    if( editor && editor->getProjectItem() == item ) {
      //Item already open, bring it on top
      mWEditors->setCurrentIndex( i );
      return;
      }
    }

  //Item not found, create and insert editor for it
  SdWEditor *editor = 0;

  if( editor != 0 )
    mWEditors->addTab( editor, QIcon( editor->getIconName() ), editor->getTitle() );
  }



void SdWMain::closeEvent(QCloseEvent *ev)
  {

  //Try close all files
  mWProjectList->fileCloseAll();

  //If all closed
  if( mWProjectList->isEmpty() ) {
    //Save settings: main window maximisation and splitter position
    QSettings s;
    s.setValue( QString(SDK_WMAIN_MAX), isMaximized() );
    s.setValue( QString(SDK_MAIN_SPLITTER), mWSplitter->saveState() );
    ev->accept();
    }
  }




SdWEditor *SdWMain::getEditor(int index)
  {
  if( index < 0 || index >= mWEditors->count() )
    return 0;
  return dynamic_cast<SdWEditor*>( mWEditors->widget(index) );
  }





SdWEditor *SdWMain::getCurrent()
  {
  return dynamic_cast<SdWEditor*>( mWEditors->currentWidget() );
  }


