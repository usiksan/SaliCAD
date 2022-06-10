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
#include "SdWProjectTree.h"
#include "SdWEditor.h"
#include "SdWEditorGraphSymbol.h"
#include "SdWEditorGraphPart.h"
#include "SdWEditorComponent.h"
#include "SdWEditorGraphSheet.h"
#include "SdWEditorGraphPlate.h"
#include "SdWEditorGraphView.h"
#include "SdWEditorHelp.h"
#include "SdWEditorIntro.h"
#include "SdWEditorProject.h"
#include "SdWEditorRich.h"
#include "SdWEditor3d.h"
#include "SdWEditor3dPart.h"
#include "SdWEditor3dPartView.h"
#include "SdWRemoteStatus.h"
#include "SdWCommand.h"
#include "SdWLabel.h"
#include "SdDOptions.h"
#include "SdDRegistation.h"
#include "SdDLayers.h"
#include "SdDProjectStore.h"
#include "SdDGetObject.h"
#include "objects/SdPulsar.h"
#include "objects/SdEnvir.h"
#include "objects/SdObjectFactory.h"
#include "objects/SdObjectNetClient.h"
#include "guider/SdGuiderCapture.h"

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
#include <QMimeData>
#include <QStatusBar>
#include <QDebug>
#include <QJsonDocument>
#include <QDesktopServices>


SdWMain::SdWMain(QStringList args, QWidget *parent) :
  QMainWindow(parent)
  {
  //Setup guider capture object
  mGuiderCapture = new SdGuiderCapture( this, this );

  //Set window icon
  setWindowIcon( QIcon(QStringLiteral(":/pic/iconMain.png")) );

  //Create menu
  SdWCommand::createMenu( this );

  //Create tool bars
  SdWCommand::createToolBars( this );


  SdWCommand::hideEditorContext();

  //At left - project list widget (stacked)
  mWProjectList = new SdWProjectList();
  //At center - editor list widget (tabbed)
  mWEditors     = new QTabWidget();
  mWEditors->setTabsClosable(true);
  connect( mWEditors, &QTabWidget::currentChanged, this, &SdWMain::onActivateEditor );
  connect( mWEditors, &QTabWidget::tabCloseRequested, this, &SdWMain::onCloseEditor );
  //At right - help wizard
  mWHelp        = new SdWHelp();
  mWHelp->setMinimumWidth(200);
  //mWHelp->resize( 300, mWHelp->size().height() );
  connect( SdPulsar::sdPulsar, &SdPulsar::helpTopic, mWHelp, &SdWHelp::helpTopic );
  connect( SdPulsar::sdPulsar, &SdPulsar::helpTopic, this, &SdWMain::cmHelpTopic );

  mWSplitter    = new QSplitter();
  mWSplitter->addWidget( mWProjectList );
  mWSplitter->addWidget( mWEditors );
  mWSplitter->addWidget( mWHelp );
  mWHelp->hide();

  connect( mWProjectList, &SdWProjectList::projectNameChanged, this, &SdWMain::activateProjectName );

  setCentralWidget( mWSplitter );

  //Restore splitter positions
  QSettings s;
  if( s.contains(QString(SDK_MAIN_SPLITTER)) )
    mWSplitter->restoreState( s.value(QString(SDK_MAIN_SPLITTER)).toByteArray() );


  //Create status bar
  QStatusBar *sbar = statusBar();
  sbar->addWidget( mXLabel = new SdWLabel( QString("X:"), QString(), 30 ) );
  sbar->addWidget( mXPos = new SdWLabel( QString("0"), tr("Current cursor position X coord"), 60 ) );
  sbar->addWidget( mYLabel = new SdWLabel( QString("Y:"), QString(), 30 ) );
  sbar->addWidget( mYPos = new SdWLabel( QString("0"), tr("Current cursor position Y coord"), 60 ) );
  //Button for handle enter position
  QPushButton *enterPos = new QPushButton( tr("Enter..."), sbar );
  enterPos->setMaximumWidth( 70 );
  enterPos->setMaximumHeight( 20 );
  enterPos->setToolTip( tr("When press this button, You can enter cursor coord") );
  sbar->addWidget( enterPos );
  connect( enterPos, &QPushButton::pressed, this, &SdWMain::cmEnterPosition );

  //Build trace status
  sbar->addWidget( mTraceStatus = new SdWLabel( QString(), tr("Current trace status: unconnected nets and rules errors count"), 200 ) );
  mTraceStatus->hide();

  sbar->addWidget( mMessage = new SdWLabel( QString(), tr("Short guide to current mode step or other messages"), 100 ), 1 );
  sbar->setSizeGripEnabled(true);

  mCapture = new QToolButton();
  mCapture->setIcon( QIcon(QString(":/pic/iconCaptureOff.png")) );
  mCapture->setToolTip( tr("Show status of guide capture video system: stopped") );
  connect( mCapture, &QToolButton::toggled, this, &SdWMain::cmGuiderCapture );
  sbar->addWidget( mCapture );

  mRemote = new QToolButton();
  if( sdObjectNetClient->isRegistered() ) {
    mRemote->setIcon( QIcon(QString(":/pic/iconRemoteOn.png")) );
    mRemote->setToolTip( tr("Remote repository status: registered and link ok") );
    }
  else {
    mRemote->setIcon( QIcon(QString(":/pic/iconRemoteOff.png")) );
    mRemote->setToolTip( tr("Remote repository status: unregistered or can't connect to repository. Check Help->Registration.") );
    }

  connect( sdObjectNetClient, &SdObjectNetClient::remoteStatus, this, [this] ( SdRemoteStatus st ) {
    if( st == SdRemoteOn ) {
      mRemote->setIcon( QIcon(QString(":/pic/iconRemoteOn.png")) );
      mRemote->setToolTip( tr("Remote repository status: registered and link ok") );
      }
    else if( st == SdRemoteOff ) {
      mRemote->setIcon( QIcon(QString(":/pic/iconRemoteOff.png")) );
      mRemote->setToolTip( tr("Remote repository status: unregistered or can't connect to repository. Check Help->Registration.") );
      }
    else {
      mRemote->setIcon( QIcon(QString(":/pic/iconRemoteSync.png")) );
      mRemote->setToolTip( tr("Remote repository status: sync being processed") );
      }
    } );
  connect( mRemote, &QToolButton::clicked, this, &SdWMain::cmRemoteStatus );
  sbar->addWidget( mRemote );
  //Show status when sync operation happens
  connect( sdObjectNetClient, &SdObjectNetClient::informationAppended, this, [this] ( const QString ) { cmRemoteStatus(); } );

  activateProjectName( nullptr );

  //Open all files from command line
  args.removeFirst(); //First is executable name
  for( const QString &file : args )
    mWProjectList->fileOpen( file );

  //Связать с пульсаром
  connect( SdPulsar::sdPulsar, &SdPulsar::activateItem, this, &SdWMain::onActivateProjectItem );
  connect( SdPulsar::sdPulsar, &SdPulsar::activateItem3d, this, &SdWMain::onActivateProjectItem3d );
  connect( SdPulsar::sdPulsar, &SdPulsar::closeEditView, this, &SdWMain::onCloseEditView );
  connect( SdPulsar::sdPulsar, &SdPulsar::removeItem, this, &SdWMain::onRemoveProjectItem );
  connect( SdPulsar::sdPulsar, &SdPulsar::closeProject, this, &SdWMain::onCloseProject );
  connect( SdPulsar::sdPulsar, &SdPulsar::setStatusLabels, this, &SdWMain::setStatusLabels );
  connect( SdPulsar::sdPulsar, &SdPulsar::setStatusPositions, this, &SdWMain::setStatusPositions );
  connect( SdPulsar::sdPulsar, &SdPulsar::setStatusMessage, this, &SdWMain::setStatusMessage );
  connect( SdPulsar::sdPulsar, &SdPulsar::renameItem, this, &SdWMain::onUpdateItemTitle );
  connect( SdPulsar::sdPulsar, &SdPulsar::browseSheetPart, this, &SdWMain::onBrowseSheetPart );
  connect( SdPulsar::sdPulsar, &SdPulsar::selectedParts, this, &SdWMain::onSelectedParts );
  connect( SdPulsar::sdPulsar, &SdPulsar::renameProject, this, &SdWMain::onRenameProject );
  connect( SdPulsar::sdPulsar, &SdPulsar::activateProject, this, &SdWMain::onActivateProject );
  connect( SdPulsar::sdPulsar, &SdPulsar::projectStatusChanged, this, &SdWMain::activateProjectName );
  connect( SdPulsar::sdPulsar, &SdPulsar::setTracingStatus, this, &SdWMain::setTraceStatus );

  //Clipboard notification
  connect( QGuiApplication::clipboard(), &QClipboard::changed, this, &SdWMain::onClipboardChanged );

  //Show help intro
  cmHelpIntro();

  connect( sdObjectNetClient, &SdObjectNetClient::fileContents, this, &SdWMain::onFileReceived );

  //When start application we check for updating
  QTimer::singleShot( 300, this, [] () {
    //We send request to receiv version info file
    sdObjectNetClient->doFile( QStringLiteral("version") );
    });
  }




void SdWMain::setStatusLabels(const QString xlabel, const QString ylabel)
  {
  mXLabel->setText( xlabel );
  mYLabel->setText( ylabel );
  }



void SdWMain::setStatusPositions(const QString x, const QString y)
  {
  mXPos->setText( x );
  mYPos->setText( y );
  }




void SdWMain::setStatusMessage(const QString msg)
  {
  mMessage->setText( msg );
  }




//Update tracing status information
void SdWMain::setTraceStatus(int unconnected, int errors)
  {
  mTraceStatus->setText( tr("Unconnected: %1  Errors: %2").arg(unconnected).arg(errors) );
  }



//Show or hide tracing status information
void SdWMain::showTrace(bool show)
  {
  if( show ) mTraceStatus->show();
  else       mTraceStatus->hide();
  }




//Activate project with name and editing dirty
//Change title bar
void SdWMain::activateProjectName(SdProject *project)
  {
  if( activeProject() && activeProject()->getProject() == project )
    setWindowTitle( QString( "SaliCAD v%1.%2 [%3%4]").arg(SD_VERSION_MAJOR).arg(SD_VERSION_MINOR).arg( project->isDirty() ? QChar('*') : QChar(' ') ).arg( activeProject()->fileName() ) );
  else
    setWindowTitle( QString( "SaliCAD v%1.%2").arg(SD_VERSION_MAJOR).arg(SD_VERSION_MINOR) );
  //Update state of undo and redo commands
  if( activeProject() )
    activeProject()->cmUndoRedoUpdate();
  }





//This signal send from project tree view
//on it we open appropriate editor
void SdWMain::onActivateProjectItem(SdProjectItem *item)
  {
  if( item == nullptr ) return;
  if( activeEditor() && !activeEditor()->is3d() && activeEditor()->getProjectItem() == item )
    //Active editor already top with this item. Nothing done
    return;

  //Find if item already open
  for( int i = 0; i < mWEditors->count(); i++ ) {
    SdWEditor *editor = getEditor(i);
    if( editor && !editor->is3d() && editor->getProjectItem() == item ) {
      //Item already open, bring it on top
      mWEditors->setCurrentIndex( i );
      return;
      }
    }

  //Item not found, create and insert editor for it
  SdWEditor *editor = nullptr;
  switch( item->getClass() ) {
    case dctSymbol :
      if( item->isEditEnable() )
        editor = new SdWEditorGraphSymbol( dynamic_cast<SdPItemSymbol*>( item ), mWEditors );
      else
        editor = new SdWEditorGraphView( item, mWEditors );
      break;
    case dctPart :
      if( item->isEditEnable() )
        editor = new SdWEditorGraphPart( dynamic_cast<SdPItemPart*>( item ), mWEditors );
      else
        editor = new SdWEditorGraphView( item, mWEditors );
      break;
    case dctComponent :
      editor = new SdWEditorComponent( dynamic_cast<SdPItemComponent*>( item ), mWEditors );
      break;
    case dctSheet :
      if( item->isEditEnable() )
        editor = new SdWEditorGraphSheet( dynamic_cast<SdPItemSheet*>( item ), mWEditors );
      else
        editor = new SdWEditorGraphView( item, mWEditors );
      break;
    case dctPlate :
      if( item->isEditEnable() )
        editor = new SdWEditorGraphPlate( dynamic_cast<SdPItemPlate*>( item ), mWEditors );
      else
        editor = new SdWEditorGraphView( item, mWEditors );
      break;
    case dctRich :
      editor = new SdWEditorRich( dynamic_cast<SdPItemRich*>(item), mWEditors );
      break;
    }

  appendEditor( editor );
  }





//This signal send from project tree view
//when user want to open 3d editor
void SdWMain::onActivateProjectItem3d(SdProjectItem *item)
  {
  if( item == nullptr ) return;
  if( activeEditor() && activeEditor()->is3d() && activeEditor()->getProjectItem() == item )
    //Active editor already top with this item. Nothing done
    return;

  //Find if item already open
  for( int i = 0; i < mWEditors->count(); i++ ) {
    SdWEditor *editor = getEditor(i);
    if( editor && editor->is3d() && editor->getProjectItem() == item ) {
      //Item already open, bring it on top
      mWEditors->setCurrentIndex( i );
      return;
      }
    }

  //Item not found, create and insert editor for it
  SdWEditor *editor = nullptr;
  switch( item->getClass() ) {
    case dctSymbol :
//      if( item->isEditEnable() )
//        editor = new SdWEditorGraphSymbol( dynamic_cast<SdPItemSymbol*>( item ), mWEditors );
//      else
//        editor = new SdWEditorGraphView( item, mWEditors );
      break;
    case dctPart :
      if( item->isEditEnable() )
        editor = new SdWEditor3dPart( dynamic_cast<SdPItemPart*>( item ), mWEditors );
      else
        editor = new SdWEditor3dPartView( dynamic_cast<SdPItemPart*>( item ), mWEditors );
      break;
    case dctSheet :
//      if( item->isEditEnable() )
//        editor = new SdWEditorGraphSheet( dynamic_cast<SdPItemSheet*>( item ), mWEditors );
//      else
//        editor = new SdWEditorGraphView( item, mWEditors );
      break;
    case dctPlate :
      editor = new SdWEditor3d( dynamic_cast<SdPItemPlate*>( item ), mWEditors );
      break;
    }

  appendEditor( editor );
  }










void SdWMain::onCloseEditView(SdProjectItem *item)
  {
  if( item == nullptr ) return;
  //Find if item already open
  for( int i = 0; i < mWEditors->count(); i++ ) {
    SdWEditor *editor = getEditor(i);
    if( editor && editor->getProjectItem() == item ) {
      //Item is open, close editor
      mWEditors->removeTab( i );
      return;
      }
    }
  }




void SdWMain::onUpdateItemTitle(SdProjectItem *item)
  {
  if( item == nullptr ) return;
  //Find if item already open
  for( int i = 0; i < mWEditors->count(); i++ ) {
    SdWEditor *editor = getEditor(i);
    if( editor && editor->getProjectItem() == item ) {
      //Item is open, rename tab and tool tip
      mWEditors->setTabIcon( i, QIcon(item->getIconName()) );
      mWEditors->setTabText( i, editor->getTitle() );
      mWEditors->setTabToolTip( i, editor->getToolTip() );
      return;
      }
    }
  }







void SdWMain::onRemoveProjectItem(SdProjectItem *item)
  {
  if( item == nullptr ) return;
  //Find if item is open
  for( int i = 0; i < mWEditors->count(); i++ ) {
    SdWEditor *editor = getEditor(i);
    if( editor && editor->getProjectItem() == item ) {
      //Item is open, close editor
      mWEditors->removeTab( i );
      return;
      }
    }
  }




void SdWMain::onCloseProject(SdProject *prj)
  {
  if( prj == nullptr ) return;
  //Find if items of this project are open
  for( int i = mWEditors->count() - 1; i >= 0; i-- ) {
    SdWEditor *editor = getEditor(i);
    if( editor && editor->getProject() == prj ) {
      //Item of this project open, close editor
      mWEditors->removeTab( i );
      }
    }
  }




//This signal send from project tree when no item selected
void SdWMain::onActivateProject(SdProject *prj, const QString shortName)
  {
  if( prj == nullptr ) return;
  if( activeEditor() && activeEditor()->getProjectItem() == nullptr && activeEditor()->getProject() == prj )
    //Active editor already top with this item. Nothing done
    return;

  //Find if item already open
  for( int i = 0; i < mWEditors->count(); i++ ) {
    SdWEditor *editor = getEditor(i);
    if( editor && editor->getProjectItem() == nullptr && editor->getProject() == prj ) {
      //Item already open, bring it on top
      mWEditors->setCurrentIndex( i );
      return;
      }
    }

  //Create new project view
  SdWEditor *editor = new SdWEditorProject( prj, shortName );
  appendEditor( editor );
  }




//This signal send from project tree when project renamed
void SdWMain::onRenameProject(SdProject *prj, const QString shortName)
  {
  //Find if there opened project view editor
  for( int i = 0; i < mWEditors->count(); i++ ) {
    SdWEditor *editor = getEditor(i);
    if( editor && editor->getProjectItem() == nullptr && editor->getProject() == prj ) {
      //Project view present - rename
      mWEditors->setTabText( i, shortName );
      return;
      }
    }
  }




void SdWMain::onClipboardChanged(QClipboard::Mode mode)
  {
  if( mode == QClipboard::Clipboard ) {
    if( activeProject() ) activeProject()->cmClipboardChange();
    else SdWCommand::cmObjectPaste->setEnabled( false );
    if( activeEditor() ) activeEditor()->cmClipboardChange();
    else SdWCommand::cmEditPaste->setEnabled(false);
    }
  }





//Browse part in sheet
void SdWMain::onBrowseSheetPart(SdProjectItem *sheet, SdProjectItem *plate)
  {
  //Brings sheet editor on top
  onActivateProjectItem( sheet );
  //set browse mode
  if( activeEditor() )
    activeEditor()->cmModeBrowse( plate );
  }




//Components, selected from sheet
void SdWMain::onSelectedParts(SdProjectItem *plate, QStringList list)
  {
  //Brings plate editor on top
  onActivateProjectItem( plate );
  //Select components
  if( activeEditor() )
    activeEditor()->cmModePartSelect( list );
  }





//Calling when press tab of editor
void SdWMain::onActivateEditor(int index)
  {
  //qDebug() << "Activate editor" << index;
  SdWEditor *editor = getEditor(index);
  SdWCommand::hideEditorContext();
  if( editor ) {
    mWProjectList->onItemActivated( editor->getProjectItem() );
    editor->onActivateEditor();
    showTrace( editor->isTraced() );
    }
  else showTrace( false );

  //For each editors decrement recently index except selected
  for( int i = 0; i < mWEditors->count(); i++ ) {
    SdWEditor *ed = getEditor(i);
    if( ed ) ed->updateRecentlyIndex( ed != editor );
    }

  }




//Calling when press close tab of editor
void SdWMain::onCloseEditor(int index)
  {
  //Remove founded editor
  SdWEditor *toRemove = getEditor(index);
  mWEditors->removeTab(index);
  if( toRemove )
    toRemove->deleteLater();
  }




//On file received from remote repository
void SdWMain::onFileReceived(int result, QString fileName, QByteArray data)
  {
  if( result == 0 ) {
    if( fileName == QStringLiteral("version") ) {
      QJsonObject obj = QJsonDocument::fromJson( data ).object();
#ifdef Q_OS_LINUX
      QJsonObject version = obj.value( QStringLiteral("linux") ).toObject();
#endif
#ifdef Q_OS_WIN
      QJsonObject version = obj.value( QStringLiteral("windows") ).toObject();
#endif
      int major = version.value( QStringLiteral("major") ).toInt();
      int minor = version.value( QStringLiteral("minor") ).toInt();
      if( major > SD_VERSION_MAJOR || (major == SD_VERSION_MAJOR && minor > SD_VERSION_MINOR) ) {
        //Need update
        if( QMessageBox::question( this, tr("Warning!"), tr("Available new version %1.%2! And your version is %3.%4. Update?").arg(major).arg(minor).arg(SD_VERSION_MAJOR).arg(SD_VERSION_MINOR)) == QMessageBox::Yes ) {
          //updating acknowledged
          //start update process
          QDesktopServices::openUrl( QUrl(SD_UPGRADE_WEB) );
          //and exit application
          close();
          }
        }
      }
    //qDebug() << data;
    }
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
  else ev->ignore();
  }





//Insert editor to tab view
void SdWMain::appendEditor(SdWEditor *editor)
  {
  if( editor != nullptr ) {
    if( mWEditors->count() >= SD_MAX_EDITORS ) {
      //Remove last recently used editor
      int minIndex = SD_MAX_EDITORS;
      int minEditor = 0;
      //Find last recently used editor
      for( int i = 0; i < mWEditors->count(); i++ ) {
        SdWEditor *ed = getEditor(i);
        if( ed && ed->getRecentlyIndex() < minIndex ) {
          minIndex = ed->getRecentlyIndex();
          minEditor = i;
          }
        }
      //Remove founded editor
      SdWEditor *toRemove = getEditor(minEditor);
      mWEditors->removeTab(minEditor);
      if( toRemove )
        toRemove->deleteLater();
      }
    //If editor created - insert it into tab
    mWEditors->addTab( editor, QIcon( editor->getIconName() ), editor->getTitle() );

    //Set tool tip
    mWEditors->setTabToolTip( mWEditors->count() - 1, editor->getToolTip() );

    //And make it current
    mWEditors->setCurrentIndex( mWEditors->count() - 1 );
    }
  }




//Get editor by index in tab view
SdWEditor *SdWMain::getEditor(int index)
  {
  if( index < 0 || index >= mWEditors->count() )
    return nullptr;
  return dynamic_cast<SdWEditor*>( mWEditors->widget(index) );
  }





//Return current active editor
SdWEditor *SdWMain::activeEditor()
  {
  return dynamic_cast<SdWEditor*>( mWEditors->currentWidget() );
  }




//Return help widget in editor area. If none it created
SdWEditor *SdWMain::helpWidget()
  {
  //Find help widget
  SdWEditor *help = findHelpWidget();
  if( help == nullptr ) {
    //Help widget not found, create it
    help = new SdWEditorHelp();
    //insert it into tab
    mWEditors->addTab( help, QIcon( QString(":/pic/help.png") ), tr("Help system") );
    //Bring help tab to top
    mWEditors->setCurrentIndex( mWEditors->count() - 1 );
    }
  return help;
  }




//Find help widget in editor area. Return nullptr if none
SdWEditor *SdWMain::findHelpWidget()
  {
  for( int i = 0; i < mWEditors->count(); i++ ) {
    SdWEditorHelp *help = dynamic_cast<SdWEditorHelp*>( getEditor(i) );
    if( help ) {
      //Item already open, bring it on top
      mWEditors->setCurrentIndex( i );
      return help;
      }
    }
  return nullptr;
  }





//Dirty cashe for graphics window and repaint its contents
void SdWMain::activeEditorUpdate()
  {
  if( activeEditor() ) {
    //For graph editor repaint with dirty cashe
    SdWEditorGraph *graph = dynamic_cast<SdWEditorGraph*>(activeEditor());
    if( graph )
      graph->dirtyCashe();
    activeEditor()->update();
    }
  }





void SdWMain::cmFileNew()
  {
  //Create new project
  mWProjectList->fileOpen( QString() );
  }






void SdWMain::cmFileLoad()
  {
  QString uid = SdDGetObject::getObjectUid( dctProject, tr("Select project to load"), this );
  if( !uid.isEmpty() ) {
    SdLibraryHeader hdr;
    if( SdObjectFactory::extractHeader( uid, hdr ) ) {
      SdProject *prj = sdObjectOnly<SdProject>( SdObjectFactory::extractObject( uid, false, this ) );
      if( prj )
        //Open loaded project
        mWProjectList->fileOpen( hdr.mName, prj );
      }
    }
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

  //Hide all bars and commands
  SdWCommand::cmEditUndo->setEnabled(false);
  SdWCommand::cmEditRedo->setEnabled(false);
  SdWCommand::barSymbol->setVisible(false);
  SdWCommand::barPart->setVisible(false);
  SdWCommand::barPart3d->setVisible(false);
  SdWCommand::barComp->setVisible(false);
  SdWCommand::barSheet->setVisible(false);
  SdWCommand::barPcb->setVisible(false);
  }




void SdWMain::cmFileOpenFile(const QString path)
  {
  if( path.isEmpty() ) return;

  mWProjectList->fileOpen( path );
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




//Store current project to library
void SdWMain::cmFileStore()
  {
  SdWProjectTree *tree = activeProject();
  if( tree ) {
    SdProject *project = tree->getProject();
    QString    name    = tree->fileName();
    SdDProjectStore store( name, project, this );
    store.exec();
    }
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




//Загрузить проект PasCAD
void SdWMain::cmFileImportPis()
  {
  QString title = QFileDialog::getOpenFileName(this, tr("Import PasCAD project file"), QString(), tr("PasCAD Files (*%1)").arg(".pis") );

  if( title.isEmpty() ) return;

  mWProjectList->fileImportPis( title );
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




void SdWMain::cmObjectLoad()
  {
  if( activeProject() )
    activeProject()->cmObjectLoad();
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



void SdWMain::cmObjectDuplicate()
  {
  if( activeProject() )
    activeProject()->cmObjectDuplicate();
  }




//Сортировать объекты в активном проекте
void SdWMain::cmObjectSort()
  {
  if( activeProject() )
    activeProject()->cmObjectSort();
  }




void SdWMain::cmObjectParam()
  {
  if( activeProject() )
    activeProject()->cmObjectParam();
  }




void SdWMain::cmObjectEditEnable()
  {
  if( activeEditor() )
    activeEditor()->cmObjectEditEnable();
  }



void SdWMain::cmObjectEditDisable()
  {
  if( activeEditor() )
    activeEditor()->cmObjectEditDisable();
  }






void SdWMain::cmProjectParam()
  {
  if( activeProject() )
    activeProject()->cmProjectParam();
  }





void SdWMain::cmProjectUpgrade()
  {
  if( activeProject() ) {
    //Perform upgrading
    activeProject()->cmProjectUpgrade();
    //Update all editors of project
    activeEditorUpdate();
    }
  }




void SdWMain::cmEditUndo()
  {
  if( activeProject() ) {
    activeProject()->cmEditUndo();
    if( activeEditor() )
      activeEditor()->cmEditUndo();
    //Update state of undo and redo commands
    if( activeProject() )
      activeProject()->cmUndoRedoUpdate();
    }
  }






void SdWMain::cmEditRedo()
  {
  if( activeProject() ) {
    activeProject()->cmEditRedo();
    if( activeEditor() )
      activeEditor()->cmEditRedo();
    //Update state of undo and redo commands
    if( activeProject() )
      activeProject()->cmUndoRedoUpdate();
    }
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




void SdWMain::cmEditUnSelect()
  {
  if( activeEditor() )
    activeEditor()->cmEditUnSelect();
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





void SdWMain::cmEditRotateGroup()
  {
  if( activeEditor() )
    activeEditor()->cmEditRotateGroup();
  }




void SdWMain::cmEditCalculations()
  {
  if( activeEditor() )
    activeEditor()->cmEditCalculations();
  }




void SdWMain::cmEditFragments()
  {
  if( activeEditor() )
    activeEditor()->cmEditFragments();
  }






void SdWMain::cmViewProject()
  {
  if( mWProjectList->isHidden() )
    mWProjectList->show();
  else {
    auto sizes = mWSplitter->sizes();
    if( sizes.at(0) )
      mWProjectList->hide();
    else {
      sizes[0] = 300;
      mWSplitter->setSizes( sizes );
      }
    }
  }



//Switch 3d-2d view of editor
void SdWMain::cmView3d()
  {
  if( activeEditor() ) {
    //If current view is 3d, then open 2d editor
    if( activeEditor()->is3d() )
      onActivateProjectItem( activeEditor()->getProjectItem() );
    else
      onActivateProjectItem3d( activeEditor()->getProjectItem() );
    }
  }




void SdWMain::cmViewFill()
  {
  if( activeEditor() )
    activeEditor()->cmViewFit();
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
  //Layers dialog
  //Get active project or nullptr
  SdProject *prj = nullptr;
  if( activeProject() != nullptr )
    prj = activeProject()->getProject();
  //Get active plate for stratum count definition
  SdPItemPlate *plate = nullptr;
  if( activeEditor() )
    plate = dynamic_cast<SdPItemPlate*>( activeEditor()->getProjectItem() );
  //Show layers dialog with active project
  SdDLayers layersDlg( prj, this );
  //If active plate then set stratum count for edition
  if( plate != nullptr )
    layersDlg.setStratumCount( plate->stratumCount() );

  //Execute layer dialog
  layersDlg.exec();

  //If active plate then get stratum count upgrade
  if( plate != nullptr )
    plate->setStratumCount( layersDlg.getStratumCount() );

  sdEnvir->resetForCache();
  //Signal viewed layers are changed
  SdPulsar::sdPulsar->emitViewedLayers();

  //For active editor update
  if( activeEditor() )
    activeEditor()->cmViewLayers();
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
    activeEditor()->cmViewWindow();
  }




void SdWMain::cmViewMeasurement()
  {
  if( activeEditor() )
    activeEditor()->cmViewMeasurement();
  }





void SdWMain::cmShowFields(bool st)
  {
  sdEnvir->mShowFields = st;
  activeEditorUpdate();
  }







void SdWMain::cmModeSelect()
  {
  if( activeEditor() )
    activeEditor()->cmModeSelect();
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




void SdWMain::cmModeParam()
  {
  if( activeEditor() )
    activeEditor()->cmModeParam();
  }




void SdWMain::cmModeScript()
  {
  if( activeEditor() )
    activeEditor()->cmModeScript();
  }





void SdWMain::cm3dMaster()
  {
  if( activeEditor() )
    activeEditor()->cm3dMaster();
  }



void SdWMain::cm3dProgram()
  {
  if( activeEditor() )
    activeEditor()->cm3dProgram();
  }





void SdWMain::cm3dImportStl()
  {
  if( activeEditor() )
    activeEditor()->cm3dImportStl();
  }



void SdWMain::cm3dImportStep()
  {
  if( activeEditor() )
    activeEditor()->cm3dImportStep();
  }



void SdWMain::cm3dImportVrml()
  {
  if( activeEditor() )
    activeEditor()->cm3dImportVrml();
  }




void SdWMain::cm3dShow2d()
  {
  if( activeEditor() )
    activeEditor()->cm3dShow2d();
  }




void SdWMain::cm3dShowPads()
  {
  if( activeEditor() )
    activeEditor()->cm3dShowPads();
  }




void SdWMain::cm3dModeView()
  {
  if( activeEditor() )
    activeEditor()->cm3dModeView();
  }




void SdWMain::cm3dModeHorzMove()
  {
  if( activeEditor() )
    activeEditor()->cm3dModeHorzMove();
  }




void SdWMain::cm3dModeHorzRotate()
  {
  if( activeEditor() )
    activeEditor()->cm3dModeHorzRotate();
  }




void SdWMain::cm3dModeVertMove()
  {
  if( activeEditor() )
    activeEditor()->cm3dModeVertMove();
  }




void SdWMain::cm3dModeVertRotate()
  {
  if( activeEditor() )
    activeEditor()->cm3dModeVertRotate();
  }




void SdWMain::cm3dModeFaceColor()
  {
  if( activeEditor() )
    activeEditor()->cm3dModeFaceColor();
  }







void SdWMain::cmShowPads(bool st)
  {
  //Setup new state of show pads flag
  sdEnvir->mShowPads = st;
  activeEditorUpdate();
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




void SdWMain::cmModeValue()
  {
  if( activeEditor() )
    activeEditor()->cmModeValue();
  }



void SdWMain::cmModeSymbolFragment()
  {
  if( activeEditor() )
    activeEditor()->cmModeSymbolFragment();
  }




void SdWMain::cmRenumeration()
  {
  if( activeEditor() )
    activeEditor()->cmRenumeration();
  }





void SdWMain::cmModeReferenceMove()
  {
  if( activeEditor() )
    activeEditor()->cmModeReferenceMove();
  }





void SdWMain::cmModeValueMove()
  {
  if( activeEditor() )
    activeEditor()->cmModeValueMove();
  }







void SdWMain::cmModeFragment()
  {
  if( activeEditor() )
    activeEditor()->cmModeFragment();
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




void SdWMain::cmShowRatNet( bool st )
  {
  sdEnvir->mShowRatNet = st;
  if( activeEditor() )
    activeEditor()->update();
  }




void SdWMain::cmShowRuleErrors(bool st)
  {
  sdEnvir->mShowRuleErrors = st;
  if( activeEditor() )
    activeEditor()->update();
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




void SdWMain::cmModePartSelect(QStringList list)
  {
  if( activeEditor() )
    activeEditor()->cmModePartSelect( list );
  }




void SdWMain::cmModePartHighlight()
  {
  if( activeEditor() )
    activeEditor()->cmModePartHighlight();
  }




void SdWMain::cmModeEditWire()
  {
  if( activeEditor() )
    activeEditor()->cmModeEditWire();
  }




void SdWMain::cmModeRoadEnter()
  {
  if( activeEditor() )
    activeEditor()->cmModeRoadEnter();
  }





void SdWMain::cmModeRoadMove()
  {
  if( activeEditor() )
    activeEditor()->cmModeRoadMove();
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





void SdWMain::cmModeViaEnter()
  {
  if( activeEditor() )
    activeEditor()->cmModeViaEnter();
  }




void SdWMain::cmRulesEdit()
  {
  if( activeEditor() )
    activeEditor()->cmRulesEdit();
  }




void SdWMain::cmRulesCheck()
  {
  if( activeEditor() )
    activeEditor()->cmRulesCheck();
  }




void SdWMain::cmRulesErrorNext()
  {
  if( activeEditor() )
    activeEditor()->cmRulesErrorNext();
  }




//Show options dialog
void SdWMain::cmOption()
  {
  SdDOptions(this).exec();
  sdEnvir->resetPads();
  }




void SdWMain::cmTools()
  {

  }




void SdWMain::cmHelpIntro()
  {
  //Create intro editor
  SdWEditorIntro *intro = new SdWEditorIntro(this);
  //insert it into tab
  mWEditors->addTab( intro, QIcon( QString(":/pic/help.png") ), tr("Intro help page") );
  //Bring intro tab to top
  mWEditors->setCurrentIndex( mWEditors->count() - 1 );
  //Show intro contents
  //intro->helpIntro( this );
  }




void SdWMain::cmHelpPage(const QString topic)
  {
  SdWEditorHelp *help = dynamic_cast<SdWEditorHelp*>( helpWidget() );
  help->helpTopic( topic );
  }




void SdWMain::cmHelpContents()
  {
  cmHelpPage( QString("contents.htm") );
  }




void SdWMain::cmHelpIndex()
  {
  cmHelpPage( QString("index.htm") );
  }




void SdWMain::cmHelpAbout()
  {
  QMessageBox::about( this, tr("About %1").arg(SD_NAME), tr("<html><body><p>Version %1.%2, </p>"
                                                            "<p>Copyright SaliLAB</p>"
                                                            "<p>Author: <a href=\"http://salilab.ru\">%3</a></p>"
                                                            "<p>Web:<br><a href=\"http://salicad.salilab.com\">SaliCAD.salilab.com</a><br>"
                                                            "<a href=\"http://salicad.ru\">SaliCAD.ru</a></p></body></html>").
                      arg(SD_VERSION_MAJOR).arg(SD_VERSION_MINOR).arg(SD_AUTHOR) );
  }







void SdWMain::cmHelpRegistration()
  {
  SdDRegistation d;
  d.exec();
  }




void SdWMain::cmGuiderCapture()
  {
  if( mGuiderCapture->isCapture() ) {
    mGuiderCapture->captureStop();
    mCapture->setIcon( QIcon(QString(":/pic/iconCaptureOff.png")) );
    mCapture->setToolTip( tr("Show status of guide capture video system: stopped") );
    }
  else {
    mGuiderCapture->captureInit();
    mCapture->setIcon( QIcon(QString(":/pic/iconCaptureOn.png")) );
    mCapture->setToolTip( tr("Show status of guide capture video system: running") );
    }
  }




void SdWMain::cmGuiderPause()
  {
  if( mGuiderCapture->isPaused() ) {
    mGuiderCapture->captureResume();
    mCapture->setIcon( QIcon(QString(":/pic/iconCaptureOn.png")) );
    mCapture->setToolTip( tr("Show status of guide capture video system: running") );
    }
  else {
    mGuiderCapture->capturePause();
    mCapture->setIcon( QIcon(QString(":/pic/iconCapturePause.png")) );
    mCapture->setToolTip( tr("Show status of guide capture video system: paused") );
    }
  }






void SdWMain::cmHelpBackward()
  {
  SdWEditorHelp *help = dynamic_cast<SdWEditorHelp*>( helpWidget() );
  help->helpBackward();
  }





void SdWMain::cmHelpForward()
  {
  SdWEditorHelp *help = dynamic_cast<SdWEditorHelp*>( helpWidget() );
  help->helpForward();
  }





void SdWMain::cmRemoteStatus()
  {
  if( SdWRemoteStatus::mWRemoteStatus == nullptr ) {
    new SdWRemoteStatus( this );
    SdWRemoteStatus::mWRemoteStatus->resize( 300, 350 );
    SdWRemoteStatus::mWRemoteStatus->move( width() - 220, height() - 400 );
    SdWRemoteStatus::mWRemoteStatus->show();
    }
  }





void SdWMain::cmClipboardChange()
  {
  if( activeEditor() )
    activeEditor()->cmClipboardChange();
  if( activeProject() )
    activeProject()->cmClipboardChange();
  }





void SdWMain::cmPropertiesChange()
  {
  if( activeEditor() )
    activeEditor()->cmPropChanged();
  }






void SdWMain::cmEnterPosition()
  {
  if( activeEditor() )
    activeEditor()->cmEnterPosition();
  }





void SdWMain::cmHelpTopic(const QString topic)
  {
  Q_UNUSED(topic)
  auto sizes = mWSplitter->sizes();
  if( mWHelp->isHidden() || sizes.at(2) == 0 ) {
    mWHelp->show();
    sizes[2] = 300;
    mWSplitter->setSizes( sizes );
    }
  }



