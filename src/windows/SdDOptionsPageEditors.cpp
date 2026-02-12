/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Some editor parametrs
*/
#include "SdConfig.h"
#include "SdDOptionsPageEditors.h"
#include "objects/SdEnvir.h"
#include <QTableWidgetItem>
#include <QColorDialog>
#include <QSettings>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>

SdDOptionsPageEditors::SdDOptionsPageEditors(QWidget *parent) : QWidget(parent)
  {
  QVBoxLayout *vbox = new QVBoxLayout();
  vbox->addWidget( mShowRatNet = new QCheckBox(tr("show rat net")) );
  mShowRatNet->setChecked( SdEnvir::instance()->mShowRatNet );
  vbox->addWidget( mShowRemarks = new QCheckBox(tr("show remarks")) );
  mShowRemarks->setChecked( SdEnvir::instance()->mShowRemark );
  vbox->addWidget( mShowMessage = new QCheckBox(tr("show message")) );
  mShowMessage->setChecked( SdEnvir::instance()->mShowMessageRemark );
  vbox->addWidget( mCenterCursor = new QCheckBox(tr("move cursor to center when zoom")) );
  mCenterCursor->setChecked( SdEnvir::instance()->mCenterCursor );

  QGridLayout *grid = new QGridLayout();
  vbox->addLayout( grid );
  grid->addWidget( new QLabel(tr("Symbol pin size:")), 0, 0 );
  grid->addWidget( mSymbolPinSize = new QLineEdit(), 0, 1 );
  mSymbolPinSize->setText( SdEnvir::instance()->toPhisSchematic(SdEnvir::instance()->mSymPinSize) );

  grid->addWidget( new QLabel(tr("Part pin size:")), 1, 0 );
  grid->addWidget( mPartPinSize = new QLineEdit(), 1, 1 );
  mPartPinSize->setText( SdEnvir::instance()->toPhisPcb(SdEnvir::instance()->mPartPinSize) );

  grid->addWidget( new QLabel(tr("Wire dot size:")), 2, 0 );
  grid->addWidget( mWireDotSize = new QLineEdit(), 2, 1 );
  mWireDotSize->setText( SdEnvir::instance()->toPhisSchematic(SdEnvir::instance()->mDotSize) );

  grid->addWidget( new QLabel(tr("Wire dot width:")), 3, 0 );
  grid->addWidget( mWireDotWidth = new QLineEdit(), 3, 1 );
  mWireDotWidth->setText( SdEnvir::instance()->toPhisSchematic(SdEnvir::instance()->mDotWidth) );

  grid->addWidget( new QLabel(tr("Via default type:")), 4, 0 );
  grid->addWidget( mViaType = new QLineEdit(), 4, 1 );
  mViaType->setText( SdEnvir::instance()->mViaType );

  //TODO D050 Assign all other envir values to editor options
  grid->addWidget( new QLabel(tr("Width step size:")), 5, 0 );
  grid->addWidget( mWidthStep = new QLineEdit(), 5, 1 );

  grid->addWidget( new QLabel(tr("Text step size:")), 6, 0 );
  grid->addWidget( mTextSizeStep = new QLineEdit(), 6, 1 );

  grid->addWidget( new QLabel(tr("Minimal view grid size:")), 7, 0 );
  grid->addWidget( mMinViewGridSize = new QLineEdit(), 7, 1 );

  grid->addWidget( new QLabel(tr("Cursor view size:")), 8, 0 );
  grid->addWidget( mCursorViewSize = new QLineEdit(), 8, 1 );

  grid->addWidget( new QLabel(tr("Trace dot size:")), 9, 0 );
  grid->addWidget( mTraceDotSize = new QLineEdit(), 9, 1 );

  setLayout(vbox);
  }



void SdDOptionsPageEditors::accept()
  {
  //Apply current params
  SdEnvir::instance()->mShowRatNet        = mShowRatNet->isChecked();
  SdEnvir::instance()->mShowRemark        = mShowRemarks->isChecked();
  SdEnvir::instance()->mShowMessageRemark = mShowMessage->isChecked();
  SdEnvir::instance()->mCenterCursor      = mCenterCursor->isChecked();

  SdEnvir::instance()->mSymPinSize  = SdEnvir::instance()->fromPhisSchematic( mSymbolPinSize->text() );
  SdEnvir::instance()->mPartPinSize = SdEnvir::instance()->fromPhisPcb( mPartPinSize->text() );
  SdEnvir::instance()->mDotSize     = SdEnvir::instance()->fromPhisSchematic( mWireDotSize->text() );
  SdEnvir::instance()->mDotWidth    = SdEnvir::instance()->fromPhisSchematic( mWireDotWidth->text() );
  SdEnvir::instance()->mViaType     = mViaType->text();

//  grid->addWidget( mViaSize = new QLineEdit(), 4, 1 );
//  grid->addWidget( new QLabel(tr("Width step size:")), 5, 0 );
//  grid->addWidget( mWidthStep = new QLineEdit(), 5, 1 );

//  grid->addWidget( new QLabel(tr("Text step size:")), 6, 0 );
//  grid->addWidget( mTextSizeStep = new QLineEdit(), 6, 1 );

//  grid->addWidget( new QLabel(tr("Minimal view grid size:")), 7, 0 );
//  grid->addWidget( mMinViewGridSize = new QLineEdit(), 7, 1 );

//  grid->addWidget( new QLabel(tr("Cursor view size:")), 8, 0 );
//  grid->addWidget( mCursorViewSize = new QLineEdit(), 8, 1 );

//  grid->addWidget( new QLabel(tr("Trace dot size:")), 9, 0 );
//  grid->addWidget( mTraceDotSize = new QLineEdit(), 9, 1 );

  }
