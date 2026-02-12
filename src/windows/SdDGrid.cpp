/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Grid options dialog
*/
#include "objects/SdEnvir.h"
#include "objects/SdUtil.h"

#include "SdDGrid.h"
#include "ui_SdDGrid.h"
#include "SdDHelp.h"


SdDGrid::SdDGrid(QPointF curGrid, QWidget *parent) :
  QDialog(parent),
  mGrid(curGrid),
  ui(new Ui::SdDGrid)
  {
  ui->setupUi(this);

  //Fill current values

  //Current grid
  printGrid( mGrid );
  ui->mStepX->selectAll();
  ui->mSyncXY->setChecked( SdEnvir::instance()->mGridSyncXY );
  ui->mStepY->setDisabled( SdEnvir::instance()->mGridSyncXY );

  connect( ui->mStepX, &QLineEdit::textChanged, this, [this] (const QString str) {
    //For syncronous editing when changed X then change and Y
    if( ui->mSyncXY->isChecked() )
      ui->mStepY->setText(str);
    });
  connect( ui->mSyncXY, &QCheckBox::toggled, ui->mStepY, &QLineEdit::setDisabled );


  //Fill previous grid list
  SdEnvir::instance()->gridForEach( -1, [this] ( QPointF p ) -> bool {
    if( p.x() == 0 || p.y() == 0 ) return false;
    QString str = QString::number( p.x(), 'f', 3 );
    str.append( " x " );
    str.append( QString::number( p.y(), 'f', 3 ) );
    ui->mPreviousList->addItem( str );
    return true;
    });

  connect( ui->mPreviousList, &QListWidget::currentRowChanged, this, [this] (int row) {
    if( row >= 0 )
      SdEnvir::instance()->gridForEach( row, [this] (QPointF p) -> bool {
        printGrid( p );
        return true;
        });
    });

  //Grid view
  ui->mShowGrid->setChecked( SdEnvir::instance()->mGridShow );
  ui->mAlignCursor->setChecked( SdEnvir::instance()->mCursorAlignGrid );
  ui->mShowCursor->setChecked( SdEnvir::instance()->mCursorShow );

  //Crosshair view
  ui->mCrosshairNone->setChecked( SdEnvir::instance()->mCursorView == 0 );
  ui->mCrosshairSmall->setChecked( SdEnvir::instance()->mCursorView == 1 );
  ui->mCrosshairFull->setChecked( SdEnvir::instance()->mCursorView == 2 );
  ui->mCrosshairSmall45->setChecked( SdEnvir::instance()->mCursorView == 3 );
  ui->mCrosshairFull45->setChecked( SdEnvir::instance()->mCursorView == 4 );

  //Help system
  connect( ui->buttonBox, &QDialogButtonBox::helpRequested, this, [this] () { SdDHelp::help( "SdDGrid.htm", this ); });
  }

SdDGrid::~SdDGrid()
  {
  delete ui;
  }



//Fill X and Y fields
void SdDGrid::printGrid(QPointF g)
  {
  ui->mStepX->setText( QString::number( g.x(), 'f', 3 ) );
  ui->mStepY->setText( QString::number( g.y(), 'f', 3 ) );
  }


void SdDGrid::accept()
  {
  //Syncro flag
  SdEnvir::instance()->mGridSyncXY = ui->mSyncXY->isChecked();

  //Current grid
  mGrid.rx() = SdUtil::str2phys( ui->mStepX->text() );
  mGrid.ry() = SdUtil::str2phys( ui->mStepY->text() );

  //Grid history
  SdEnvir::instance()->gridAppend( mGrid );

  //Flags
  SdEnvir::instance()->mGridShow        = ui->mShowGrid->isChecked();
  SdEnvir::instance()->mCursorAlignGrid = ui->mAlignCursor->isChecked();
  SdEnvir::instance()->mCursorShow      = ui->mShowCursor->isChecked();

  //Cursor crosshair
  if( ui->mCrosshairNone->isChecked() )
    SdEnvir::instance()->mCursorView = 0;
  else if( ui->mCrosshairSmall->isChecked() )
    SdEnvir::instance()->mCursorView = 1;
  else if( ui->mCrosshairFull->isChecked() )
    SdEnvir::instance()->mCursorView = 2;
  else if( ui->mCrosshairSmall45->isChecked() )
    SdEnvir::instance()->mCursorView = 3;
  else
    SdEnvir::instance()->mCursorView = 4;

  done(1);
  }
