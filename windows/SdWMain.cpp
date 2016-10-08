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
#include <QMessageBox>
#include <QMenuBar>
#include <QEvent>
#include <QCoreApplication>
#include <QTextEdit>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QApplication>
#include <QClipboard>
#include <QToolBar>
#include <QToolButton>

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





SdWEditor *SdWMain::activeEditor()
  {
  return dynamic_cast<SdWEditor*>( mWEditors->currentWidget() );
  }





void SdWMain::cmFileNew()
  {
  //Create new project
  mWProjectList->fileOpen( QString() );
  }






void SdWMain::cmFileOpen()
  {
  QString title = QFileDialog::getOpenFileName(this, tr("Open project file"), QString(), tr("SaliCAD 3D Files (*%1)").arg(SD_BASE_EXTENSION) );

  if( title.isEmpty() ) return;

  mWProjectList->fileOpen( title );
  }




void SdWMain::cmFilePrevious()
  {
  QAction *action = qobject_cast<QAction *>(sender());
  if( action )
    mWProjectList->fileOpen( action->data().toString() );
  }





//Закрыть активный проект
void SdWMain::cmFileClose()
  {
  mWProjectList->cmFileClose();
  }




//Закрыть все открытые проекты
void SdWMain::cmFileCloseAll()
  {
  //Продолжать, пока имеются проекты
  while( activeProject() ) {
    //Если закрытие проекта прервано, то прервать и весь процесс
    if( !mWProjectList->cmFileClose() ) break;
    }
  }





//Сохранить активный проект
void SdWMain::cmFileSave()
  {
  if( activeProject() )
    activeProject()->cmFileSave();
  }




//Сохранить проект под другим именем
void SdWMain::cmFileSaveAs()
  {
  if( activeProject() )
    activeProject()->cmFileSaveAs();
  }




void SdWMain::cmFileSaveAll()
  {
  //Для всех открытых проектов выполнить сохранение
  for( int i = 0; project(i); ++i )
    project(i)->cmFileSave();
  }




//Печать активного редактора
void SdWMain::cmFilePrint()
  {
  if( activeEditor() )
    activeEditor()->cmFilePrint();
  }





//Завершение работы
void SdWMain::cmFileExit()
  {
  close();
  }





//Импорт в активный редактор
void SdWMain::cmFileImport()
  {
  if( activeEditor() )
    activeEditor()->cmFileImport();
  }

//Экспорт из активного редактора
void SdWMain::cmFileExport()
  {
  if( activeEditor() )
    activeEditor()->cmFileExport();
  }

//Создание нового объекта
void SdWMain::cmObjectNew()
  {
  if( activeProject() )
    activeProject()->cmObjectNew();
  }

//Переименовать текущий объект в проекте
void SdWMain::cmObjectRename()
  {
  if( activeProject() )
    activeProject()->cmObjectRename();
  }

//Удалить активный объект в проекте
void SdWMain::cmObjectDelete()
  {
  if( activeProject() )
    activeProject()->cmObjectDelete();
  }

//Копировать активный объект проекта в буфер обмена
void SdWMain::cmObjectCopy()
  {
  if( activeProject() )
    activeProject()->cmObjectCopy();
  }

//Вставить объект в активный проект из буфера обмена
void SdWMain::cmObjectPaste()
  {
  if( activeProject() )
    activeProject()->cmObjectPaste();
  }

//Вырезать активный объект из активного проекта в буфер обмена
void SdWMain::cmObjectCut()
  {
  if( activeProject() )
    activeProject()->cmObjectCut();
  }

//Сортировать объекты в активном проекте
void SdWMain::cmObjectSort()
  {
  if( activeProject() )
    activeProject()->cmObjectSort();
  }

void SdWMain::cmEditUndo()
  {
  if( activeEditor() )
    activeEditor()->cmEditUndo();
  }

void SdWMain::cmEditRedo()
  {
  if( activeEditor() )
    activeEditor()->cmEditRedo();
  }

void SdWMain::cmEditCut()
  {
  if( activeEditor() )
    activeEditor()->cmEditCut();
  }

void SdWMain::cmEditCopy()
  {
  if( activeEditor() )
    activeEditor()->cmEditCopy();
  }

void SdWMain::cmEditPaste()
  {
  if( activeEditor() )
    activeEditor()->cmEditPaste();
  }

void SdWMain::cmEditDelete()
  {
  if( activeEditor() )
    activeEditor()->cmEditDelete();
  }

void SdWMain::cmEditSelectAll()
  {
  if( activeEditor() )
    activeEditor()->cmEditSelectAll();
  }

void SdWMain::cmEditFind()
  {
  if( activeEditor() )
    activeEditor()->cmEditFind();
  }

void SdWMain::cmEditReplace()
  {
  if( activeEditor() )
    activeEditor()->cmEditReplace();
  }

void SdWMain::cmEditProperties()
  {
  if( activeEditor() )
    activeEditor()->cmEditProperties();
  }

void SdWMain::cmViewProject()
  {
//  if( mSplitter->position() > 5 ) {
//    //Скрыть проект
//    mSplitterPos = mSplitter->position();
//    mSplitter->setPosition( 0 );
//    }
//  else {
//    //Показать проект
//    mSplitter->setPosition( mSplitterPos );
//    }
  }

void SdWMain::cmViewFill()
  {
  if( activeEditor() )
    activeEditor()->cmViewFill();
  }

void SdWMain::cmViewNets()
  {
  if( activeEditor() )
    activeEditor()->cmViewNets();
  }

void SdWMain::cmViewGrid()
  {
  if( activeEditor() )
    activeEditor()->cmViewGrid();
  }




void SdWMain::cmViewLayers()
  {
  //if( activeEditor() )
  }




void SdWMain::cmViewZoomIn()
  {
  if( activeEditor() )
    activeEditor()->cmViewZoomIn();
  }

void SdWMain::cmViewZoomOut()
  {
  if( activeEditor() )
    activeEditor()->cmViewZoomOut();
  }

void SdWMain::cmViewArea()
  {
  if( activeEditor() )
    activeEditor()->cmViewArea();
  }

void SdWMain::cmViewMeasurement()
  {
  if( activeEditor() )
    activeEditor()->cmViewMeasurement();
  }

void SdWMain::cmModeLine()
  {
  if( activeEditor() )
    activeEditor()->cmModeLine();
  }

void SdWMain::cmModeRect()
  {
  if( activeEditor() )
    activeEditor()->cmModeRect();
  }

void SdWMain::cmModeFilledRect()
  {
  if( activeEditor() )
    activeEditor()->cmModeFilledRect();
  }

void SdWMain::cmModeRegion()
  {
  if( activeEditor() )
    activeEditor()->cmModeRegion();
  }

void SdWMain::cmModeFilledRegion()
  {
  if( activeEditor() )
    activeEditor()->cmModeFilledRegion();
  }

void SdWMain::cmModeCircle()
  {
  if( activeEditor() )
    activeEditor()->cmModeCircle();
  }

void SdWMain::cmModeFilledCircle()
  {
  if( activeEditor() )
    activeEditor()->cmModeFilledCircle();
  }

void SdWMain::cmModeArc()
  {
  if( activeEditor() )
    activeEditor()->cmModeArc();
  }

void SdWMain::cmModeText()
  {
  if( activeEditor() )
    activeEditor()->cmModeText();
  }

void SdWMain::cmModeField()
  {
  if( activeEditor() )
    activeEditor()->cmModeField();
  }

void SdWMain::cmBall()
  {
  if( activeEditor() )
    activeEditor()->cmBall();
  }

void SdWMain::cmPinWired()
  {
  if( activeEditor() )
    activeEditor()->cmPinWired();
  }

void SdWMain::cmPinFlat()
  {
  if( activeEditor() )
    activeEditor()->cmPinFlat();
  }

void SdWMain::cmBodyCylinder()
  {
  if( activeEditor() )
    activeEditor()->cmBodyCylinder();
  }

void SdWMain::cmBodyBrick()
  {
  if( activeEditor() )
    activeEditor()->cmBodyBrick();
  }

void SdWMain::cmBodyContur()
  {
  if( activeEditor() )
    activeEditor()->cmBodyContur();
  }

void SdWMain::cmModePin()
  {
  if( activeEditor() )
    activeEditor()->cmModePin();
  }

void SdWMain::cmModeReference()
  {
  if( activeEditor() )
    activeEditor()->cmModeReference();
  }

void SdWMain::cmModeOrigin()
  {
  if( activeEditor() )
    activeEditor()->cmModeOrigin();
  }

void SdWMain::cmModeComponent()
  {
  if( activeEditor() )
    activeEditor()->cmModeComponent();
  }

void SdWMain::cmModeNet()
  {
  if( activeEditor() )
    activeEditor()->cmModeNet();
  }

void SdWMain::cmNetSetup()
  {
  if( activeEditor() )
    activeEditor()->cmNetSetup();
  }

void SdWMain::cmModeBus()
  {
  if( activeEditor() )
    activeEditor()->cmModeBus();
  }

void SdWMain::cmModeDisconnect()
  {
  if( activeEditor() )
    activeEditor()->cmModeDisconnect();
  }

void SdWMain::cmModePcbArea()
  {
  if( activeEditor() )
    activeEditor()->cmModePcbArea();
  }

void SdWMain::cmModeLink()
  {
  if( activeEditor() )
    activeEditor()->cmModeLink();
  }

void SdWMain::cmModeNetName()
  {
  if( activeEditor() )
    activeEditor()->cmModeNetName();
  }

void SdWMain::cmModeNetList()
  {
  if( activeEditor() )
    activeEditor()->cmModeNetList();
  }

void SdWMain::cmModePack()
  {
  if( activeEditor() )
    activeEditor()->cmModePack();
  }

void SdWMain::cmPads()
  {
  if( activeEditor() )
    activeEditor()->cmPads();
  }

void SdWMain::cmModeLineSize()
  {
  if( activeEditor() )
    activeEditor()->cmModeLineSize();
  }

void SdWMain::cmModeRadiusSize()
  {
  if( activeEditor() )
    activeEditor()->cmModeRadiusSize();
  }

void SdWMain::cmModeMovePart()
  {
  if( activeEditor() )
    activeEditor()->cmModeMovePart();
  }

void SdWMain::cmModePlace()
  {
  if( activeEditor() )
    activeEditor()->cmModePlace();
  }

void SdWMain::cmModeEditWire()
  {
  if( activeEditor() )
    activeEditor()->cmModeEditWire();
  }

void SdWMain::cmModeWire()
  {
  if( activeEditor() )
    activeEditor()->cmModeWire();
  }

void SdWMain::cmModePolygon()
  {
  if( activeEditor() )
    activeEditor()->cmModePolygon();
  }

void SdWMain::cmModeDeleteWire()
  {
  if( activeEditor() )
    activeEditor()->cmModeDeleteWire();
  }

void SdWMain::cmModePad()
  {
  if( activeEditor() )
    activeEditor()->cmModePad();
  }

void SdWMain::cmOption()
  {

  }

void SdWMain::cmTools()
  {

  }

void SdWMain::cmHelpContens()
  {

  }

void SdWMain::cmHelpIndex()
  {

  }




void SdWMain::cmHelpAbout()
  {
  QMessageBox::about( this, tr("About %1").arg(SALICAD_NAME), tr("Version %1, \nCopyright SaliLAB\n%2").arg(SALICAD_VERSION).arg(SALICAD_AUTHOR));
  }





void SdWMain::cmClipboardChange()
  {
  if( activeEditor() )
    activeEditor()->cmClipboardChange();
  if( activeProject() )
    activeProject()->cmClipboardChange();
  }









WClientBase *SdWMain::activeEditor()
  {
  return 0;
  }

WProjectTree *SdWMain::activeProject()
  {
  return dynamic_cast<WProjectTree*>( mProjects->currentWidget() );
  }

WProjectTree *SdWMain::project(int index)
  {
  return dynamic_cast<WProjectTree*>( mProjects->widget(index) );
  }

void SdWMain::createMenu()
  {

  }

void SdWMain::fileOpen(const QString &fname)
  {

  }

//void SdWMain::destroyProject(SdWProjectTree *prj)
//  {
//  int index = mProjects->indexOf( prj );
//  if( index >= 0 ) {
//    mProjectTitles->removeItem( index );
//    mProjects->removeWidget( prj );
//    delete prj;

//    if( mProjects->count() == 0 ) {
//      //Выключить пункты меню
//      MainMenu::projectState(false);
//      mCloseProject->setEnabled(false);
//      }
//    }
//  else
//    qDebug() << "****** destroyProject";
//  }





//Установить заголовок
void SdWMain::setupTitle()
  {
  //setWindowTitle( QString(SALICAD_NAME SALICAD_VERSION "[%1]").arg( activeProject()-> dsProject->mProjectName) );
  }


