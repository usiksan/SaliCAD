/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part master with rectangle body and two pins rows on top and on bottom sides
*/
#include "SdDMasterPartDoubleSide.h"
#include "ui_SdDMasterPartDoubleSide.h"
#include "windows/SdDPads.h"
#include "windows/SdDPadMaster.h"
#include "objects/SdEnvir.h"

#include <QToolButton>

static int
sBodySizeX = 4600,
sBodySizeY = 3000,
sPinSizeY = 5000,
sBottomPinDistance = 1000,
sTopPinDistance = 1000,
sBottomPinOffsetX = 0,
sTopPinOffsetX = 0;

static QString
sBottomPinCount("2"),
sTopPinCount("2"),
sBottomPinType("r0.4x1.0m0.1s0.04"),
sTopPinType("r0.4x1.0m0.1s0.04");


SdDMasterPartDoubleSide::SdDMasterPartDoubleSide(SdProjectItem *item, QWidget *parent) :
  SdDMasterPart( item, parent ),
  ui(new Ui::SdDMasterPartDoubleSide)
  {
  ui->setupUi(this);

  ui->mBodySizeY->setText( sdEnvir->toPhisPcb(sBodySizeY) );
  ui->mBodySizeX->setText( sdEnvir->toPhisPcb(sBodySizeX) );
  ui->mPinSizeY->setText( sdEnvir->toPhisPcb(sPinSizeY) );

  //Left side
  ui->mBottomPinCount->setText( sBottomPinCount );
  ui->mBottomPinDistance->setText( sdEnvir->toPhisPcb(sBottomPinDistance) );
  ui->mBottomPinOffsetX->setText( sdEnvir->toPhisPcb(sBottomPinOffsetX) );
  ui->mBottomPinType->setText( sBottomPinType );

  //Right side
  ui->mTopPinCount->setText( sTopPinCount );
  ui->mTopPinDistance->setText( sdEnvir->toPhisPcb(sTopPinDistance) );
  ui->mTopPinOffsetX->setText( sdEnvir->toPhisPcb(sTopPinOffsetX) );
  ui->mTopPinType->setText( sTopPinType );

  onEditChanged( QString() );

  connect( ui->mBodySizeX, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSide::onEditChanged );
  connect( ui->mBodySizeY, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSide::onEditChanged );
  connect( ui->mPinSizeY, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSide::onEditChanged );

  connect( ui->mBottomPinCount, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSide::onEditChanged );
  connect( ui->mBottomPinDistance, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSide::onEditChanged );
  connect( ui->mBottomPinOffsetX, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSide::onEditChanged );

  connect( ui->mTopPinCount, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSide::onEditChanged );
  connect( ui->mTopPinDistance, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSide::onEditChanged );
  connect( ui->mTopPinOffsetX, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSide::onEditChanged );

  ui->mBottomPinType->setReadOnly(true);
  connect( ui->mBottomPinTypeSelect, &QToolButton::clicked, this, [this] () {
    ui->mBottomPinType->setText( SdDPadMaster::build( ui->mBottomPinType->text(), this ) );
    } );

  ui->mTopPinType->setReadOnly(true);
  connect( ui->mTopPinTypeSelect, &QToolButton::clicked, this, [this] () {
    ui->mTopPinType->setText( SdDPadMaster::build( ui->mTopPinType->text(), this ) );
    } );

  }



SdDMasterPartDoubleSide::~SdDMasterPartDoubleSide()
  {
  delete ui;
  }




//Update preview on any params changed
void SdDMasterPartDoubleSide::onEditChanged(const QString txt)
  {
  Q_UNUSED(txt)

  SdIllustrator il;

  drawPart( il );
  il.setPixmap( 250, 250, Qt::white );
  drawPart( il );
  ui->mPreview->setPixmap( il.pixmap() );
  }




//Draw part preview
void SdDMasterPartDoubleSide::drawPart(SdIllustrator &il)
  {
  int bodySizeX = sdEnvir->fromPhisPcb( ui->mBodySizeX->text() );
  int bodySizeY = sdEnvir->fromPhisPcb( ui->mBodySizeY->text() );
  int pinSizeY  = sdEnvir->fromPhisPcb( ui->mPinSizeY->text() );
  int pinLen = (pinSizeY - bodySizeY) / 2;

  int bottomPinCount    = ui->mBottomPinCount->text().toInt();
  int bottomPinDistance = sdEnvir->fromPhisPcb( ui->mBottomPinDistance->text() );
  int bottomPinOffsetX  = sdEnvir->fromPhisPcb( ui->mBottomPinOffsetX->text() );

  int topPinCount    = ui->mTopPinCount->text().toInt();
  int topPinDistance = sdEnvir->fromPhisPcb( ui->mTopPinDistance->text() );
  int topPinOffsetX  = sdEnvir->fromPhisPcb( ui->mTopPinOffsetX->text() );

  QColor red("red");
  QColor green("green");

  //Pin 1 reside at point 0,0

  //Bottom pins lenght
  int bottomPinsLenght = (bottomPinCount - 1) * bottomPinDistance;

  //Part left pos
  int partLeft = -( bodySizeX - bottomPinsLenght ) / 2 + bottomPinOffsetX;

  //Part right pos
  int partRight = partLeft + bodySizeX;

  //Top pins lenght
  int topPinsLenght = (topPinCount - 1) * topPinDistance;

  //Part right offset top right pin
  int pinRtop = partLeft + ((bodySizeX - topPinsLenght) / 2 - topPinOffsetX) + topPinsLenght;

  //Draw part
  il.drawRect( partLeft, pinLen, partRight, pinSizeY - pinLen, red );

  //Draw pins
  if( pinLen > 0 ) {
    //Bottom pins
    for( int i = 0; i < bottomPinCount; i++ )
      il.drawLine( i*bottomPinDistance,0, i*bottomPinDistance,pinLen, red );

    //Top pins
    for( int i = 0; i < topPinCount; i++ )
      il.drawLine( pinRtop - i*topPinDistance,pinSizeY,  pinRtop - i*topPinDistance,pinSizeY - pinLen, red );
    }

  //Draw pin connectors
  int crossSize = bodySizeY / 20;
  //Left pins
  for( int i = 0; i < bottomPinCount; i++ )
    il.drawCross( i*bottomPinDistance,0, crossSize, green );
  //Right pins
  for( int i = 0; i < topPinCount; i++ )
    il.drawCross( pinRtop - i*topPinDistance,pinSizeY, crossSize, green );
  }



void SdDMasterPartDoubleSide::accept()
  {
  int bodySizeX = sBodySizeX = sdEnvir->fromPhisPcb( ui->mBodySizeX->text() );
  int bodySizeY = sBodySizeY = sdEnvir->fromPhisPcb( ui->mBodySizeY->text() );
  int pinSizeY  = sPinSizeY = sdEnvir->fromPhisPcb( ui->mPinSizeY->text() );
  int pinLen = (pinSizeY - bodySizeY) / 2;

  sBottomPinCount = ui->mBottomPinCount->text();
  int bottomPinCount    = sBottomPinCount.toInt();
  int bottomPinDistance = sBottomPinDistance = sdEnvir->fromPhisPcb( ui->mBottomPinDistance->text() );
  int bottomPinOffsetX  = sBottomPinOffsetX = sdEnvir->fromPhisPcb( ui->mBottomPinOffsetX->text() );

  sTopPinCount = ui->mTopPinCount->text();
  int topPinCount    = sTopPinCount.toInt();
  int topPinDistance = sTopPinDistance = sdEnvir->fromPhisPcb( ui->mTopPinDistance->text() );
  int topPinOffsetX  = sTopPinOffsetX = sdEnvir->fromPhisPcb( ui->mTopPinOffsetX->text() );

  //Pin types
  QString topPinType = sTopPinType = ui->mTopPinType->text();
  QString bottomPinType = sBottomPinType = ui->mBottomPinType->text();
  if( topPinType.isEmpty() )
    topPinType = bottomPinType;

  //Pin 1 reside at point 0,0

  //Bottom pins lenght
  int bottomPinsLenght = (bottomPinCount - 1) * bottomPinDistance;

  //Part left pos
  int partLeft = -( bodySizeX - bottomPinsLenght ) / 2 - bottomPinOffsetX;

  //Part right pos
  int partRight = partLeft + bodySizeX;

  //Top pins lenght
  int topPinsLenght = (topPinCount - 1) * topPinDistance;

  //Part right offset top right pin
  int pinRtop = partLeft + ((bodySizeX - topPinsLenght) / 2 + topPinOffsetX) + topPinsLenght;

  //Draw part
  addRect( partLeft, pinLen, partRight, pinSizeY - pinLen );

  //Draw pins
  if( pinLen > 0 ) {
    //Bottom pins
    for( int i = 0; i < bottomPinCount; i++ )
      addLine( i*bottomPinDistance,0, i*bottomPinDistance,pinLen );

    //Top pins
    for( int i = 0; i < topPinCount; i++ )
      addLine( pinRtop - i*topPinDistance,pinSizeY,  pinRtop - i*topPinDistance,pinSizeY - pinLen );
    }

  //Draw inside part first pin indicator
  addCircle( 0, pinLen + 500, 250 );

  //Add pins
  if( ui->mPlanar->isChecked() )
    setupSmdPin();
  else
    setupThrouPin();
  //Make pin number and pin name invisible
  mPinNameProp.mLayer.set( LID0_INVISIBLE );
  mPinNumberProp.mLayer.set( LID0_INVISIBLE );

  //Bottom pins
  mPinNumberProp.mHorz = dhjLeft;
  mPinNumberProp.mDir  = da90;
  mPinNameProp.mHorz   = dhjLeft;
  mPinNameProp.mDir    = da90;
  for( int i = 0; i < bottomPinCount; i++ ) {
    SdPoint pinOrg(i*bottomPinDistance,0);
    SdPoint numberOrg(pinOrg.x()-250,0);
    SdPoint nameOrg(pinOrg.x(), pinLen );
    addPin( pinOrg, bottomPinType, numberOrg, QString::number(i+1), nameOrg );
    }

  //Top pins
  mPinNumberProp.mHorz = dhjRight;
  mPinNameProp.mHorz   = dhjRight;
  for( int i = 0; i < topPinCount; i++ ) {
    SdPoint pinOrg(pinRtop - i*topPinDistance,pinSizeY);
    SdPoint numberOrg(pinOrg.x()-250,pinSizeY);
    SdPoint nameOrg(pinOrg.x(),pinSizeY-pinLen);
    addPin( pinOrg, topPinType, numberOrg, QString::number(i+1+bottomPinCount), nameOrg );
    }

  //Update ident position
  //When part size greater then ident text size (1000) then place ident in center of part
  // else place ident at top of part
  if( bodySizeY <= 3000 && bodySizeX > 3000 ) {
    mIdentProp.mDir = da0;
    setId( SdPoint( partLeft + bodySizeX / 2, pinSizeY/2 + 500 ) );
    mValueProp.mDir = da0;
    setValue( SdPoint( partLeft + bodySizeX / 2, pinSizeY/2 - 500 ) );
    }
  else {
    mIdentProp.mDir = da90;
    setId( SdPoint( partLeft+500, pinSizeY/2) );
//    setId( SdPoint( bodySizeY > 3000 ? partLeft - bodySizeX / 2 : partLeft+500, pinSizeY/2) );
    mValueProp.mDir = da90;
    setValue( SdPoint( partLeft + bodySizeX + 500, pinSizeY/2) );
    }

  SdDMasterPart::accept();
  }
