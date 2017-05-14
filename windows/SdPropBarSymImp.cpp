/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdPropBarSymImp.h"
#include "objects/SdPropSymImp.h"


SdPropBarSymImp::SdPropBarSymImp(const QString title) :
  QToolBar( title )
  {
  //Symbol direction
  mDir0 = addAction( QIcon(QString(":/pic/textDirLR.png")), tr("Symbol angle 0 grad") );
  mDir0->setCheckable(true);
  connect( mDir0, &QAction::triggered, [=](bool checked){
    Q_UNUSED(checked)
    setDirection( da0 );
    emit propChanged();
    });

  mDir90 = addAction( QIcon(QString(":/pic/textDirBT.png")), tr("Symbol angle 90 grad") );
  mDir90->setCheckable(true);
  connect( mDir90, &QAction::triggered, [=](bool checked){
    Q_UNUSED(checked)
    setDirection( da90 );
    emit propChanged();
    });

  mDir180 = addAction( QIcon(QString(":/pic/textDirRL.png")), tr("Symbol angle 180 grad") );
  mDir180->setCheckable(true);
  connect( mDir180, &QAction::triggered, [=](bool checked){
    Q_UNUSED(checked)
    setDirection( da180 );
    emit propChanged();
    });

  mDir270 = addAction( QIcon(QString(":/pic/textDirTB.png")), tr("Symbol angle 270 grad") );
  mDir270->setCheckable(true);
  connect( mDir270, &QAction::triggered, [=](bool checked){
    Q_UNUSED(checked)
    setDirection( da270 );
    emit propChanged();
    });

  addSeparator();

  mMirror = addAction( QIcon(QStringLiteral(":/pic/dthMirror.png")), tr("Symbol mirroring") );
  mMirror->setCheckable(true);
  connect( mMirror, &QAction::triggered, [=](bool checked){
    Q_UNUSED(checked)
    emit propChanged();
    });

  }




void SdPropBarSymImp::setPropSymImp(SdPropSymImp *propSymImp)
  {
  if( propSymImp ) {
    //Set angle
    setDirection( propSymImp->mAngle.getValue() );

    //Set mirror
    mMirror->setChecked( propSymImp->mMirror.getValue() > 0 );
    }
  }




void SdPropBarSymImp::getPropSymImp(SdPropSymImp *propSymImp)
  {
  if( propSymImp ) {
    //Get direction
    if( mDir0->isChecked() ) propSymImp->mAngle = da0;
    else if( mDir90->isChecked() ) propSymImp->mAngle = da90;
    else if( mDir180->isChecked() ) propSymImp->mAngle = da180;
    else if( mDir270->isChecked() ) propSymImp->mAngle = da270;

    if( mMirror->isChecked() ) propSymImp->mMirror = 1;
    else propSymImp->mMirror = 0;
    }
  }




void SdPropBarSymImp::setDirection(int dir)
  {
  mDir0->setChecked( dir == da0 );
  mDir90->setChecked( dir == da90 );
  mDir180->setChecked( dir == da180 );
  mDir270->setChecked( dir == da270 );
  }
