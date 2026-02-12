/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Default tool bar for selection
*/
#include "SdPropBarDefault.h"
#include "objects/SdEnvir.h"

SdPropBarDefault::SdPropBarDefault(const QString title) :
  SdPropBar( title )
  {

  mMaskComp = addAction( QIcon(QStringLiteral(":/pic/iconDisableComp.png")), tr("Do not tought components") );
  mMaskComp->setCheckable(true);
  mMaskComp->setChecked( !SdEnvir::instance()->mEnableComp );
  connect( mMaskComp, &QAction::triggered, [=](bool) {
    SdEnvir::instance()->mEnableComp = !mMaskComp->isChecked();
    });

  mMaskNet = addAction( QIcon(QStringLiteral(":/pic/iconDisableWire.png")), tr("Do not tought nets") );
  mMaskNet->setCheckable(true);
  mMaskNet->setChecked( !SdEnvir::instance()->mEnableNet );
  connect( mMaskNet, &QAction::triggered, [=](bool) {
    SdEnvir::instance()->mEnableNet = !mMaskNet->isChecked();
    });

  mMaskPicture = addAction( QIcon(QStringLiteral(":/pic/iconDisablePic.png")), tr("Do not tought picture elements") );
  mMaskPicture->setCheckable(true);
  mMaskPicture->setChecked( !SdEnvir::instance()->mEnablePic );
  connect( mMaskPicture, &QAction::triggered, [=](bool) {
    SdEnvir::instance()->mEnablePic = !mMaskPicture->isChecked();
    });
  }

