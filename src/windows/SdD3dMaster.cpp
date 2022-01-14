#include "SdD3dMaster.h"
#include "SdWView3d.h"
#include "SdD3dModelProgrammEditor.h"
#include "SdD3dModelMaster.h"
#include "SdDHelp.h"
#include "objects/SdObjectFactory.h"
#include "objects/SdPItemRich.h"
#include "master3d/SdM3dParser.h"

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
  setWindowTitle( tr("Select master to 3d model creation") );

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
  connect( editProg, &QPushButton::clicked, this, [this] () {
    int row = mMasterType->currentRow();
    if( row >= 0 && row < mIdList.count() ) {
      SdD3dModelProgrammEditor editor( mIdList.at(row), this );
      editor.exec();
      initializePage();
      }
    });


  boxLay->addStretch(1);

  vlay->addLayout( boxLay );

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
      SdD3dModelMaster dlg( mIdList.at(index), mPreviewPart, this );
      if( dlg.exec() ) {
        //Building model is successfull. Copy 3d model to the edited part
        mPartPtr->getUndo()->begin( tr("Replace 3d model"), mPartPtr, true );
        //Remove previous 3d objects
        mPartPtr->forEach( dct3D, [this] ( SdObject *obj ) -> bool {
          mPartPtr->deleteChild( obj, mPartPtr->getUndo() );
          return false;
          });
        //Insert new created 3d objects
        mPreviewPart->forEach( dct3D, [this] ( SdObject *obj ) -> bool {
          mPartPtr->insertChild( obj->copy(), mPartPtr->getUndo() );
          return false;
          });
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
  if( row >= 0 && row < mIdList.count() ) {
    SdPItemRich *rich = sdObjectOnly<SdPItemRich>( SdObjectFactory::extractObject( mIdList.at(row), false, this ) );
    if( rich != nullptr ) {
      mDescription->setText( rich->paramGet(stdParam3dModelProgramm) );
      SdM3dParser parser(nullptr);
      SdScriptProgramm *programm = parser.parse( rich->contents(), mPreviewPart );
      mPreviewPart->clear();
      programm->execute();
      delete programm;
      mPreview->setItem( mPreviewPart );
      mPreview->fitItem();
      mPreview->update();
      return;
      }
    }
  //Clear description and 3d object preview
  mDescription->clear();
  if( mPreview->item() != nullptr )
    mPreview->item()->clear();
  }




void SdD3dMaster::initializePage()
  {
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
