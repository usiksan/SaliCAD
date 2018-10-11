/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Open project list widget
*/

#include "SdWProjectList.h"
#include "SdWCommand.h"
#include "SdWProjectTree.h"
#include "objects/SdPulsar.h"
#include "objects/SdPasCadImport.h"
#include <QSettings>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QComboBox>
#include <QDebug>

SdWProjectList::SdWProjectList(QWidget *parent) : QWidget(parent)
  {

  QVBoxLayout *lay = new QVBoxLayout();
  mWProjectStack = new QStackedWidget( this );
  QHBoxLayout *hlay = new QHBoxLayout();
  mProjectTitles = new QComboBox( this );
  mCloseProject  = new QToolButton();
  mCloseProject->setIcon( QIcon(QString(":/pic/closeFile.png")) );
  mCloseProject->setToolTip( tr("Close active project") );
  mCloseProject->setEnabled(false);
  hlay->addWidget( mProjectTitles );
  hlay->addWidget( mCloseProject );
  lay->addLayout(hlay);
  lay->addWidget( mWProjectStack );
  setLayout( lay );

  //connect( mProjectTitles, &QComboBox::activated, this, &SdWProjectList::onProjectActivated );
  connect( mProjectTitles, SIGNAL(activated(int)), this, SLOT(onProjectActivated(int)) );
  connect( mCloseProject, SIGNAL(clicked()), this, SLOT(cmFileClose()) );
  connect( SdPulsar::sdPulsar, &SdPulsar::renameProject, this, &SdWProjectList::onRenameProject );
  connect( SdPulsar::sdPulsar, &SdPulsar::highlightProject, this, &SdWProjectList::onProjectHighlighted );
  }





SdWProjectTree *SdWProjectList::activeProject()
  {
  return dynamic_cast<SdWProjectTree*>( mWProjectStack->currentWidget() );
  }





SdWProjectTree *SdWProjectList::project(int index)
  {
  if( index >= 0 && index < mWProjectStack->count() )
    return dynamic_cast<SdWProjectTree*>( mWProjectStack->widget(index) );
  return nullptr;
  }




//Close all projects
void SdWProjectList::fileCloseAll()
  {
  while( activeProject() )
    if( !cmFileClose() ) break;
  }





//File project open
SdWProjectTree *SdWProjectList::fileOpen(const QString fname, SdProject *project)
  {
  if( project == nullptr )
    //Append file name to previous file list [Добавить в список предыдущих файлов]
    SdWCommand::addToPreviousMenu( fname );

  //Creating project window [Создаем окно проекта]
  SdWProjectTree *prj = new SdWProjectTree( fname, project );

  //Check if project is created (valid) [Проверяем, создался ли проект]
  if( prj->isProjectValid() ) {
    mProjectTitles->addItem( prj->fileName() );
    mWProjectStack->addWidget( prj );

    if( mWProjectStack->count() == 1 ) {
      //Выключить пункты меню
      SdWCommand::projectState(true);
      mCloseProject->setEnabled(true);
      }
    mProjectTitles->setCurrentIndex( mProjectTitles->count() - 1 );
    onProjectActivated( mProjectTitles->count() - 1 );
    }
  else {
    delete prj;
    prj = nullptr;
    //Проект не создан
    QMessageBox::warning( this, tr("Warning"), tr("Error on file reading [%1]").arg(fname) );
    }
  //
  return prj;
  }




//File import PasCAD
void SdWProjectList::fileImportPis(const QString fname)
  {
  //Создаем новый проект
  SdWProjectTree *prj = fileOpen( QString() );

  if( prj != nullptr ) {
    SdPasCadImport importer( prj->getProject(), this );
    importer.import( fname );
    }
  }



bool SdWProjectList::cmFileClose()
  {
  SdWProjectTree *active = activeProject();
  if( active && active->cmFileClose() ) {
    //Remove name from drop down box
    mProjectTitles->removeItem( mWProjectStack->currentIndex() );
    //Remove tree window
    mWProjectStack->removeWidget( active );
    //B035 When close project incorect deleting from comboBox
    //I move signal sending AFTER remove project from visual widgets
    //Send signal before deleting
    SdPulsar::sdPulsar->emitCloseProject( active->getProject() );
    active->deleteLater();
    //Activate other project
    onProjectActivated( mProjectTitles->currentIndex() );
    return true;
    }
  return active == nullptr;
  }




void SdWProjectList::onProjectActivated(int index)
  {
  //Activate project tree
  mWProjectStack->setCurrentIndex( index );
  //New active project
  SdWProjectTree *active = activeProject();
  if( active )
    //Send signal to change project name at programm title
    emit projectNameChanged( active->getProject() );
  else
    emit projectNameChanged( nullptr );
  }




void SdWProjectList::onRenameProject( SdProject *prj, const QString shortName )
  {
  //Rename possible of active project
  SdWProjectTree *active = activeProject();
  if( active && active->getProject() == prj ) {
    //Replace text in combo box
    mProjectTitles->setItemText( mWProjectStack->currentIndex(), shortName );
    //And send signal to change title
    emit projectNameChanged( prj );
    }
  }




//On activate item. We brings item's project up
void SdWProjectList::onItemActivated(SdProjectItem *item)
  {
  if( item == nullptr || activeProject() == nullptr || item->getProject() == activeProject()->getProject() )
    return;

  //Switch to item's project
  for( int i = 0; i < mWProjectStack->count(); i++ ) {
    SdWProjectTree *prj = dynamic_cast<SdWProjectTree*>( mWProjectStack->widget(i) );
    if( prj && prj->getProject() == item->getProject() ) {
      //Project found, bring it up
      mProjectTitles->setCurrentIndex( i );
      onProjectActivated( i );
      return;
      }
    }
  }




//On project highlighted
void SdWProjectList::onProjectHighlighted(SdProject *project)
  {
  if( activeProject() && activeProject()->getProject() == project )
    //Already active project. Nothing done
    return;

  //Switch to project
  for( int i = 0; i < mWProjectStack->count(); i++ ) {
    SdWProjectTree *prj = dynamic_cast<SdWProjectTree*>( mWProjectStack->widget(i) );
    if( prj && prj->getProject() == project ) {
      prj->highlightSheets();
      //Project found, bring it up
      mProjectTitles->setCurrentIndex( i );
      onProjectActivated( i );
      return;
      }
    }
  }

