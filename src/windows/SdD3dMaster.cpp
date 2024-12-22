#include "SdD3dMaster.h"
#include "SdWView3d.h"
#include "SdD3dModelProgrammEditor.h"
#include "SdD3dModelMaster.h"
#include "SdDHelp.h"
#include "objects/Sd3dGraphModel.h"
#include "objects/SdCopyMap.h"
#include "script/SdScriptParser3d.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSettings>
#include <QMessageBox>
#include <QDialogButtonBox>

SdD3dMaster::SdD3dMaster(SdPItemPart *part, QWidget *parent) :
  QDialog(parent),
  mPartPtr(part),
  mPreviewPart( new SdPItemPart() )
  {
  //List of available models
  mModelList = Sd3dModelScript::scriptList();

  setWindowTitle( tr("Select master to 3d model creation") );

  QHBoxLayout *hlay = new QHBoxLayout();
  QVBoxLayout *vlay = new QVBoxLayout();
  vlay->addWidget( new QLabel( tr("Select master type") ) );
  mMasterType = new QListWidget();
  vlay->addWidget( mMasterType );

  hlay->addLayout( vlay, 3 );

  vlay = new QVBoxLayout();
  vlay->addWidget( new QLabel( tr("Preview")) );
  vlay->addWidget( mPreview = new SdWView3d( mPreviewPart, this ), 1 );
  hlay->addLayout( vlay, 5 );

  vlay = new QVBoxLayout();
  vlay->addWidget( new QLabel( tr("Description")) );
  vlay->addWidget( mDescription = new QTextEdit() );
  mDescription->setReadOnly(true);
  hlay->addLayout( vlay, 2 );


  vlay = new QVBoxLayout();
  vlay->addLayout( hlay );

  //Buttons at dialog bottom
  QDialogButtonBox *dialogButtonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Help );
  vlay->addWidget( dialogButtonBox );
  connect( dialogButtonBox, &QDialogButtonBox::accepted, this, &SdD3dMaster::accept );
  connect( dialogButtonBox, &QDialogButtonBox::rejected, this, &SdD3dMaster::reject );
  //Help system
  connect( dialogButtonBox, &QDialogButtonBox::helpRequested, this, [this] () { SdDHelp::help( "SdD3dMaster.htm", this ); });


  setLayout( vlay );

  connect( mMasterType, &QListWidget::currentRowChanged, this, &SdD3dMaster::onCurrentRowChanged );
  connect( this, &SdD3dMaster::accepted, this, [this] () {
    int index = mMasterType->currentRow();
    if( index >= 0 ) {
      SdD3dModelMaster dlg( mModelList.at(index).mScript, mPreviewPart, this );
      if( dlg.exec() ) {
        //Building model is successfull. Copy 3d model to the edited part
        mPartPtr->getUndo()->begin( tr("Replace 3d model"), mPartPtr, true );
        //Remove previous 3d objects
        mPartPtr->forEach( dctAll, [this] ( SdObject *obj ) -> bool {
          mPartPtr->deleteChild( obj, mPartPtr->getUndo() );
          return false;
          });
        SdCopyMap map;
        mPreviewPart->forEach( dctAll & (~dct3D), [this, &map] ( SdObject *obj ) -> bool {
          mPartPtr->insertChild( obj->copy( map, false ), mPartPtr->getUndo() );
          return false;
          });
        //Insert new created 3d object
        mPartPtr->model()->scriptSet( dlg.scriptRelease(), mPartPtr->getUndo() );
        //Mark project as dirty
        mPartPtr->setProjectDirtyFlag();
        }
      }
    });

  initializePage();
  mMasterType->setCurrentRow(0);
  }



SdD3dMaster::~SdD3dMaster()
  {
  delete mPreviewPart;
  }




//!
//! \brief onCurrentRowChanged Called on current row in model master list
//! \param row                 Selected row index
//!
void SdD3dMaster::onCurrentRowChanged(int row)
  {
  if( row >= 0 && row < mModelList.count() ) {
    mDescription->setText( mModelList.at(row).mDescription );
    Sd3drModel model;
    SdScriptParser3d parser( nullptr, &model );
    SdScriptProgrammPtr programm = parser.parse3d( mModelList.at(row).mScript, mPreviewPart, &model );
    mPreviewPart->clear();
    programm->execute();
    mPreviewPart->insertChild( new Sd3dGraphModel(model), nullptr );
    mPreview->setItem( mPreviewPart );
    mPreview->fitItem();
    mPreview->update();
    return;
    }
  //Clear description and 3d object preview
  mDescription->clear();
  if( mPreview->item() != nullptr )
    mPreview->item()->clear();
  }




void SdD3dMaster::initializePage()
  {
  mMasterType->clear();
  for( const auto &model : std::as_const(mModelList) ) {
    mMasterType->addItem( model.mName );
    }
  onCurrentRowChanged(0);
  }
