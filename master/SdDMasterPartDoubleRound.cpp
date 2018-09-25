/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part master for round part with exactly two pins
*/
#include "SdDMasterPartDoubleRound.h"
#include "ui_SdDMasterPartDoubleRound.h"

#include "windows/SdDPads.h"
#include "objects/SdEnvir.h"

#include <QToolButton>

SdDMasterPartDoubleRound::SdDMasterPartDoubleRound(SdProjectItem *item, bool noPin, QWidget *parent) :
  SdDMasterPart( item, parent),
  ui(new Ui::SdDMasterPartDoubleRound)
  {
  ui->setupUi(this);

  if( noPin ) {
    ui->mBodyDiameter->setText("5.0");
    ui->mBetweenPins->setText("3.0");
    }
  else {
    ui->mBodyDiameter->setText("3.0");
    ui->mBetweenPins->setText("5.0");
    }
  onEditChanged( QString() );

  connect( ui->mBodyDiameter, &QLineEdit::textEdited, this, &SdDMasterPartDoubleRound::onEditChanged );
  connect( ui->mBetweenPins, &QLineEdit::textEdited, this, &SdDMasterPartDoubleRound::onEditChanged );

  connect( ui->mLeftPinTypeSelect, &QToolButton::clicked, this, [this] () {
    QString str = SdDPads::selectPinType(this);
    if( !str.isEmpty() )
      ui->mLeftPinType->setText( str );
    } );

  connect( ui->mRightPinTypeSelect, &QToolButton::clicked, this, [this] () {
    QString str = SdDPads::selectPinType(this);
    if( !str.isEmpty() )
      ui->mRightPinType->setText( str );
    } );
  }




SdDMasterPartDoubleRound::~SdDMasterPartDoubleRound()
  {
  delete ui;
  }




//Update preview on any params changed
void SdDMasterPartDoubleRound::onEditChanged(const QString txt)
  {
  Q_UNUSED(txt)

  SdIllustrator il;

  drawPart( il );
  il.setPixmap( 250, 250, Qt::white );
  drawPart( il );
  ui->mPreview->setPixmap( il.pixmap() );
  }




void SdDMasterPartDoubleRound::changeEvent(QEvent *e)
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
void SdDMasterPartDoubleRound::drawPart(SdIllustrator &il)
  {
  int partDiameter = sdEnvir->fromPhisPcb( ui->mBodyDiameter->text() );
  int sizeX = sdEnvir->fromPhisPcb( ui->mBetweenPins->text() );
  int pinLen = (sizeX - partDiameter) / 2;

  QColor red("red");
  QColor green("green");

  //Draw part
  il.drawCircle( sizeX / 2, 0, partDiameter / 2, red );

  //Draw pins
  if( pinLen > 0 ) {
    il.drawLine( 0,0, pinLen,0, red );
    il.drawLine( sizeX - pinLen,0,  sizeX,0, red );
    }

  //Draw pin connectors
  int crossSize = partDiameter / 20;
  il.drawCross( 0,0, crossSize, green );
  il.drawCross( sizeX,0, crossSize, green );
  }



//When accept we build part with current params
void SdDMasterPartDoubleRound::accept()
  {
  //Build part
  int partDiameter = sdEnvir->fromPhisPcb( ui->mBodyDiameter->text() );
  int sizeX = sdEnvir->fromPhisPcb( ui->mBetweenPins->text() );
  int pinLen = (sizeX - partDiameter) / 2;
  //Pin types
  QString leftPinType = ui->mLeftPinType->text();
  QString rightPinType = ui->mRightPinType->text();
  if( rightPinType.isEmpty() )
    rightPinType = leftPinType;

  //Rectangle body
  addCircle( sizeX / 2, 0, partDiameter / 2 );

  //If pins outside body rectangle then draw pins
  if( pinLen > 0 ) {
    addLine( 0,0, pinLen,0 );
    addLine( sizeX-pinLen,0, sizeX,0 );
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
  addPin( SdPoint(sizeX,0), rightPinType, SdPoint(sizeX,250), QString("2"), SdPoint(sizeX,-250) );

  //Update ident position
  //When part size greater then ident text size (1000) then place ident in center of part
  // else place ident at top of part
  setId( SdPoint( sizeX/2, partDiameter > 2000 && sizeX > 4000 ? 0 : partDiameter/2+500) );

  //Update value position
  setValue( SdPoint( sizeX/2, -partDiameter/2-500) );

  //Close dialog
  SdDMasterPart::accept();
  }
