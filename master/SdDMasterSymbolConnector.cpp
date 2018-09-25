/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Symbol creation master for connectors
*/
#include "SdDMasterSymbolConnector.h"
#include "ui_SdDMasterSymbolConnector.h"
#include "objects/SdEnvir.h"

SdDMasterSymbolConnector::SdDMasterSymbolConnector(SdProjectItem *item, QWidget *parent) :
  SdDMasterSymbol( item, parent ),
  ui(new Ui::SdDMasterSymbolConnector)
  {
  ui->setupUi(this);

  ui->mPinCount->setText("2");

  //Connector construction defaults
  ui->mCellHeight->setText("5.0");
  ui->mPinSizeX->setText("5.0");
  ui->mNumberSizeX->setText("7.5");
  ui->mCellSizeX->setText("20.0");

  onEditChanged( QString() );

  connect( ui->mRightDirection, &QCheckBox::toggled, this, [this](bool) {
    onEditChanged( QString() );
    });
  connect( ui->mPinCount, &QLineEdit::textEdited, this, &SdDMasterSymbolConnector::onEditChanged );
  connect( ui->mCellHeight, &QLineEdit::textEdited, this, &SdDMasterSymbolConnector::onEditChanged );
  connect( ui->mPinSizeX, &QLineEdit::textEdited, this, &SdDMasterSymbolConnector::onEditChanged );

  connect( ui->mNumberSizeX, &QLineEdit::textEdited, this, &SdDMasterSymbolConnector::onEditChanged );
  connect( ui->mCellSizeX, &QLineEdit::textEdited, this, &SdDMasterSymbolConnector::onEditChanged );
  }




SdDMasterSymbolConnector::~SdDMasterSymbolConnector()
  {
  delete ui;
  }







//Update preview on any params changed
void SdDMasterSymbolConnector::onEditChanged(const QString txt)
  {
  Q_UNUSED(txt)

  SdIllustrator il;

  drawSymbol( il );
  il.setPixmap( 250, 250, Qt::white );
  drawSymbol( il );
  ui->mPreview->setPixmap( il.pixmap() );
  }




//Draw symbol preview
void SdDMasterSymbolConnector::drawSymbol(SdIllustrator &il)
  {
  int pinCount    = ui->mPinCount->text().toInt();
  int cellHeight  = sdEnvir->fromPhisSchematic( ui->mCellHeight->text() );
  int pinSizeX    = sdEnvir->fromPhisSchematic( ui->mPinSizeX->text() );
  int numberSizeX = sdEnvir->fromPhisSchematic( ui->mNumberSizeX->text() );
  int cellSizeX   = sdEnvir->fromPhisSchematic( ui->mCellSizeX->text() );
  bool rightDirection = ui->mRightDirection->isChecked();

  int rightPos = pinSizeX + numberSizeX + cellSizeX;

  if( pinCount < 1 )
    pinCount = 1;

  QColor red("red");
  QColor green("green");

  int dir = 1;
  if( rightDirection ) dir = -1;

  for( int i = 0; i < pinCount; i++ ) {
    //Pin line
    il.drawLine( 0, i * cellHeight, pinSizeX * dir, i * cellHeight, red );
    //Pin down line
    if( i < pinCount - 1 )
      il.drawLine( pinSizeX * dir, i * cellHeight + cellHeight / 2,
                   rightPos * dir, i * cellHeight + cellHeight / 2, red );
    //Pin crosshair
    il.drawCross( 0, i * cellHeight, 50, green );
    }

  //Body
  il.drawRect( pinSizeX * dir, -cellHeight / 2, rightPos * dir, pinCount * cellHeight - cellHeight / 2, red );
  //Vertical delimiter
  int delimiter = (pinSizeX + numberSizeX) * dir;
  il.drawLine( delimiter, -cellHeight / 2, delimiter, pinCount * cellHeight - cellHeight / 2, red );
  }



void SdDMasterSymbolConnector::accept()
  {
  int pinCount    = ui->mPinCount->text().toInt();
  int cellHeight  = sdEnvir->fromPhisSchematic( ui->mCellHeight->text() );
  int pinSizeX    = sdEnvir->fromPhisSchematic( ui->mPinSizeX->text() );
  int numberSizeX = sdEnvir->fromPhisSchematic( ui->mNumberSizeX->text() );
  int cellSizeX   = sdEnvir->fromPhisSchematic( ui->mCellSizeX->text() );
  bool rightDirection = ui->mRightDirection->isChecked();

  int rightPos = pinSizeX + numberSizeX + cellSizeX;

  if( pinCount < 1 )
    pinCount = 1;

  int dir = 1;
  if( rightDirection ) {
    dir = -1;
    mPinNameProp.mHorz = dhjRight;
    }
  mPinNumberProp.mHorz = dhjCenter;

  mPinNameProp.mLayer.set( LID0_INVISIBLE );

  for( int i = 0; i < pinCount; i++ ) {
    //Pin line
    addLine( 0, -i * cellHeight, pinSizeX * dir, -i * cellHeight );
    //Pin down line
    if( i < pinCount - 1 )
      addLine( pinSizeX * dir, -(i * cellHeight + cellHeight / 2),
               rightPos * dir, -(i * cellHeight + cellHeight / 2) );
    //Pin
    SdPoint pinOrg(0,-i*cellHeight);
    addPin( pinOrg, 0, SdPoint(0,pinOrg.y()+cellHeight/2), QString::number(i+1), SdPoint((pinSizeX+numberSizeX/2)*dir,pinOrg.y()) );
    }

  //Body
  addRect( pinSizeX * dir, cellHeight / 2, rightPos * dir, -(pinCount * cellHeight - cellHeight / 2) );
  //Vertical delimiter
  int delimiter = (pinSizeX + numberSizeX) * dir;
  addLine( delimiter, cellHeight / 2, delimiter, -(pinCount * cellHeight - cellHeight / 2) );

  //id
  setId( SdPoint( (pinSizeX + (numberSizeX+cellSizeX)/2) * dir, cellHeight) );

  //value
  setValue( SdPoint( (pinSizeX + (numberSizeX+cellSizeX)/2) * dir, -pinCount * cellHeight - cellHeight ) );

  SdDMasterSymbol::accept();
  }
