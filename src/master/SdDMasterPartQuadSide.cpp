/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part master for rectangle part with pins on all four sides
*/
#include "SdDMasterPartQuadSide.h"
#include "ui_SdDMasterPartQuadSide.h"
#include "windows/SdDPads.h"
#include "windows/SdDPadMaster.h"
#include "objects/SdEnvir.h"

#include <QToolButton>


static int
sBodySizeX = 6000,
sBodySizeY = 6000,
sPinSizeX = 7000,
sPinSizeY = 7000,
sPinDistance = 1000,
sLeftPinOffsetY = 0,
sBottomPinOffsetX = 0,
sRightPinOffsetY = 0,
sTopPinOffsetX = 0;

static QString
sTotalPinCount("16"),
sBottomLeftPinNumber("1"),
sLeftPinType("r1.2x0.3m0.1s0.04"),
sBottomPinType("r0.3x1.2m0.1s0.04"),
sRightPinType("r1.2x0.3m0.1s0.04"),
sTopPinType("r0.3x1.2m0.1s0.04");

SdDMasterPartQuadSide::SdDMasterPartQuadSide(SdProjectItem *item, QWidget *parent) :
  SdDMasterPart( item, parent ),
  ui(new Ui::SdDMasterPartQuadSide)
  {
  ui->setupUi(this);

  ui->mBodySizeX->setText( sdEnvir->toPhisPcb(sBodySizeX) );
  ui->mBodySizeY->setText( sdEnvir->toPhisPcb(sBodySizeY) );
  ui->mPinSizeX->setText( sdEnvir->toPhisPcb(sPinSizeX) );
  ui->mPinSizeY->setText( sdEnvir->toPhisPcb(sPinSizeY) );
  ui->mPinDistance->setText( sdEnvir->toPhisPcb(sPinDistance) );
  ui->mTotalPinCount->setText( sTotalPinCount );
  ui->mBottomLeftPinNumber->setText( sBottomLeftPinNumber );

  //Left side
  ui->mLeftPinOffsetY->setText( sdEnvir->toPhisPcb(sLeftPinOffsetY) );
  ui->mLeftPinType->setText( sLeftPinType );

  //Bottom side
  ui->mBottomPinOffsetX->setText( sdEnvir->toPhisPcb(sBottomPinOffsetX) );
  ui->mBottomPinType->setText( sBottomPinType );

  //Right side
  ui->mRightPinOffsetY->setText( sdEnvir->toPhisPcb(sRightPinOffsetY) );
  ui->mRightPinType->setText( sRightPinType );

  //Top side
  ui->mTopPinOffsetX->setText( sdEnvir->toPhisPcb(sTopPinOffsetX) );
  ui->mTopPinType->setText( sTopPinType );

  //By default - planar pins
  ui->mPlanar->setChecked(true);

  onEditChanged( QString() );

  connect( ui->mBodySizeX, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );
  connect( ui->mBodySizeY, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );
  connect( ui->mPinSizeX, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );
  connect( ui->mPinSizeY, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );
  connect( ui->mPinDistance, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );
  connect( ui->mTotalPinCount, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );

  //Left side
  connect( ui->mLeftPinOffsetY, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );
  connect( ui->mLeftPinCount, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );

  //Bottom side
  connect( ui->mBottomPinOffsetX, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );
  connect( ui->mBottomPinCount, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );

  //Right side
  connect( ui->mRightPinOffsetY, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );
  connect( ui->mRightPinCount, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );

  //Top side
  connect( ui->mTopPinOffsetX, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );
  connect( ui->mTopPinCount, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );

  connect( ui->mLeftPinTypeSelect, &QToolButton::clicked, this, [this] () {
    ui->mLeftPinType->setText( SdDPadMaster::build( ui->mLeftPinType->text(), this ) );
    } );

  connect( ui->mBottomPinTypeSelect, &QToolButton::clicked, this, [this] () {
    ui->mBottomPinType->setText( SdDPadMaster::build( ui->mBottomPinType->text(), this ) );
    } );

  connect( ui->mRightPinTypeSelect, &QToolButton::clicked, this, [this] () {
    ui->mRightPinType->setText( SdDPadMaster::build( ui->mRightPinType->text(), this ) );
    } );

  connect( ui->mTopPinTypeSelect, &QToolButton::clicked, this, [this] () {
    ui->mTopPinType->setText( SdDPadMaster::build( ui->mTopPinType->text(), this ) );
    } );

  connect( ui->buttonBox, &QDialogButtonBox::accepted, this, &SdDMasterPartQuadSide::accept );
  connect( ui->buttonBox, &QDialogButtonBox::rejected, this, &SdDMasterPartQuadSide::reject );
  }




SdDMasterPartQuadSide::~SdDMasterPartQuadSide()
  {
  delete ui;
  }




//Update preview on any params changed
void SdDMasterPartQuadSide::onEditChanged(const QString txt)
  {
  Q_UNUSED(txt)

  SdIllustrator il;

  drawPart( il );
  il.setPixmap( 400, 400, Qt::white );
  drawPart( il );
  ui->mPreview->setPixmap( il.pixmap() );
  }




void SdDMasterPartQuadSide::changeEvent(QEvent *e)
  {
  QDialog::changeEvent(e);
  switch (e->type()) {
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
      break;
    default:
      break;
    }
  }





//Draw part preview
void SdDMasterPartQuadSide::drawPart(SdIllustrator &il)
  {
  int bodySizeX = sdEnvir->fromPhisPcb( ui->mBodySizeX->text() );
  int bodySizeY = sdEnvir->fromPhisPcb( ui->mBodySizeY->text() );
  int pinSizeX  = sdEnvir->fromPhisPcb( ui->mPinSizeX->text() );
  int pinSizeY  = sdEnvir->fromPhisPcb( ui->mPinSizeY->text() );
  int pinLenY = (pinSizeY - bodySizeY) / 2;
  int pinLenX = (pinSizeX - bodySizeX) / 2;
  int pinDistance = sdEnvir->fromPhisPcb( ui->mPinDistance->text() );
  int totalPinCount = ui->mTotalPinCount->text().toInt();
  if( totalPinCount < 4 ) totalPinCount = 4;

  //Left side
  int leftPinCount    = ui->mLeftPinCount->text().toInt();
  int leftPinOffsetY  = sdEnvir->fromPhisPcb( ui->mLeftPinOffsetY->text() );
  if( leftPinCount < 1 ) leftPinCount = totalPinCount / 4;

  //Bottom side
  int bottomPinCount    = ui->mBottomPinCount->text().toInt();
  int bottomPinOffsetX  = sdEnvir->fromPhisPcb( ui->mBottomPinOffsetX->text() );
  if( bottomPinCount < 1 ) bottomPinCount = totalPinCount / 4;

  //Right side
  int rightPinCount    = ui->mRightPinCount->text().toInt();
  int rightPinOffsetY  = sdEnvir->fromPhisPcb( ui->mRightPinOffsetY->text() );
  if( rightPinCount < 1 ) rightPinCount = totalPinCount / 4;

  //Top side
  int topPinCount    = ui->mTopPinCount->text().toInt();
  int topPinOffsetX  = sdEnvir->fromPhisPcb( ui->mTopPinOffsetX->text() );
  if( topPinCount < 1 ) topPinCount = totalPinCount / 4;

  QColor red("red");
  QColor green("green");

  int leftPinSize   = (leftPinCount - 1) * pinDistance;
  int bottomPinSize = (bottomPinCount - 1) * pinDistance;
  int rightPinSize  = (rightPinCount - 1) * pinDistance;
  int topPinSize    = (topPinCount - 1) * pinDistance;

  //Pin 1 reside at point 0,0 it is bottom left pin
  int bottomPinX = 0;
  int bottomPin = 0;
  int bottomBody = bottomPin + pinLenY;
  int topPin = pinSizeY;
  int topBody = topPin - pinLenY;
  int leftBody = -(bodySizeX - bottomPinSize) / 2 - bottomPinOffsetX;
  int leftPin = leftBody - pinLenX;
  int rightPin = leftPin + pinSizeX;
  int rightBody = rightPin - pinLenX;
  int leftPinY = topBody - (bodySizeY - leftPinSize) / 2 - leftPinOffsetY;
  int rightPinY = bottomBody + (bodySizeY - rightPinSize) / 2 + rightPinOffsetY;
  int topPinX = rightBody - (bodySizeX - topPinSize) / 2 - topPinOffsetX;

  //Draw part
  il.drawRect( leftBody, topBody, rightBody, bottomBody, red );

  //Draw pins
  if( pinLenX > 0 ) {
    //Left pins
    for( int i = 0; i < leftPinCount; i++ )
      il.drawLine( leftPin,leftPinY-i*pinDistance, leftBody,leftPinY-i*pinDistance, red );

    //Right pins
    for( int i = 0; i < rightPinCount; i++ )
      il.drawLine( rightBody,rightPinY+i*pinDistance, rightPin,rightPinY+i*pinDistance, red );
    }

  if( pinLenX > 0 ) {
    //Bottom pins
    for( int i = 0; i < bottomPinCount; i++ )
      il.drawLine( bottomPinX+i*pinDistance,bottomBody, bottomPinX+i*pinDistance,bottomPin, red );

    //Top pins
    for( int i = 0; i < topPinCount; i++ )
      il.drawLine( topPinX - i*pinDistance,topBody,  topPinX - i*pinDistance,topPin, red );
    }

  //Draw pin connectors
  int crossSize = pinDistance / 3;
  //Left pins
  for( int i = 0; i < leftPinCount; i++ )
    il.drawCross( leftPin,leftPinY-i*pinDistance, crossSize, green );
  //Bottom pins
  for( int i = 0; i < bottomPinCount; i++ )
    il.drawCross( bottomPinX+i*pinDistance,bottomPin, crossSize, green );
  //Right pins
  for( int i = 0; i < rightPinCount; i++ )
    il.drawCross( rightPin,rightPinY+i*pinDistance, crossSize, green );
  //Top pins
  for( int i = 0; i < topPinCount; i++ )
    il.drawCross( topPinX - i*pinDistance,topPin, crossSize, green );
  }




//When accept we build part with current params
void SdDMasterPartQuadSide::accept()
  {
  int bodySizeX = sBodySizeX = sdEnvir->fromPhisPcb( ui->mBodySizeX->text() );
  int bodySizeY = sBodySizeY = sdEnvir->fromPhisPcb( ui->mBodySizeY->text() );
  int pinSizeX  = sPinSizeX = sdEnvir->fromPhisPcb( ui->mPinSizeX->text() );
  int pinSizeY  = sPinSizeY = sdEnvir->fromPhisPcb( ui->mPinSizeY->text() );
  int pinLenY = (pinSizeY - bodySizeY) / 2;
  int pinLenX = (pinSizeX - bodySizeX) / 2;
  int pinDistance = sPinDistance = sdEnvir->fromPhisPcb( ui->mPinDistance->text() );
  sTotalPinCount = ui->mTotalPinCount->text();
  int totalPinCount = sTotalPinCount.toInt();
  if( totalPinCount < 4 ) totalPinCount = 4;

  //Left side
  int leftPinCount    = ui->mLeftPinCount->text().toInt();
  int leftPinOffsetY  = sLeftPinOffsetY = sdEnvir->fromPhisPcb( ui->mLeftPinOffsetY->text() );
  if( leftPinCount < 1 ) leftPinCount = totalPinCount / 4;

  //Bottom side
  int bottomPinCount    = ui->mBottomPinCount->text().toInt();
  int bottomPinOffsetX  = sBottomPinOffsetX = sdEnvir->fromPhisPcb( ui->mBottomPinOffsetX->text() );
  if( bottomPinCount < 1 ) bottomPinCount = totalPinCount / 4;

  //Right side
  int rightPinCount    = ui->mRightPinCount->text().toInt();
  int rightPinOffsetY  = sRightPinOffsetY = sdEnvir->fromPhisPcb( ui->mRightPinOffsetY->text() );
  if( rightPinCount < 1 ) rightPinCount = totalPinCount / 4;

  //Top side
  int topPinCount    = ui->mTopPinCount->text().toInt();
  int topPinOffsetX  = sTopPinOffsetX = sdEnvir->fromPhisPcb( ui->mTopPinOffsetX->text() );
  if( topPinCount < 1 ) topPinCount = totalPinCount / 4;

  totalPinCount = leftPinCount + bottomPinCount + rightPinCount + topPinCount;
  //Pin types
  QString bottomPinType = sBottomPinType = ui->mBottomPinType->text();
  QString topPinType = sTopPinType = ui->mTopPinType->text();
  if( topPinType.isEmpty() )
    topPinType = bottomPinType;
  QString leftPinType = sLeftPinType = ui->mLeftPinType->text();
  QString rightPinType = sRightPinType = ui->mRightPinType->text();
  if( rightPinType.isEmpty() )
    rightPinType = leftPinType;

  int leftPinSize   = (leftPinCount - 1) * pinDistance;
  int bottomPinSize = (bottomPinCount - 1) * pinDistance;
  int rightPinSize  = (rightPinCount - 1) * pinDistance;
  int topPinSize    = (topPinCount - 1) * pinDistance;

  //Pin 1 reside at point 0,0 it is bottom left pin
  int bottomPinX = 0;
  int bottomPin = 0;
  int bottomBody = bottomPin + pinLenY;
  int topPin = pinSizeY;
  int topBody = topPin - pinLenY;
  int leftBody = -(bodySizeX - bottomPinSize) / 2 - bottomPinOffsetX;
  int leftPin = leftBody - pinLenX;
  int rightPin = leftPin + pinSizeX;
  int rightBody = rightPin - pinLenX;
  int leftPinY = topBody - (bodySizeY - leftPinSize) / 2 - leftPinOffsetY;
  int rightPinY = bottomBody + (bodySizeY - rightPinSize) / 2 + rightPinOffsetY;
  int topPinX = rightBody - (bodySizeX - topPinSize) / 2 - topPinOffsetX;

  //Draw part
  addRect( leftBody, topBody, rightBody, bottomBody );

  //Draw inside part first pin indicator
  addCircle( leftBody + 500, bottomBody + 500, 250 );

  //Draw pins
  if( pinLenX > 0 ) {
    //Left pins
    for( int i = 0; i < leftPinCount; i++ )
      addLine( leftPin,leftPinY-i*pinDistance, leftBody,leftPinY-i*pinDistance );

    //Right pins
    for( int i = 0; i < rightPinCount; i++ )
      addLine( rightBody,rightPinY+i*pinDistance, rightPin,rightPinY+i*pinDistance );
    }

  if( pinLenY > 0 ) {
    //Bottom pins
    for( int i = 0; i < bottomPinCount; i++ )
      addLine( bottomPinX+i*pinDistance,bottomBody, bottomPinX+i*pinDistance,bottomPin );

    //Top pins
    for( int i = 0; i < topPinCount; i++ )
      addLine( topPinX - i*pinDistance,topBody,  topPinX - i*pinDistance,topPin );
    }

  //Add pins
  if( ui->mPlanar->isChecked() )
    setupSmdPin();
  else
    setupThrouPin();
  //Make pin number and pin name invisible
  mMasterPart.mPinNameProp.mLayer.set( LID0_INVISIBLE );
  mMasterPart.mPinNumberProp.mLayer.set( LID0_INVISIBLE );

  int pinIndex = ui->mBottomLeftPinNumber->text().toInt();
  //Bottom pins
  mMasterPart.mPinNumberProp.mHorz = dhjLeft;
  mMasterPart.mPinNameProp.mHorz   = dhjLeft;
  mMasterPart.mPinNumberProp.mDir  = da90;
  mMasterPart.mPinNameProp.mDir    = da90;
  for( int i = 0; i < bottomPinCount; i++ ) {
    SdPoint pinOrg(bottomPinX+i*pinDistance,bottomPin);
    SdPoint numberOrg(pinOrg.x()-250,pinOrg.y());
    SdPoint nameOrg(pinOrg.x(), pinOrg.y()+pinLenY );
    addPin( pinOrg, bottomPinType, numberOrg, QString::number(pinIndex), nameOrg );
    pinIndex++;
    if( pinIndex > totalPinCount )
      pinIndex = 1;
    }
  //Right pins
  mMasterPart.mPinNumberProp.mHorz = dhjRight;
  mMasterPart.mPinNumberProp.mDir  = da0;
  mMasterPart.mPinNameProp.mHorz   = dhjRight;
  mMasterPart.mPinNameProp.mDir    = da0;
  for( int i = 0; i < rightPinCount; i++ ) {
    SdPoint pinOrg(rightPin,rightPinY+i*pinDistance);
    SdPoint numberOrg(pinOrg.x(),pinOrg.y()+250);
    SdPoint nameOrg(pinOrg.x()-pinLenX, pinOrg.y() );
    addPin( pinOrg, rightPinType, numberOrg, QString::number(pinIndex), nameOrg );
    pinIndex++;
    if( pinIndex > totalPinCount )
      pinIndex = 1;
    }
  //Top pins
  mMasterPart.mPinNumberProp.mDir  = da90;
  mMasterPart.mPinNameProp.mDir    = da90;
  for( int i = 0; i < topPinCount; i++ ) {
    SdPoint pinOrg(topPinX - i*pinDistance,topPin);
    SdPoint numberOrg(pinOrg.x()-250,pinOrg.y());
    SdPoint nameOrg(pinOrg.x(), pinOrg.y()-pinLenY );
    addPin( pinOrg, topPinType, numberOrg, QString::number(pinIndex), nameOrg );
    pinIndex++;
    if( pinIndex > totalPinCount )
      pinIndex = 1;
    }
  //Left pins
  mMasterPart.mPinNumberProp.mHorz = dhjLeft;
  mMasterPart.mPinNumberProp.mDir  = da0;
  mMasterPart.mPinNameProp.mHorz   = dhjLeft;
  mMasterPart.mPinNameProp.mDir    = da0;
  for( int i = 0; i < leftPinCount; i++ ) {
    SdPoint pinOrg(leftPin,leftPinY-i*pinDistance);
    SdPoint numberOrg(pinOrg.x(),pinOrg.y()+250);
    SdPoint nameOrg(pinOrg.x()+pinLenX, pinOrg.y() );
    addPin( pinOrg, leftPinType, numberOrg, QString::number(pinIndex), nameOrg );
    pinIndex++;
    if( pinIndex > totalPinCount )
      pinIndex = 1;
    }

  //Update ident position
  //When part size greater then ident text size (1000) then place ident in center of part
  // else place ident at top of part
  mMasterPart.mIdentProp.mDir = da0;
  mMasterPart.mValueProp.mDir = da0;
  if( bodySizeX >= 3000 && bodySizeY >= 3000 )
    //id inside part
    setId( SdPoint( leftBody + bodySizeX / 2, (topBody + bottomBody) / 2 ) );
  else
    //id outside part
    setId( SdPoint( leftBody + bodySizeX / 2, topPin + 500 ) );

  if( bodySizeX >= 15000 && bodySizeY >= 3000 )
    //value inside body
    setValue( SdPoint( leftBody + bodySizeX / 2, bottomBody + 500 ) );
  else
    //value outside body
    setValue( SdPoint( leftBody + bodySizeX / 2, bottomPin - 500 ) );

  SdDMasterPart::accept();
  }
