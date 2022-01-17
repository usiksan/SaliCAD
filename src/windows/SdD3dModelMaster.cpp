/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model master.

  It allow build new 3d and 2d projection model of part
*/
#include "SdD3dModelMaster.h"
#include "SdDPadMaster.h"
#include "script/SdScriptParser3d.h"
#include "script/SdScriptProgramm.h"
#include "objects/SdObjectFactory.h"
#include "SdDHelp.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSplitter>
#include <QDialogButtonBox>
#include <QFont>
#include <QMessageBox>
#include <QColorDialog>
#include <QGuiApplication>
#include <QScreen>


//!
//! \brief SdD3dModelMaster Dialog constructor. It creates dialog for build new 3d and 2d projection model of part
//! \param id               id of rich text object which conains 3d model source programm
//! \param part             Pointer to created part object
//! \param parent           Parent widget
//!
SdD3dModelMaster::SdD3dModelMaster(const QString id, SdPItemPart *part, QWidget *parent) :
  QDialog(parent),
  mPart(part),
  mProgramm(nullptr),
  mActive(false)
  {
  //Setup editor size
  resize( qMin( QGuiApplication::primaryScreen()->size().width(), 1600), 700 );

  //Main layout of dialog is vertical
  QVBoxLayout *vlay = new QVBoxLayout();

  //Central part of dialog is splitter
  QSplitter *splitter = new QSplitter();
  vlay->addWidget( splitter );

   //Param table widget
   splitter->addWidget( mParamWidget = new QTableWidget() );
   connect( mParamWidget, &QTableWidget::cellChanged, this, &SdD3dModelMaster::rebuild );
   connect( mParamWidget, &QTableWidget::cellClicked, this, &SdD3dModelMaster::onCellClicked );

   //Part preview widget
   splitter->addWidget( mPreview = new SdWView3d( mPart, this ) );

   //Stretch factor
   splitter->setStretchFactor( 0, 1 );
   splitter->setStretchFactor( 1, 3 );

  //Buttons at dialog bottom
  QDialogButtonBox *dialogButtonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Help );
  vlay->addWidget( dialogButtonBox );
  connect( dialogButtonBox, &QDialogButtonBox::accepted, this, &SdD3dModelMaster::accept );
  connect( dialogButtonBox, &QDialogButtonBox::rejected, this, &SdD3dModelMaster::reject );
  //Help system
  connect( dialogButtonBox, &QDialogButtonBox::helpRequested, this, [this] () { SdDHelp::help( "SdD3dModelMaster.htm", this ); });

  setLayout( vlay );

  //Extract programm source
  if( id.isEmpty() ) done(0);
  else {
    //Retrive rich object with id from local libary
    SdPItemRich *rich = sdObjectOnly<SdPItemRich>( SdObjectFactory::extractObject( id, false, parent ) );

    if( rich == nullptr ) done(0);
    else {
      mParamWidget->clear();
      mParamWidget->setColumnCount(2);
      mParamWidget->setRowCount(0);
      mParamWidget->setHorizontalHeaderLabels( {tr("Parametr name"), tr("Parametr value") } );
      SdScriptParser3d parser(mParamWidget);

      mProgramm = parser.parse3d( rich->contents(), mPart );
      rebuild();
      mPreview->fitItem();
      mPreview->update();
      }
    }
  }





SdD3dModelMaster::~SdD3dModelMaster()
  {
  }





void SdD3dModelMaster::rebuild()
  {
  if( mProgramm && !mActive ) {
    mActive = true;
    //Clear previously builded part
    mPart->clear();

    //Build new part
    mProgramm->execute();

    //Update preview
    mPreview->update();
    mActive = false;
    }
  }




void SdD3dModelMaster::onCellClicked(int row, int column)
  {
  if( column == 1 ) {
    QTableWidgetItem *item = mParamWidget->item( row, column );
    if( item != nullptr ) {
      if( item->type() == SDM3D_INPUT_COLOR ) {
        //Show color selection dialog
        QColor color = QColorDialog::getColor( QColor(item->text()), this, mParamWidget->item(row,0)->text() );
        if( color.isValid() )
          item->setText( color.name() );
        return;
        }
      else if( item->type() == SDM3D_INPUT_PAD ) {
        //Show pad master dialog
        item->setText( SdDPadMaster::build( item->text(), this ) );
        return;
        }
      }
    }
  }
