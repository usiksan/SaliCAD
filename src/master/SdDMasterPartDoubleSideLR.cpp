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

  ui->mBodySizeY->setText( SdEnvir::instance()->toPhisPcb(sBodySizeY) );
  ui->mBodySizeX->setText( SdEnvir::instance()->toPhisPcb(sBodySizeX) );
  ui->mPinSizeX->setText( SdEnvir::instance()->toPhisPcb(sPinSizeX) );

  //Left side
  ui->mLeftPinCount->setText( sLeftPinCount );
  ui->mLeftPinDistance->setText( SdEnvir::instance()->toPhisPcb(sLeftPinDistance) );
  ui->mLeftPinOffsetY->setText( SdEnvir::instance()->toPhisPcb(sLeftPinOffsetY) );
  ui->mLeftPinType->setText( sLeftPinType );

  //Right side
  ui->mRightPinCount->setText( sRightPinCount );
  ui->mRightPinDistance->setText( SdEnvir::instance()->toPhisPcb(sRightPinDistance) );
  ui->mRightPinOffsetY->setText( SdEnvir::instance()->toPhisPcb(sRightPinOffsetY) );
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


  connect( ui->mLeftPinTypeSelect, &QToolButton::clicked, this, [this] () {
    ui->mLeftPinType->setText( SdDPadMaster::build( ui->mLeftPinType->text(), this ) );
    } );

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
  int bodySizeX = SdEnvir::instance()->fromPhisPcb( ui->mBodySizeX->text() );
  int bodySizeY = SdEnvir::instance()->fromPhisPcb( ui->mBodySizeY->text() );
  int pinSizeX  = SdEnvir::instance()->fromPhisPcb( ui->mPinSizeX->text() );
  int pinLen = (pinSizeX - bodySizeX) / 2;

  int leftPinCount    = ui->mLeftPinCount->text().toInt();
  int leftPinDistance = SdEnvir::instance()->fromPhisPcb( ui->mLeftPinDistance->text() );
  int leftPinOffsetY  = SdEnvir::instance()->fromPhisPcb( ui->mLeftPinOffsetY->text() );

  int rightPinCount    = ui->mRightPinCount->text().toInt();
  int rightPinDistance = SdEnvir::instance()->fromPhisPcb( ui->mRightPinDistance->text() );
  int rightPinOffsetY  = SdEnvir::instance()->fromPhisPcb( ui->mRightPinOffsetY->text() );

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
  sBodySizeX = SdEnvir::instance()->fromPhisPcb( ui->mBodySizeX->text() );
  sBodySizeY = SdEnvir::instance()->fromPhisPcb( ui->mBodySizeY->text() );
  sPinSizeX  = SdEnvir::instance()->fromPhisPcb( ui->mPinSizeX->text() );
  int pinLen = (sPinSizeX - sBodySizeX) / 2;

  sLeftPinCount = ui->mLeftPinCount->text();
  int leftPinCount    = sLeftPinCount.toInt();
  sLeftPinDistance = SdEnvir::instance()->fromPhisPcb( ui->mLeftPinDistance->text() );
  sLeftPinOffsetY  = SdEnvir::instance()->fromPhisPcb( ui->mLeftPinOffsetY->text() );

  sRightPinCount = ui->mRightPinCount->text();
  int rightPinCount    = sRightPinCount.toInt();
  sRightPinDistance = SdEnvir::instance()->fromPhisPcb( ui->mRightPinDistance->text() );
  sRightPinOffsetY  = SdEnvir::instance()->fromPhisPcb( ui->mRightPinOffsetY->text() );

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

  //Draw inside part first pin indicator
  addCircle( pinLen + 500, 0, 250 );

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
  mMasterPart.mPinNameProp.mLayer.set( LID0_INVISIBLE );
  mMasterPart.mPinNumberProp.mLayer.set( LID0_INVISIBLE );

  //Left pins
  mMasterPart.mPinNumberProp.mHorz = dhjLeft;
  mMasterPart.mPinNameProp.mHorz   = dhjLeft;
  for( int i = 0; i < leftPinCount; i++ ) {
    SdPoint pinOrg(0,-i*sLeftPinDistance);
    SdPoint numberOrg(0,pinOrg.y()+250);
    SdPoint nameOrg(pinLen,pinOrg.y());
    addPin( pinOrg, sLeftPinType, numberOrg, QString::number(i+1), nameOrg );
    }

  //Right pins
  mMasterPart.mPinNumberProp.mHorz = dhjRight;
  mMasterPart.mPinNameProp.mHorz   = dhjRight;
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
    mMasterPart.mIdentProp.mDir    = da90;
    setId( SdPoint( sPinSizeX/2, partTop - sBodySizeY / 2 ) );
    }
  else setId( SdPoint( sPinSizeX/2, sBodySizeX > 3000 ? partTop - sBodySizeY / 2 : partTop+500) );

  mMasterPart.mValueProp.mDir = da0;
  setValue( SdPoint( sPinSizeX / 2, partBot - 500 ) );

  SdDMasterPart::accept();
  }
