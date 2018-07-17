/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
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

  //Close dialog
  SdDMasterPart::accept();
  }
