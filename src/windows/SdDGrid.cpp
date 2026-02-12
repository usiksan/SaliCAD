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
  ui->mSyncXY->setChecked( sdEnvir::instance()->mGridSyncXY );
  ui->mStepY->setDisabled( sdEnvir::instance()->mGridSyncXY );

  connect( ui->mStepX, &QLineEdit::textChanged, this, [this] (const QString str) {
    //For syncronous editing when changed X then change and Y
    if( ui->mSyncXY->isChecked() )
      ui->mStepY->setText(str);
    });
  connect( ui->mSyncXY, &QCheckBox::toggled, ui->mStepY, &QLineEdit::setDisabled );


  //Fill previous grid list
  for( QPointF p : sdEnvir::instance()->mGridHistory ) {
    if( p.x() == 0 || p.y() == 0 ) break;
    QString str = QString::number( p.x(), 'f', 3 );
    str.append( " x " );
    str.append( QString::number( p.y(), 'f', 3 ) );
    ui->mPreviousList->addItem( str );
    }

  connect( ui->mPreviousList, &QListWidget::currentRowChanged, this, [this] (int row) {
    if( row >= 0 && row < sdEnvir::instance()->mGridHistory.count() )
      printGrid( sdEnvir::instance()->mGridHistory.at(row) );
    });

  //Grid view
  ui->mShowGrid->setChecked( sdEnvir::instance()->mGridShow );
  ui->mAlignCursor->setChecked( sdEnvir::instance()->mCursorAlignGrid );
  ui->mShowCursor->setChecked( sdEnvir::instance()->mCursorShow );

  //Crosshair view
  ui->mCrosshairNone->setChecked( sdEnvir::instance()->mCursorView == 0 );
  ui->mCrosshairSmall->setChecked( sdEnvir::instance()->mCursorView == 1 );
  ui->mCrosshairFull->setChecked( sdEnvir::instance()->mCursorView == 2 );
  ui->mCrosshairSmall45->setChecked( sdEnvir::instance()->mCursorView == 3 );
  ui->mCrosshairFull45->setChecked( sdEnvir::instance()->mCursorView == 4 );

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
  sdEnvir::instance()->mGridSyncXY = ui->mSyncXY->isChecked();

  //Current grid
  mGrid.rx() = SdUtil::str2phys( ui->mStepX->text() );
  mGrid.ry() = SdUtil::str2phys( ui->mStepY->text() );

  //Grid history
  int i;
  for( i = 0; i < sdEnvir::instance()->mGridHistory.count(); i++ )
    if( sdEnvir::instance()->mGridHistory.at(i) == mGrid ) {
      if( i ) {
        //Move grid dimension on top of list
        sdEnvir::instance()->mGridHistory.removeAt( i );
        sdEnvir::instance()->mGridHistory.insert( 0, mGrid );
        }
      break;
      }
  if( i >= sdEnvir::instance()->mGridHistory.count() ) {
    //This grid dimension is new for list
    //Check list size. If enough space then simle append else last remove
    if( sdEnvir::instance()->mGridHistory.count() >= GRID_HISTORY_SIZE )
      sdEnvir::instance()->mGridHistory.removeLast();
    sdEnvir::instance()->mGridHistory.insert( 0, mGrid );
    }

  //Flags
  sdEnvir::instance()->mGridShow        = ui->mShowGrid->isChecked();
  sdEnvir::instance()->mCursorAlignGrid = ui->mAlignCursor->isChecked();
  sdEnvir::instance()->mCursorShow      = ui->mShowCursor->isChecked();

  //Cursor crosshair
  if( ui->mCrosshairNone->isChecked() )
    sdEnvir::instance()->mCursorView = 0;
  else if( ui->mCrosshairSmall->isChecked() )
    sdEnvir::instance()->mCursorView = 1;
  else if( ui->mCrosshairFull->isChecked() )
    sdEnvir::instance()->mCursorView = 2;
  else if( ui->mCrosshairSmall45->isChecked() )
    sdEnvir::instance()->mCursorView = 3;
  else
    sdEnvir::instance()->mCursorView = 4;

  done(1);
  }
