/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Listing of creation 3d masters.

  Show list for object type, user select one master and dialog
  execute selected master.
*/
#include "SdPNewProjectItem_3dMaster.h"
#include "SdPNewProjectItem.h"
#include "SdWView3d.h"
#include "SdD3dModelProgrammEditor.h"
#include "objects/SdObjectFactory.h"
#include "objects/SdPItemRich.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSettings>
#include <QMessageBox>

SdPNewProjectItem_3dMaster::SdPNewProjectItem_3dMaster(SdProjectItemPtr *item, SdProject *prj, QWidget *parent) :
  QWizardPage(parent),
  mItemPtr(item),
  mProject(prj)
  {
  setTitle( tr("Creation new project object") );
  setSubTitle( tr("Select master to object creation") );

  QHBoxLayout *hlay = new QHBoxLayout();
  QVBoxLayout *vlay = new QVBoxLayout();
  vlay->addWidget( new QLabel( tr("Select master type") ) );
  mMasterType = new QListWidget();
  vlay->addWidget( mMasterType );
  QHBoxLayout *boxLay = new QHBoxLayout();
  QPushButton *newProg = new QPushButton( tr("New programm") );
  newProg->setToolTip( tr("Open dialog to create new 3d model programm") );
  boxLay->addWidget( newProg );
  connect( newProg, &QPushButton::clicked, this, [this] () {
    SdD3dModelProgrammEditor editor( QString{}, this );
    editor.exec();
    initializePage();
    });

  QPushButton *editProg = new QPushButton( tr("Edit programm") );
  editProg->setToolTip( tr("Open dialog to edit selected 3d model programm") );
  boxLay->addWidget( editProg );

  boxLay->addStretch(1);

  vlay->addLayout( boxLay );

  hlay->addLayout( vlay );

  vlay = new QVBoxLayout();
  vlay->addWidget( mPreview = new SdWView3d( *item, this ) );
  hlay->addLayout( vlay );

  vlay = new QVBoxLayout();
  vlay->addWidget( new QLabel( tr("Description")) );
  vlay->addWidget( mDescription = new QTextEdit() );
  mDescription->setReadOnly(true);
  hlay->addLayout( vlay );



  setLayout( hlay );

  connect( mMasterType, &QListWidget::currentRowChanged, this, [this] ( int row ) {
    if( row >= 0 ) {
      //mDescription->setText( mDescriptions.at(row) );
      //mImage->setPixmap( QPixmap(mImages.at(row)) );
      }
    } );
  }




bool SdPNewProjectItem_3dMaster::validatePage()
  {
  int index = mMasterType->currentRow();
  if( index >= 0 )
    return true;
  return false;
  }




int SdPNewProjectItem_3dMaster::nextId() const
  {
  return SDP_NPI_NAME;
  }




void SdPNewProjectItem_3dMaster::initializePage()
  {
//  int index = mMasterType->currentRow();
  //Fill programm list
  mIdList.clear();
  SdObjectFactory::forEachHeader( [this] ( SdLibraryHeader &hdr ) -> bool {
    if( hdr.mClass == dctRich && hdr.mParamTable.contains( stdParam3dModelProgramm ) ) {
      mIdList.append( hdr.uid() );
      }
    if( mIdList.count() > 100 )
      return true;
    return false;
    });

//  for( auto id : qAsConst(idList) ) {
//    SdPItemRich *rich = sdObjectOnly<SdPItemRich>( SdObjectFactory::extractObject( id, false, this ) );
//    if( rich != nullptr ) {
//      mMasterType->addItem( rich->getTitle() );
//      mDescriptions.append( rich->paramGet( stdParam3dModelProgramm ) );
//      mProgramms.append( rich->contents() );
//      }
//    }

  }
