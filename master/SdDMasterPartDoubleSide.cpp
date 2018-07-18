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
#include "SdDMasterPartDoubleSide.h"
#include "ui_SdDMasterPartDoubleSide.h"
#include "objects/SdEnvir.h"


SdDMasterPartDoubleSide::SdDMasterPartDoubleSide(SdProjectItem *item, QWidget *parent) :
  SdDMasterPart( item, parent ),
  ui(new Ui::SdDMasterPartDoubleSide)
  {
  ui->setupUi(this);

  ui->mBodySizeX->setText("3.0");
  ui->mBodySizeY->setText("4.6");
  ui->mPinSizeX->setText("5.0");

  //Left side
  ui->mLeftPinCount->setText("2");
  ui->mLeftPinDistance->setText("1.0");
  ui->mLeftPinOffsetY->setText("0");
  ui->mLeftPinType->setText("pr2.0x0.6");

  //Right side
  ui->mRightPinCount->setText("2");
  ui->mRightPinDistance->setText("1.0");
  ui->mRightPinOffsetY->setText("0");
  ui->mRightPinType->setText("pr2.0x0.6");

  onEditChanged( QString() );

  connect( ui->mBodySizeX, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSide::onEditChanged );
  connect( ui->mBodySizeY, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSide::onEditChanged );
  connect( ui->mPinSizeX, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSide::onEditChanged );

  connect( ui->mLeftPinCount, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSide::onEditChanged );
  connect( ui->mLeftPinDistance, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSide::onEditChanged );
  connect( ui->mLeftPinOffsetY, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSide::onEditChanged );

  connect( ui->mRightPinCount, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSide::onEditChanged );
  connect( ui->mRightPinDistance, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSide::onEditChanged );
  connect( ui->mRightPinOffsetY, &QLineEdit::textEdited, this, &SdDMasterPartDoubleSide::onEditChanged );
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



void SdDMasterPartDoubleSide::accept()
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

  //Pin types
  QString leftPinType = ui->mLeftPinType->text();
  QString rightPinType = ui->mRightPinType->text();
  if( rightPinType.isEmpty() )
    rightPinType = leftPinType;

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
  addRect( pinLen, partTop, pinSizeX - pinLen, partBot );

  //Draw pins
  if( pinLen > 0 ) {
    //Left pins
    for( int i = 0; i < leftPinCount; i++ )
      addLine( 0,-i*leftPinDistance, pinLen,-i*leftPinDistance );

    //Right pins
    for( int i = 0; i < rightPinCount; i++ )
      addLine( pinSizeX, pinRbot + i*rightPinDistance, pinSizeX - pinLen, pinRbot + i*rightPinDistance );
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
    SdPoint pinOrg(0,-i*leftPinDistance);
    SdPoint numberOrg(0,pinOrg.y()+250);
    SdPoint nameOrg(pinLen,pinOrg.y());
    addPin( pinOrg, leftPinType, numberOrg, QString::number(i+1), nameOrg );
    }

  //Right pins
  mPinNumberProp.mHorz = dhjRight;
  mPinNameProp.mHorz   = dhjRight;
  for( int i = 0; i < rightPinCount; i++ ) {
    SdPoint pinOrg(pinSizeX, pinRbot + i*rightPinDistance);
    SdPoint numberOrg(pinSizeX,pinOrg.y()+250);
    SdPoint nameOrg(pinSizeX-pinLen,pinOrg.y());
    addPin( pinOrg, leftPinType, numberOrg, QString::number(i+1+leftPinCount), nameOrg );
    }

  //Update ident position
  //When part size greater then ident text size (1000) then place ident in center of part
  // else place ident at top of part
  if( bodySizeX <= 3000 && bodySizeY > 3000 ) {
    mIdentProp.mDir    = da90;
    setId( SdPoint( pinSizeX/2, partTop - bodySizeY / 2 ), QString("Id") );
    }
  else setId( SdPoint( pinSizeX/2, bodySizeX > 3000 ? partTop - bodySizeY / 2 : partTop+500), QString("Id") );

  SdDMasterPart::accept();
  }
