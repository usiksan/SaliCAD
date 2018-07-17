/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part master with exactly two pins
*/
#include "SdDMasterPartDoubleRect.h"
#include "ui_SdDMasterPartDoubleRect.h"
#include "objects/SdEnvir.h"

SdDMasterPartDoubleRect::SdDMasterPartDoubleRect(SdProjectItem *item, QWidget *parent) :
  SdDMasterPart( item, parent),
  ui(new Ui::SdDMasterPartDoubleRect)
  {
  ui->setupUi(this);

  ui->mBodySizeX->setText("3.0");
  ui->mBodySizeY->setText("1.6");
  ui->mBetweenPins->setText("5.0");
  onEditChanged( QString() );

  connect( ui->mBodySizeX, &QLineEdit::textEdited, this, &SdDMasterPartDoubleRect::onEditChanged );
  connect( ui->mBodySizeY, &QLineEdit::textEdited, this, &SdDMasterPartDoubleRect::onEditChanged );
  connect( ui->mBetweenPins, &QLineEdit::textEdited, this, &SdDMasterPartDoubleRect::onEditChanged );
  }




SdDMasterPartDoubleRect::~SdDMasterPartDoubleRect()
  {
  delete ui;
  }





void SdDMasterPartDoubleRect::onEditChanged(const QString txt)
  {
  Q_UNUSED(txt)

  SdIllustrator il;

  drawPart( il );
  il.setPixmap( 250, 250, Qt::white );
  drawPart( il );
  ui->mPreview->setPixmap( il.pixmap() );
  }




void SdDMasterPartDoubleRect::drawPart(SdIllustrator &il)
  {
  int partSizeX = sdEnvir->fromPhisPcb( ui->mBodySizeX->text() );
  int partSizeY = sdEnvir->fromPhisPcb( ui->mBodySizeY->text() );
  int sizeX = sdEnvir->fromPhisPcb( ui->mBetweenPins->text() );
  int pinLen = (sizeX - partSizeX) / 2;

  QColor red("red");
  QColor green("green");

  //Draw part
  il.drawRect( pinLen, -partSizeY / 2, sizeX - pinLen, partSizeY / 2, red );

  //Draw pins
  if( pinLen > 0 ) {
    il.drawLine( 0,0, pinLen,0, red );
    il.drawLine( sizeX - pinLen,0,  sizeX,0, red );
    }

  //Draw pin connectors
  int crossSize = partSizeY / 20;
  il.drawCross( 0,0, crossSize, green );
  il.drawCross( sizeX,0, crossSize, green );
  }





void SdDMasterPartDoubleRect::accept()
  {
  //Build part
  int partSizeX = sdEnvir->fromPhisPcb( ui->mBodySizeX->text() );
  int partSizeY = sdEnvir->fromPhisPcb( ui->mBodySizeY->text() );
  int sizeX = sdEnvir->fromPhisPcb( ui->mBetweenPins->text() );
  int pinLen = (sizeX - partSizeX) / 2;
  //Pin types
  QString leftPinType = ui->mLeftPinType->text();
  QString rightPinType = ui->mRightPinType->text();
  if( rightPinType.isEmpty() )
    rightPinType = leftPinType;

  //Rectangle body
  addRect( SdPoint(pinLen,-partSizeY/2), SdPoint(sizeX-pinLen,partSizeY/2) );

  //If pins outside body rectangle then draw pins
  if( pinLen > 0 ) {
    addLine( SdPoint(0,0), SdPoint(pinLen,0) );
    addLine( SdPoint(sizeX-pinLen,0), SdPoint(sizeX,0) );
    }

  //Add pins
  if( ui->mSmd->isChecked() )
    setupSmdPin();
  else
    setupThrouPin();
  //Make pin number and pin name invisible
  mPinNameProp.mLayer.set( LID0_INVISIBLE );
  mPinNumberProp.mLayer.set( LID0_INVISIBLE );
  addPin( SdPoint(0,0), leftPinType, SdPoint(0,250), QString("1"), SdPoint(0,-250) );
  addPin( SdPoint(sizeX,0), rightPinType, SdPoint(sizeX,250), QString("1"), SdPoint(sizeX,-250) );

  //Update ident position
  //When part size greater then ident text size (1000) then place ident in center of part
  // else place ident at top of part
  setId( SdPoint( sizeX/2, partSizeY > 1000 ? 0 : partSizeY/2+500), QString("Id") );

  //Close dialog
  SdDMasterPart::accept();
  }
