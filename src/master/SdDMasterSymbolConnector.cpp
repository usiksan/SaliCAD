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

static int
sPinCount = 2,
sCellHeight = 500,
sPinSizeX = 500,
sNumberSizeX = 1000,
sCellSizeX = 2500;
static bool
sRightDirection = false;


SdDMasterSymbolConnector::SdDMasterSymbolConnector(SdProjectItem *item, QWidget *parent) :
  SdDMasterSymbol( item, parent ),
  ui(new Ui::SdDMasterSymbolConnector)
  {
  ui->setupUi(this);

  ui->mPinCount->setText( QString::number(sPinCount) );

  //Connector construction defaults
  ui->mCellHeight->setText( SdEnvir::instance()->toPhisSchematic(sCellHeight) );
  ui->mPinSizeX->setText( SdEnvir::instance()->toPhisSchematic(sPinSizeX) );
  ui->mNumberSizeX->setText( SdEnvir::instance()->toPhisSchematic(sNumberSizeX) );
  ui->mCellSizeX->setText( SdEnvir::instance()->toPhisSchematic(sCellSizeX) );
  ui->mRightDirection->setChecked(sRightDirection);

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
  int cellHeight  = SdEnvir::instance()->fromPhisSchematic( ui->mCellHeight->text() );
  int pinSizeX    = SdEnvir::instance()->fromPhisSchematic( ui->mPinSizeX->text() );
  int numberSizeX = SdEnvir::instance()->fromPhisSchematic( ui->mNumberSizeX->text() );
  int cellSizeX   = SdEnvir::instance()->fromPhisSchematic( ui->mCellSizeX->text() );
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
  sPinCount    = ui->mPinCount->text().toInt();
  sCellHeight  = SdEnvir::instance()->fromPhisSchematic( ui->mCellHeight->text() );
  sPinSizeX    = SdEnvir::instance()->fromPhisSchematic( ui->mPinSizeX->text() );
  sNumberSizeX = SdEnvir::instance()->fromPhisSchematic( ui->mNumberSizeX->text() );
  sCellSizeX   = SdEnvir::instance()->fromPhisSchematic( ui->mCellSizeX->text() );
  sRightDirection = ui->mRightDirection->isChecked();

  int rightPos = sPinSizeX + sNumberSizeX + sCellSizeX;

  if( sPinCount < 1 )
    sPinCount = 1;

  int dir = 1;
  if( sRightDirection ) {
    dir = -1;
    mPinNameProp.mHorz = dhjRight;
    }
  mPinNumberProp.mHorz = dhjCenter;

  mPinNameProp.mLayer.set( LID0_INVISIBLE );

  for( int i = 0; i < sPinCount; i++ ) {
    //Pin line
    addLine( 0, -i * sCellHeight, sPinSizeX * dir, -i * sCellHeight );
    //Pin down line
    if( i < sPinCount - 1 )
      addLine( sPinSizeX * dir, -(i * sCellHeight + sCellHeight / 2),
               rightPos * dir, -(i * sCellHeight + sCellHeight / 2) );
    //Pin
    SdPoint pinOrg(0,-i*sCellHeight);
    addPin( pinOrg, 0, SdPoint(0,pinOrg.y()+sCellHeight/2), QString::number(i+1), SdPoint((sPinSizeX+sNumberSizeX/2)*dir,pinOrg.y()) );
    }

  //Body
  addRect( sPinSizeX * dir, sCellHeight / 2, rightPos * dir, -(sPinCount * sCellHeight - sCellHeight / 2) );
  //Vertical delimiter
  int delimiter = (sPinSizeX + sNumberSizeX) * dir;
  addLine( delimiter, sCellHeight / 2, delimiter, -(sPinCount * sCellHeight - sCellHeight / 2) );

  //id
  setId( SdPoint( (sPinSizeX + (sNumberSizeX+sCellSizeX)/2) * dir, sCellHeight) );

  //value
  setValue( SdPoint( (sPinSizeX + (sNumberSizeX+sCellSizeX)/2) * dir, -sPinCount * sCellHeight - sCellHeight ) );

  SdDMasterSymbol::accept();
  }
