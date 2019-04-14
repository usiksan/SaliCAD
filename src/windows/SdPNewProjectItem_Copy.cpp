/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Creation copy of existing object.

  Allow to select object for cloning
*/
#include "SdPNewProjectItem_Copy.h"

#include "SdPNewProjectItem.h"
#include "SdDGetProjectObject.h"
#include "SdDGetObject.h"
#include "objects/SdPItemSymbol.h"
#include "objects/SdPItemPart.h"
#include "objects/SdPItemComponent.h"
#include "objects/SdPItemSheet.h"
#include "objects/SdPItemPlate.h"
#include "objects/SdObjectFactory.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QFileDialog>
#include <QSettings>


SdPNewProjectItem_Copy::SdPNewProjectItem_Copy(SdProjectItemPtr *itemPtr, SdProject *prj, QWidget *parent) :
  QWizardPage( parent ),
  mItem(itemPtr),
  mProject(prj),
  mCopyItem(nullptr),
  mCopyProject(nullptr),
  mCopyOwner(false)
  {
  setTitle( tr("Creation new project object") );
  setSubTitle( tr("Select object to copy new object from") );

  QHBoxLayout *lay = new QHBoxLayout();
  lay->addWidget( new QLabel(tr("Name of object for copy:")) );
  lay->addWidget( mCopyName = new QLineEdit() );

  QVBoxLayout *vlay = new QVBoxLayout();
  vlay->addLayout( lay );

  QPushButton *but;
  vlay->addWidget( but = new QPushButton(tr("Select from current project")) );
  connect( but, &QPushButton::clicked, this, [this] () {
    //Clear copy item and delete all owned copy objects
    clearCopyItem();
    mCopyItem = SdDGetProjectObject::getItem( mProject, (*mItem)->getClass(), tr("Select object for copy from"), this );
    if( mCopyItem ) {
      mCopyProject = mProject;
      mCopyOwner = false;
      }
    //Update copy item information
    copyItemUpdate();
    });

  vlay->addWidget( but = new QPushButton(tr("Select from library")) );
  connect( but, &QPushButton::clicked, this, [this] () {
    //Clear copy item and delete all owned copy objects
    clearCopyItem();
    mCopyItem = sdObjectOnly<SdProjectItem>( SdDGetObject::getObject( (*mItem)->getClass(), tr("Select object for copy from"), this ) );
    if( mCopyItem ) {
      mCopyProject = nullptr;
      mCopyOwner = true;
      }
    //Update copy item information
    copyItemUpdate();
    });

  vlay->addWidget( but = new QPushButton(tr("Select from file")) );
  connect( but, &QPushButton::clicked, this, &SdPNewProjectItem_Copy::onCopyFromFile );

  setLayout( vlay );
  }


void SdPNewProjectItem_Copy::initializePage()
  {
  }



bool SdPNewProjectItem_Copy::validatePage()
  {
  //If copy item present and classes of both items are match then perform clone
  if( mCopyItem && mCopyItem->getClass() == (*mItem)->getClass() ) {
    (*mItem)->cloneFrom( mCopyItem );
    (*mItem)->setEditEnable( true, QString() );
    return true;
    }
  return false;
  }




int SdPNewProjectItem_Copy::nextId() const
  {
  return SDP_NPI_NAME;
  }




//Clear copy item and delete all owned copy objects
void SdPNewProjectItem_Copy::clearCopyItem()
  {
  if( mCopyOwner ) {
    if( mCopyProject ) {
      delete mCopyProject;
      }
    else if( mCopyItem ) {
      delete mCopyItem;
      }
    }
  mCopyProject = nullptr;
  mCopyItem = nullptr;

  mCopyName->clear();
  }




//Update copy item information
void SdPNewProjectItem_Copy::copyItemUpdate()
  {
  if( mCopyItem )
    mCopyName->setText( mCopyItem->getExtendTitle() );
  }




void SdPNewProjectItem_Copy::onCopyFromFile()
  {
  //Clear copy item and delete all owned copy objects
  clearCopyItem();
  QString fname = QFileDialog::getOpenFileName( this, tr("File for loading"), QString(), tr("SaliCAD 3D Files (*%1)").arg(SD_BASE_EXTENSION) );
  if( !fname.isEmpty() ) {
    mCopyProject = SdProject::load( fname );
    mCopyOwner = true;
    if( mCopyProject ) {
      mCopyItem = SdDGetProjectObject::getItem( mCopyProject, (*mItem)->getClass(), tr("Select object for copy from"), this );
      if( mCopyItem == nullptr )
        clearCopyItem();
      }
    }
  //Update copy item information
  copyItemUpdate();
  }

