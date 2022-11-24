#include "SdDMasterPartModule.h"
#include "ui_SdDMasterPartModule.h"

SdDMasterPartModule::SdDMasterPartModule(SdProjectItem *item, QWidget *parent) :
  SdDMasterPart( item, parent ),
  ui(new Ui::SdDMasterPartModule)
  {
  ui->setupUi(this);
  }



SdDMasterPartModule::~SdDMasterPartModule()
  {
  delete ui;
  }



void SdDMasterPartModule::onEditChanged(const QString txt)
  {
  Q_UNUSED(txt)

  SdIllustrator il;

  drawPart( il );
  il.setPixmap( 400, 400, Qt::white );
  drawPart( il );
//  ui->mPreview->setPixmap( il.pixmap() );
  }




void SdDMasterPartModule::changeEvent(QEvent *e)
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

void SdDMasterPartModule::drawPart(SdIllustrator &il)
  {

  }

void SdDMasterPartModule::accept()
  {

  }
