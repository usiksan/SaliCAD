/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part master with rectangle body and two pins columns by left and right sides
*/
#include "SdDMasterPartDoubleSideLR.h"
#include "ui_SdDMasterPartDoubleSideLR.h"
#include "windows/SdDPads.h"
#include "windows/SdDPadMaster.h"
#include "objects/SdEnvir.h"

#include <QToolButton>

static int
sBodySizeX = 4600,
sBodySizeY = 3000,
sPinSizeX = 5000,
sLeftPinDistance = 1000,
sRightPinDistance = 1000,
sLeftPinOffsetY = 0,
sRightPinOffsetY = 0;

static QString
sLeftPinCount("2"),
sRightPinCount("2"),
sLeftPinType("r1.5x0.4m0.1s0.04"),
sRightPinType("r1.5x0.4m0.1s0.04");


SdDMasterPartDoubleSideLR::SdDMasterPartDoubleSideLR(SdProjectItem *item, QWidget *parent) :
  SdDMasterPart( item, parent ),
  ui(new Ui::SdDMasterPartDoubleSideLR)
  {
  ui->setupUi(this);

  ui->mBodySizeY->setText( sdEnvir->toPhisPcb(sBodySizeY) );
  ui->mBodySizeX->setText( sdEnvir->toPhisPcb(sBodySizeX) );
  ui->mPinSizeX->setText( sdEnvir->toPhisPcb(sPinSizeX) );

  //Left side
  ui->mLeftPinCount->setText( sLeftPinCount );
  ui->mLeftPinDistance->setText( sdEnvir->toPhisPcb(sLeftPinDistance) );
  ui->mLeftPinOffsetY->setText( sdEnvir->toPhisPcb(sLeftPinOffsetY) );
  ui->mLeftPinType->setText( sLeftPinType );

  //Right side
  ui->mRightPinCount->setText( sRightPinCount );
  ui->mRightPinDistance->setText( sdEnvir->toPhisPcb(sRightPinDistance) );
  ui->mRightPinOffsetY->setText( sdEnvir->toPhisPcb(sRightPinOffsetY) );
  ui->mRightPinType->setText( sRightPinType );

  onEditChanged( QString() );

  connect( ui->mBodySizeX, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSideLR::onEditChanged );
  connect( ui->mBodySizeY, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSideLR::onEditChanged );
  connect( ui->mPinSizeX, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSideLR::onEditChanged );

  connect( ui->mLeftPinCount, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSideLR::onEditChanged );
  connect( ui->mLeftPinDistance, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSideLR::onEditChanged );
  connect( ui->mLeftPinOffsetY, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSideLR::onEditChanged );

  connect( ui->mRightPinCount, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSideLR::onEditChanged );
  connect( ui->mRightPinDistance, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSideLR::onEditChanged );
  connect( ui->mRightPinOffsetY, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSideLR::onEditChanged );


  ui->mLeftPinType->setReadOnly(true);
  connect( ui->mLeftPinTypeSelect, &QToolButton::clicked, this, [this] () {
    ui->mLeftPinType->setText( SdDPadMaster::build( ui->mLeftPinType->text(), this ) );
    } );

  ui->mRightPinType->setReadOnly(true);
  connect( ui->mRightPinTypeSelect, &QToolButton::clicked, this, [this] () {
    ui->mRightPinType->setText( SdDPadMaster::build( ui->mRightPinType->text(), this ) );
    } );


  connect( ui->buttonBox, &QDialogButtonBox::accepted, this, &SdDMasterPartDoubleSideLR::accept );
  connect( ui->buttonBox, &QDialogButtonBox::rejected, this, &SdDMasterPartDoubleSideLR::reject );
  }



SdDMasterPartDoubleSideLR::~SdDMasterPartDoubleSideLR()
  {
  delete ui;
  }




//Update preview on any params changed
void SdDMasterPartDoubleSideLR::onEditChanged(const QString txt)
  {
  Q_UNUSED(txt)

  SdIllustrator il;

  drawPart( il );
  il.setPixmap( 250, 250, Qt::white );
  drawPart( il );
  ui->mPreview->setPixmap( il.pixmap() );
  }




//Draw part preview
void SdDMasterPartDoubleSideLR::drawPart(SdIllustrator &il)
  {
  int bodySizeX = sdEnvir->fromPhisPcb( ui->mBodySizeX->text() );
  int bodySizeY = sdEnvir->fromPhisPcb( ui->mBodySizeY->text() );
  int pinSizeX  = sdEnvir->fromPhisPcb( ui->mPinSizeX->text() );
  int pinLen = (pinSizeX - bodySizeX) / 2;

  int leftPinCount    = ui->mLeftPinCount->text().toInt();
  int leftPinDistance = sdEnvir->fromPhisPcb( ui->mLeftPinDistance->text() );
  int leftPinOffsetY  = sdEnvir->fromPhisPcb( ui->mLeftPinOffsetY->text() );

  int rightPinCount    = ui->mRightPinCount->text().toInt();
  int rightPinDistance = sdEnvir->fromPhisPcb( ui->mRightPinDistance->text() );
  int rightPinOffsetY  = sdEnvir->fromPhisPcb( ui->mRightPinOffsetY->text() );

  QColor red("red");
  QColor green("green");

  //Pin 1 reside at point 0,0

  //Left pins lenght
  int leftPinsLenght = (leftPinCount - 1) * leftPinDistance;

  //Part top pos
  int partTop = ( bodySizeY - leftPinsLenght ) / 2 - leftPinOffsetY;

  //Part bot pos
  int partBot = partTop - bodySizeY;

  //Right pins lenght
  int rightPinsLenght = (rightPinCount - 1) * rightPinDistance;

  //Part bot offset low right pin
  int pinRbot = partTop - ((bodySizeY - rightPinsLenght) / 2 - rightPinOffsetY) - rightPinsLenght;

  //Draw part
  il.drawRect( pinLen, partTop, pinSizeX - pinLen, partBot, red );

  //Draw pins
  if( pinLen > 0 ) {
    //Left pins
    for( int i = 0; i < leftPinCount; i++ )
      il.drawLine( 0,-i*leftPinDistance, pinLen,-i*leftPinDistance, red );

    //Right pins
    for( int i = 0; i < rightPinCount; i++ )
      il.drawLine( pinSizeX, pinRbot + i*rightPinDistance, pinSizeX - pinLen, pinRbot + i*rightPinDistance, red );
    }

  //Draw pin connectors
  int crossSize = bodySizeY / 20;
  //Left pins
  for( int i = 0; i < leftPinCount; i++ )
    il.drawCross( 0,-i*leftPinDistance, crossSize, green );
  //Right pins
  for( int i = 0; i < rightPinCount; i++ )
    il.drawCross( pinSizeX, pinRbot + i*rightPinDistance, crossSize, green );
  }



void SdDMasterPartDoubleSideLR::accept()
  {
  sBodySizeX = sdEnvir->fromPhisPcb( ui->mBodySizeX->text() );
  sBodySizeY = sdEnvir->fromPhisPcb( ui->mBodySizeY->text() );
  sPinSizeX  = sdEnvir->fromPhisPcb( ui->mPinSizeX->text() );
  int pinLen = (sPinSizeX - sBodySizeX) / 2;

  sLeftPinCount = ui->mLeftPinCount->text();
  int leftPinCount    = sLeftPinCount.toInt();
  sLeftPinDistance = sdEnvir->fromPhisPcb( ui->mLeftPinDistance->text() );
  sLeftPinOffsetY  = sdEnvir->fromPhisPcb( ui->mLeftPinOffsetY->text() );

  sRightPinCount = ui->mRightPinCount->text();
  int rightPinCount    = sRightPinCount.toInt();
  sRightPinDistance = sdEnvir->fromPhisPcb( ui->mRightPinDistance->text() );
  sRightPinOffsetY  = sdEnvir->fromPhisPcb( ui->mRightPinOffsetY->text() );

  //Pin types
  sLeftPinType = ui->mLeftPinType->text();
  sRightPinType = ui->mRightPinType->text();
  if( sRightPinType.isEmpty() )
    sRightPinType = sLeftPinType;

  //Pin 1 reside at point 0,0

  //Left pins lenght
  int leftPinsLenght = (leftPinCount - 1) * sLeftPinDistance;

  //Part top pos
  int partTop = ( sBodySizeY - leftPinsLenght ) / 2 - sLeftPinOffsetY;

  //Part bot pos
  int partBot = partTop - sBodySizeY;

  //Right pins lenght
  int rightPinsLenght = (rightPinCount - 1) * sRightPinDistance;

  //Part bot offset low right pin
  int pinRbot = partTop - ((sBodySizeY - rightPinsLenght) / 2 - sRightPinOffsetY) - rightPinsLenght;

  //Draw part
  addRect( pinLen, partTop, sPinSizeX - pinLen, partBot );

  //Draw pins
  if( pinLen > 0 ) {
    //Left pins
    for( int i = 0; i < leftPinCount; i++ )
      addLine( 0,-i*sLeftPinDistance, pinLen,-i*sLeftPinDistance );

    //Right pins
    for( int i = 0; i < rightPinCount; i++ )
      addLine( sPinSizeX, pinRbot + i*sRightPinDistance, sPinSizeX - pinLen, pinRbot + i*sRightPinDistance );
    }

  //Add pins

  //Add pins
  if( ui->mPlanar->isChecked() )
    setupSmdPin();
  else
    setupThrouPin();
  //Make pin number and pin name invisible
  mPinNameProp.mLayer.set( LID0_INVISIBLE );
  mPinNumberProp.mLayer.set( LID0_INVISIBLE );

  //Left pins
  mPinNumberProp.mHorz = dhjLeft;
  mPinNameProp.mHorz   = dhjLeft;
  for( int i = 0; i < leftPinCount; i++ ) {
    SdPoint pinOrg(0,-i*sLeftPinDistance);
    SdPoint numberOrg(0,pinOrg.y()+250);
    SdPoint nameOrg(pinLen,pinOrg.y());
    addPin( pinOrg, sLeftPinType, numberOrg, QString::number(i+1), nameOrg );
    }

  //Right pins
  mPinNumberProp.mHorz = dhjRight;
  mPinNameProp.mHorz   = dhjRight;
  for( int i = 0; i < rightPinCount; i++ ) {
    SdPoint pinOrg(sPinSizeX, pinRbot + i*sRightPinDistance);
    SdPoint numberOrg(sPinSizeX,pinOrg.y()+250);
    SdPoint nameOrg(sPinSizeX-pinLen,pinOrg.y());
    addPin( pinOrg, sLeftPinType, numberOrg, QString::number(i+1+leftPinCount), nameOrg );
    }

  //Update ident position
  //When part size greater then ident text size (1000) then place ident in center of part
  // else place ident at top of part
  if( sBodySizeX <= 3000 && sBodySizeY > 3000 ) {
    mIdentProp.mDir    = da90;
    setId( SdPoint( sPinSizeX/2, partTop - sBodySizeY / 2 ) );
    }
  else setId( SdPoint( sPinSizeX/2, sBodySizeX > 3000 ? partTop - sBodySizeY / 2 : partTop+500) );

  mValueProp.mDir = da0;
  setValue( SdPoint( sPinSizeX / 2, partBot - 500 ) );

  SdDMasterPart::accept();
  }
