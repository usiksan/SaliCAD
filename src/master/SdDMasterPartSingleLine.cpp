/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part master with rectangle body and single pins line (row)
*/
#include "SdDMasterPartSingleLine.h"
#include "ui_SdDMasterPartSingleLine.h"
#include "windows/SdDPads.h"
#include "windows/SdDPadMaster.h"
#include "objects/SdEnvir.h"

#include <QToolButton>

static int
sBodySizeX = 4600,
sBodySizeY = 3000,
sPinDistance = 1000,
sPinOffsetY = 0,
sPinOffsetX = 0;

static QString
sPinCount("2"),
sPinType("r1.5x0.4m0.1s0.04");

SdDMasterPartSingleLine::SdDMasterPartSingleLine(SdProjectItem *item, QWidget *parent) :
  SdDMasterPart( item, parent ),
  ui(new Ui::SdDMasterPartSingleLine)
  {
  ui->setupUi(this);

  ui->mBodySizeY->setText( sdEnvir->toPhisPcb(sBodySizeY) );
  ui->mBodySizeX->setText( sdEnvir->toPhisPcb(sBodySizeX) );

  //Pins line
  ui->mPinCount->setText( sPinCount );
  ui->mPinDistance->setText( sdEnvir->toPhisPcb(sPinDistance) );
  ui->mPinOffsetY->setText( sdEnvir->toPhisPcb(sPinOffsetY) );
  ui->mPinOffsetX->setText( sdEnvir->toPhisPcb(sPinOffsetX) );
  ui->mPinType->setText( sPinType );

  onEditChanged( QString() );

  connect( ui->mBodySizeX, &QLineEdit::textEdited, this, &SdDMasterPartSingleLine::onEditChanged );
  connect( ui->mBodySizeY, &QLineEdit::textEdited, this, &SdDMasterPartSingleLine::onEditChanged );

  connect( ui->mPinCount, &QLineEdit::textEdited, this, &SdDMasterPartSingleLine::onEditChanged );
  connect( ui->mPinDistance, &QLineEdit::textEdited, this, &SdDMasterPartSingleLine::onEditChanged );
  connect( ui->mPinOffsetY, &QLineEdit::textEdited, this, &SdDMasterPartSingleLine::onEditChanged );
  connect( ui->mPinOffsetX, &QLineEdit::textEdited, this, &SdDMasterPartSingleLine::onEditChanged );


  connect( ui->mPinTypeSelect, &QToolButton::clicked, this, [this] () {
    ui->mPinType->setText( SdDPadMaster::build( ui->mPinType->text(), this ) );
    } );


  connect( ui->buttonBox, &QDialogButtonBox::accepted, this, &SdDMasterPartSingleLine::accept );
  connect( ui->buttonBox, &QDialogButtonBox::rejected, this, &SdDMasterPartSingleLine::reject );
  }






SdDMasterPartSingleLine::~SdDMasterPartSingleLine()
  {
  delete ui;
  }





//Update preview on any params changed
void SdDMasterPartSingleLine::onEditChanged(const QString txt)
  {
  Q_UNUSED(txt)

  SdIllustrator il;

  drawPart( il );
  il.setPixmap( 250, 250, Qt::white );
  drawPart( il );
  ui->mPreview->setPixmap( il.pixmap() );
  }





//Draw part preview
void SdDMasterPartSingleLine::drawPart(SdIllustrator &il)
  {
  int bodySizeX = sdEnvir->fromPhisPcb( ui->mBodySizeX->text() );
  int bodySizeY = sdEnvir->fromPhisPcb( ui->mBodySizeY->text() );

  int pinCount    = ui->mPinCount->text().toInt();
  int pinDistance = sdEnvir->fromPhisPcb( ui->mPinDistance->text() );
  int pinOffsetY  = sdEnvir->fromPhisPcb( ui->mPinOffsetY->text() );
  int pinOffsetX  = sdEnvir->fromPhisPcb( ui->mPinOffsetX->text() );

//  int pinLen = (pinSizeX - bodySizeX) / 2;

  QColor red("red");
  QColor green("green");

  //Pin 1 reside at point 0,0

  //Left pins lenght
  int pinsLenght = (pinCount - 1) * pinDistance;

  //Part top pos
  int partTop = bodySizeY / 2 - pinOffsetY;

  //Part bot pos
  int partBot = partTop - bodySizeY;

  //Part left pos
  int partLeftPos = -(bodySizeX - pinsLenght) / 2 - pinOffsetX;

  //Part right pos
  int partRightPos = partLeftPos + bodySizeX;

  //Draw part
  il.drawRect( partLeftPos, partTop, partRightPos, partBot, red );

  //Draw pin connectors
  int crossSize = bodySizeY / 20;
  //Pins
  for( int i = 0; i < pinCount; i++ )
    il.drawCross( i * pinDistance, 0, crossSize, green );
  }




void SdDMasterPartSingleLine::accept()
  {
  sBodySizeX = sdEnvir->fromPhisPcb( ui->mBodySizeX->text() );
  sBodySizeY = sdEnvir->fromPhisPcb( ui->mBodySizeY->text() );

  sPinCount    = ui->mPinCount->text();
  int pinCount = sPinCount.toInt();
  sPinDistance = sdEnvir->fromPhisPcb( ui->mPinDistance->text() );
  sPinOffsetY  = sdEnvir->fromPhisPcb( ui->mPinOffsetY->text() );
  sPinOffsetX  = sdEnvir->fromPhisPcb( ui->mPinOffsetX->text() );

  //Pin types
  sPinType = ui->mPinType->text();

  //Pin 1 reside at point 0,0

  //Left pins lenght
  int pinsLenght = (pinCount - 1) * sPinDistance;

  //Part top pos
  int partTop = sBodySizeY / 2 - sPinOffsetY;

  //Part bot pos
  int partBot = partTop - sBodySizeY;

  //Part left pos
  int partLeftPos = -(sBodySizeX - pinsLenght) / 2 - sPinOffsetX;

  //Part right pos
  int partRightPos = partLeftPos + sBodySizeX;

  //Draw part
  addRect( partLeftPos, partTop, partRightPos, partBot );

  //Add pins

  //Add pins
  if( ui->mPlanar->isChecked() )
    setupSmdPin();
  else
    setupThrouPin();
  //Make pin number and pin name invisible
  mPinNameProp.mLayer.set( LID0_INVISIBLE );
  mPinNumberProp.mLayer.set( LID0_INVISIBLE );

  //Pins
  mPinNumberProp.mHorz = dhjLeft;
  mPinNameProp.mHorz   = dhjLeft;
  for( int i = 0; i < pinCount; i++ ) {
    SdPoint pinOrg( i * sPinDistance, 0 );
    SdPoint numberOrg( pinOrg.x(), 250 );
    SdPoint nameOrg( pinOrg.x(), -250 );
    addPin( pinOrg, sPinType, numberOrg, QString::number(i+1), nameOrg );
    }


  //Update ident position
  //Place ident at top of part
  setId( SdPoint( pinsLenght / 2, partTop+500 ) );

  mValueProp.mDir = da0;
  setValue( SdPoint( pinsLenght / 2, partBot - 500 ) );

  SdDMasterPart::accept();
  }
