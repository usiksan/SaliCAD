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
#include "SdConfig.h"
#include "SdPNewProjectItem_3dMaster.h"
#include "SdPNewProjectItem.h"
#include "SdWView3d.h"
#include "SdD3dModelProgrammEditor.h"
#include "SdD3dModelMaster.h"
#include "objects/SdObjectFactory.h"
#include "objects/SdPItemRich.h"
#include "objects/Sd3dGraphModel.h"
#include "script/SdScriptParser3d.h"

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
#ifndef SD_DISABLE_SALI_AUTHOR
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
  connect( editProg, &QPushButton::clicked, this, [this] () {
    int row = mMasterType->currentRow();
    if( row >= 0 && row < mIdList.count() ) {
      SdD3dModelProgrammEditor editor( mIdList.at(row), this );
      editor.exec();
      initializePage();
      }
    });
#endif

  boxLay->addStretch(1);

  vlay->addLayout( boxLay );

  hlay->addLayout( vlay, 3 );

  vlay = new QVBoxLayout();
  vlay->addWidget( new QLabel( tr("Preview")) );
  vlay->addWidget( mPreview = new SdWView3d( *item, this ), 1 );
  hlay->addLayout( vlay, 5 );

  vlay = new QVBoxLayout();
  vlay->addWidget( new QLabel( tr("Description")) );
  vlay->addWidget( mDescription = new QTextEdit() );
  mDescription->setReadOnly(true);
  hlay->addLayout( vlay, 2 );



  setLayout( hlay );

  connect( mMasterType, &QListWidget::currentRowChanged, this, &SdPNewProjectItem_3dMaster::onCurrentRowChanged );
  }




bool SdPNewProjectItem_3dMaster::validatePage()
  {
  int index = mMasterType->currentRow();
  if( index >= 0 ) {
    SdPtr<SdPItemPart> part(*mItemPtr);
    if( part.isValid() ) {
      SdD3dModelMaster dlg( mIdList.at(index), part.ptr(), this );
      return dlg.exec() != 0;
      }
    }
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

  mMasterType->clear();
  for( const auto &id : qAsConst(mIdList) ) {
    SdPItemRich *rich = sdObjectOnly<SdPItemRich>( SdObjectFactory::extractObject( id, false, this ) );
    if( rich != nullptr ) {
      mMasterType->addItem( rich->getTitle() );
      delete rich;
      }
    }
  onCurrentRowChanged(0);
  }




//!
//! \brief onCurrentRowChanged Called on current row in model master list
//! \param row                 Selected row index
//!
void SdPNewProjectItem_3dMaster::onCurrentRowChanged(int row)
  {
  if( row >= 0 && row < mIdList.count() ) {
    SdPItemRich *rich = sdObjectOnly<SdPItemRich>( SdObjectFactory::extractObject( mIdList.at(row), false, this ) );
    if( rich != nullptr ) {
      mDescription->setText( rich->paramGet(stdParam3dModelProgramm) );
      Sd3dModel model;
      SdScriptParser3d parser( nullptr, &model );
      static SdPItemPart previewPart;
      auto programm = parser.parse3d( rich->contents(), &previewPart, &model );
      previewPart.clear();
      programm->execute();
      previewPart.insertChild( new Sd3dGraphModel(model), nullptr );
      mPreview->setItem( &previewPart );
      mPreview->fitItem();
      return;
      }
    }
  //Clear description and 3d object preview
  mDescription->clear();
  if( mPreview->item() != nullptr )
    mPreview->item()->clear();
  }
