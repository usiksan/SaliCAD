/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdWProjectList.h"
#include "SdWCommand.h"
#include "SdWProjectTree.h"
#include <QSettings>
#include <QVBoxLayout>
#include <QMessageBox>


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

  connect( mProjectTitles, SIGNAL(activated(int)), mWProjectStack, SLOT(setCurrentIndex(int)) );
  connect( mCloseProject, SIGNAL(clicked()), this, SLOT(cmFileClose()) );
  }





SdWProjectTree *SdWProjectList::activeProject()
  {
  return dynamic_cast<SdWProjectTree*>( mWProjectStack->currentWidget() );
  }





SdWProjectTree *SdWProjectList::project(int index)
  {
  if( index >= 0 && index < mWProjectStack->count() )
    return dynamic_cast<SdWProjectTree*>( mWProjectStack->widget(index) );
  return 0;
  }




//Close all projects
void SdWProjectList::fileCloseAll()
  {

  }





//File project open
void SdWProjectList::fileOpen(const QString fname)
  {
  //Добавить в список предыдущих файлов
  if( !fname.isEmpty() && fname != SD_DEFAULT_FILE_NAME ) {
    //Если файл имеет неверное расширение, то не открываем
    QSettings settings;
    QStringList files = settings.value(SDK_PREVIOUS_FILES).toStringList();
    files.removeAll(fname);
    files.prepend(fname);
    while( files.size() > PREVIOUS_FILES_COUNT )
      files.removeLast();

    settings.setValue(SDK_PREVIOUS_FILES, files);

    //Обновить меню
    SdWCommand::updatePreviousMenu();
    }

  //Создаем окно проекта
  SdWProjectTree *prj = new SdWProjectTree( fname );

  //Проверяем, создался ли проект
  if( prj->isProjectValid() ) {
    mProjectTitles->addItem( prj->fileName() );
    mWProjectStack->addWidget( prj );

    if( mWProjectStack->count() == 1 ) {
      //Выключить пункты меню
      SdWCommand::projectState(true);
      mCloseProject->setEnabled(true);
      }
    }
  else {
    //Проект не создан
    QMessageBox::warning( this, tr("Warning"), tr("Error on file reading [%1]").arg(fname) );
    }
  //

  }



bool SdWProjectList::cmFileClose()
  {

  }

