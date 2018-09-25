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
#include "objects/SdEnvir.h"

#include <QToolButton>

SdDMasterPartQuadSide::SdDMasterPartQuadSide(SdProjectItem *item, QWidget *parent) :
  SdDMasterPart( item, parent ),
  ui(new Ui::SdDMasterPartQuadSide)
  {
  ui->setupUi(this);

  ui->mBodySizeX->setText("4.6");
  ui->mBodySizeY->setText("3.0");
  ui->mPinSizeX->setText("5.0");
  ui->mPinSizeY->setText("5.0");
  ui->mPinDistance->setText("1.0");
  ui->mTotalPinCount->setText("16");

  //Left side
  ui->mLeftPinOffsetY->setText("0");
  ui->mLeftPinType->setText("pr2.0x0.6");

  //Bottom side
  ui->mBottomPinOffsetX->setText("0");
  ui->mBottomPinType->setText("pr0.6x2.0");

  //Right side
  ui->mRightPinOffsetY->setText("0");
  ui->mRightPinType->setText("pr2.0x0.6");

  //Top side
  ui->mTopPinOffsetX->setText("0");
  ui->mTopPinType->setText("pr0.6x2.0");

  onEditChanged( QString() );

  connect( ui->mBodySizeX, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );
  connect( ui->mBodySizeY, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );
  connect( ui->mPinSizeX, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );
  connect( ui->mPinSizeY, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );
  connect( ui->mPinDistance, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );
  connect( ui->mTotalPinCount, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );

  //Left side
  connect( ui->mLeftPinOffsetY, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );

  //Bottom side
  connect( ui->mBottomPinOffsetX, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );

  //Right side
  connect( ui->mRightPinOffsetY, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );

  //Top side
  connect( ui->mTopPinOffsetX, &QLineEdit::textEdited, this, &SdDMasterPartQuadSide::onEditChanged );

  connect( ui->mLeftPinTypeSelect, &QToolButton::clicked, this, [this] () {
    QString str = SdDPads::selectPinType(this);
    if( !str.isEmpty() )
      ui->mLeftPinType->setText( str );
    } );

  connect( ui->mBottomPinTypeSelect, &QToolButton::clicked, this, [this] () {
    QString str = SdDPads::selectPinType(this);
    if( !str.isEmpty() )
      ui->mBottomPinType->setText( str );
    } );

  connect( ui->mRightPinTypeSelect, &QToolButton::clicked, this, [this] () {
    QString str = SdDPads::selectPinType(this);
    if( !str.isEmpty() )
      ui->mRightPinType->setText( str );
    } );

  connect( ui->mTopPinTypeSelect, &QToolButton::clicked, this, [this] () {
    QString str = SdDPads::selectPinType(this);
    if( !str.isEmpty() )
      ui->mTopPinType->setText( str );
    } );
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
  int leftPinOffsetX  = sdEnvir->fromPhisPcb( ui->mLeftPinOffsetY->text() );
  if( leftPinCount < 1 ) leftPinCount = totalPinCount / 4;

  //Bottom side
  int bottomPinCount    = ui->mBottomPinCount->text().toInt();
  int bottomPinOffsetX  = sdEnvir->fromPhisPcb( ui->mBottomPinOffsetX->text() );
  if( bottomPinCount < 1 ) bottomPinCount = totalPinCount / 4;

  //Right side
  int rightPinCount    = ui->mRightPinCount->text().toInt();
  int rightPinOffsetX  = sdEnvir->fromPhisPcb( ui->mRightPinOffsetY->text() );
  if( rightPinCount < 1 ) rightPinCount = totalPinCount / 4;

  //Top side
  int topPinCount    = ui->mTopPinCount->text().toInt();
  int topPinOffsetX  = sdEnvir->fromPhisPcb( ui->mTopPinOffsetX->text() );
  if( topPinCount < 1 ) topPinCount = totalPinCount / 4;

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




//When accept we build part with current params
void SdDMasterPartQuadSide::accept()
  {

  }
