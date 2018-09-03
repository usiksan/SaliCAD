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

  mMaskComp = addAction( QIcon(QStringLiteral(":/pic/flipSideTop.png")), tr("Do not tought components") );
  mMaskComp->setCheckable(true);
  mMaskComp->setChecked( !sdEnvir->mEnableComp );
  connect( mMaskComp, &QAction::triggered, [=](bool) {
    sdEnvir->mEnableComp = !mMaskComp->isChecked();
    });

  mMaskNet = addAction( QIcon(QStringLiteral(":/pic/flipSideTop.png")), tr("Do not tought nets") );
  mMaskNet->setCheckable(true);
  mMaskNet->setChecked( !sdEnvir->mEnableNet );
  connect( mMaskNet, &QAction::triggered, [=](bool) {
    sdEnvir->mEnableNet = !mMaskNet->isChecked();
    });

  mMaskPicture = addAction( QIcon(QStringLiteral(":/pic/flipSideTop.png")), tr("Do not tought picture elements") );
  mMaskPicture->setCheckable(true);
  mMaskPicture->setChecked( !sdEnvir->mEnablePic );
  connect( mMaskPicture, &QAction::triggered, [=](bool) {
    sdEnvir->mEnablePic = !mMaskPicture->isChecked();
    });
  }

