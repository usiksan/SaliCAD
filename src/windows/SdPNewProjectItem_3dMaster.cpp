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
  //List of available models
  mModelList = Sd3dModelScript::scriptList();

  setTitle( tr("Creation new project object") );
  setSubTitle( tr("Select master to object creation") );

  QHBoxLayout *hlay = new QHBoxLayout();
  QVBoxLayout *vlay = new QVBoxLayout();
  vlay->addWidget( new QLabel( tr("Select master type") ) );
  mMasterType = new QListWidget();
  vlay->addWidget( mMasterType );

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
      SdD3dModelMaster dlg( mModelList.at(index).mScript, part.ptr(), this );
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
  mMasterType->clear();
  for( const auto &model : qAsConst(mModelList) ) {
    mMasterType->addItem( model.mName );
    }
  onCurrentRowChanged(0);
  }




//!
//! \brief onCurrentRowChanged Called on current row in model master list
//! \param row                 Selected row index
//!
void SdPNewProjectItem_3dMaster::onCurrentRowChanged(int row)
  {
  if( row >= 0 && row < mModelList.count() ) {
    mDescription->setText( mModelList.at(row).mDescription );
    Sd3drModel model;
    SdScriptParser3d parser( nullptr, &model );
    static SdPItemPart previewPart;
    SdScriptProgrammPtr programm = parser.parse3d( mModelList.at(row).mScript, &previewPart, &model );
    previewPart.clear();
    programm->execute();
    previewPart.insertChild( new Sd3dGraphModel(model), nullptr );
    mPreview->setItem( &previewPart );
    mPreview->fitItem();
    mPreview->update();
    return;
    }
  //Clear description and 3d object preview
  mDescription->clear();
  if( mPreview->item() != nullptr )
    mPreview->item()->clear();
  }
