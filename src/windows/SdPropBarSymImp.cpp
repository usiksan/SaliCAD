/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Symbol implementation properties bar for component insertion and edit in sheet editor
*/
#include "SdPropBarSymImp.h"
#include "objects/SdPropSymImp.h"
#include "SdWCommand.h"


SdPropBarSymImp::SdPropBarSymImp(const QString title) :
  QToolBar( title )
  {
  //Layers management
  insertAction( nullptr, SdWCommand::cmViewLayers );
  addSeparator();

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




void SdPropBarSymImp::setPropSymImp(const SdPropSymImp &propSymImp)
  {
  SdPropComposerSymImp propComposerSymImp;
  propComposerSymImp.reset( propSymImp );
  setPropSymImp( propComposerSymImp );
  }




void SdPropBarSymImp::getPropSymImp(SdPropSymImp &propSymImp)
  {
  SdPropComposerSymImp propComposerSymImp;
  getPropSymImp( propComposerSymImp );
  propComposerSymImp.store( propSymImp );
  }



void SdPropBarSymImp::setPropSymImp(const SdPropComposerSymImp &propSymImp)
  {
  //Set angle
  auto &propAngle = propSymImp.get<&SdPropSymImp::mAngle>();
  setDirection( propAngle.isSingle() ? propAngle.value().as360() : -1 );

  //Set mirror
  auto &propMirror = propSymImp.get<&SdPropSymImp::mMirror>();
  mMirror->setChecked( propMirror.isSingle() && propMirror.value().asBool() );
  }




void SdPropBarSymImp::getPropSymImp(SdPropComposerSymImp &propSymImp)
  {
  propSymImp.clear();
  //Get direction
  auto &propAngle = propSymImp.get<&SdPropSymImp::mAngle>();
  if( mDir0->isChecked() ) propAngle.reset(da0);
  else if( mDir90->isChecked() ) propAngle.reset(da90);
  else if( mDir180->isChecked() ) propAngle.reset(da180);
  else if( mDir270->isChecked() ) propAngle.reset(da270);

  propSymImp.get<&SdPropSymImp::mMirror>().reset( mMirror->isChecked() ? 1 : 0 );
  }




void SdPropBarSymImp::setDirection(int dir)
  {
  mDir0->setChecked( dir == da0 );
  mDir90->setChecked( dir == da90 );
  mDir180->setChecked( dir == da180 );
  mDir270->setChecked( dir == da270 );
  }
