#include "SdDPartTwoPin.h"
#include "ui_SdDPartTwoPin.h"
#include <QLineEdit>

#include "objects/SdEnvir.h"
#include "windows/SdIllustrator.h"

SdDPartTwoPin::SdDPartTwoPin(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SdDPartSmd)
  {
  ui->setupUi(this);

  ui->mPartSizeX->setText("3.0");
  ui->mPartSizeY->setText("1.6");
  ui->mSizeX->setText("5.0");
  onEditChanged(QString());

  connect( ui->mPartSizeX, &QLineEdit::textEdited, this, &SdDPartTwoPin::onEditChanged );
  connect( ui->mPartSizeY, &QLineEdit::textEdited, this, &SdDPartTwoPin::onEditChanged );
  connect( ui->mSizeX, &QLineEdit::textEdited, this, &SdDPartTwoPin::onEditChanged );
  }

SdDPartTwoPin::~SdDPartTwoPin()
  {
  delete ui;
  }

void SdDPartTwoPin::changeEvent(QEvent *e)
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







void SdDPartTwoPin::onEditChanged(const QString str)
  {
  Q_UNUSED(str)

  SdIllustrator il;

  drawPart( il );
  il.setPixmap( 200, 200, Qt::white );
  drawPart( il );
  ui->mPreview->setPixmap( il.pixmap() );
  }




void SdDPartTwoPin::drawPart(SdIllustrator &il)
  {
  int partSizeX = sdEnvir->fromPhisPcb( ui->mPartSizeX->text() );
  int partSizeY = sdEnvir->fromPhisPcb( ui->mPartSizeY->text() );
  int sizeX = sdEnvir->fromPhisPcb( ui->mSizeX->text() );
  int pinLen = (sizeX - partSizeX) / 2;
  //if( pinLen < 0 ) pinLen = 0;

  QColor red("red");
  QColor green("green");

  //Draw part
  il.drawRect( pinLen, -partSizeY / 2, sizeX - pinLen, partSizeY / 2, red );

  //Draw pins
  il.drawLine( 0,0, pinLen,0, red );
  il.drawLine( sizeX - pinLen,0,  sizeX,0, red );

  //Draw pin connectors
  int crossSize = partSizeY / 20;
  il.drawCross( 0,0, crossSize, green );
  il.drawCross( sizeX,0, crossSize, green );

  }
