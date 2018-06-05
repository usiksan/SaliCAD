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
  ui->mSyncXY->setChecked( sdEnvir->mGridSyncXY );
  ui->mStepY->setDisabled( sdEnvir->mGridSyncXY );

  connect( ui->mStepX, &QLineEdit::textChanged, this, [this] (const QString str) {
    //For syncronous editing when changed X then change and Y
    if( ui->mSyncXY->isChecked() )
      ui->mStepY->setText(str);
    });
  connect( ui->mSyncXY, &QCheckBox::toggled, ui->mStepY, &QLineEdit::setDisabled );


  //Fill previous grid list
  for( QPointF p : sdEnvir->mGridHistory ) {
    if( p.x() == 0 || p.y() == 0 ) break;
    QString str = QString::number( p.x(), 'f', 3 );
    str.append( " x " );
    str.append( QString::number( p.y(), 'f', 3 ) );
    ui->mPreviousList->addItem( str );
    }

  connect( ui->mPreviousList, &QListWidget::currentRowChanged, this, [this] (int row) {
    if( row >= 0 && row < sdEnvir->mGridHistory.count() )
      printGrid( sdEnvir->mGridHistory.at(row) );
    });

  //Grid view
  ui->mShowGrid->setChecked( sdEnvir->mGridShow );
  ui->mAlignCursor->setChecked( sdEnvir->mCursorAlignGrid );
  ui->mShowCursor->setChecked( sdEnvir->mCursorShow );

  //Crosshair view
  ui->mCrosshairNone->setChecked( sdEnvir->mCursorView == 0 );
  ui->mCrosshairSmall->setChecked( sdEnvir->mCursorView == 1 );
  ui->mCrosshairFull->setChecked( sdEnvir->mCursorView == 2 );
  ui->mCrosshairSmall45->setChecked( sdEnvir->mCursorView == 3 );
  ui->mCrosshairFull45->setChecked( sdEnvir->mCursorView == 4 );
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
  sdEnvir->mGridSyncXY = ui->mSyncXY->isChecked();

  //Current grid
  mGrid.rx() = SdUtil::str2phys( ui->mStepX->text() );
  mGrid.ry() = SdUtil::str2phys( ui->mStepY->text() );

  //Grid history
  int i;
  for( i = 0; i < sdEnvir->mGridHistory.count(); i++ )
    if( sdEnvir->mGridHistory.at(i) == mGrid ) {
      if( i ) {
        //Move grid dimension on top of list
        sdEnvir->mGridHistory.removeAt( i );
        sdEnvir->mGridHistory.insert( 0, mGrid );
        }
      break;
      }
  if( i >= sdEnvir->mGridHistory.count() ) {
    //This grid dimension is new for list
    //Check list size. If enough space then simle append else last remove
    if( sdEnvir->mGridHistory.count() >= GRID_HISTORY_SIZE )
      sdEnvir->mGridHistory.removeLast();
    sdEnvir->mGridHistory.insert( 0, mGrid );
    }

  //Flags
  sdEnvir->mGridShow        = ui->mShowGrid->isChecked();
  sdEnvir->mCursorAlignGrid = ui->mAlignCursor->isChecked();
  sdEnvir->mCursorShow      = ui->mShowCursor->isChecked();

  //Cursor crosshair
  if( ui->mCrosshairNone->isChecked() )
    sdEnvir->mCursorView = 0;
  else if( ui->mCrosshairSmall->isChecked() )
    sdEnvir->mCursorView = 1;
  else if( ui->mCrosshairFull->isChecked() )
    sdEnvir->mCursorView = 2;
  else if( ui->mCrosshairSmall45->isChecked() )
    sdEnvir->mCursorView = 3;
  else
    sdEnvir->mCursorView = 4;

  done(1);
  }
