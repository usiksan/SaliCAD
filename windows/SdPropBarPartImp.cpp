/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdPropBarPartImp.h"
#include "SdStringHistory.h"
#include "objects/SdPropAngle.h"
#include "objects/SdPropPartImp.h"
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>


static SdStringHistory prevAngle;

SdPropBarPartImp::SdPropBarPartImp(const QString title) :
  QToolBar(title)
  {
  addWidget( new QLabel(tr("Direction:")) );
  mDirection = new QComboBox();
  mDirection->setMinimumWidth(150);
  //Enable handle enter angles
  mDirection->setEditable(true);
  //Main angles
  mDirection->addItem( QIcon(QString(":/pic/textDirLR.png")), QString("0") );
  mDirection->addItem( QIcon(QString(":/pic/textDirBT.png")), QString("90") );
  mDirection->addItem( QIcon(QString(":/pic/textDirRL.png")), QString("180") );
  mDirection->addItem( QIcon(QString(":/pic/textDirTB.png")), QString("270") );
  //Append from history
  for( const QString &s : prevAngle )
    mDirection->addItem( s );

  //on complete editing
  connect( mDirection->lineEdit(), &QLineEdit::editingFinished, [=]() {
    reorderDirection();
    emit propChanged();
    });
  //on select other size
  connect( mDirection, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [=](int) {
    reorderDirection();
    emit propChanged();
    });

  addWidget( mDirection );


//  mMirror = addAction( QIcon(QStringLiteral(":/pic/dthMirror.png")), tr("Part mirroring") );
//  mMirror->setCheckable(true);
//  connect( mMirror, &QAction::triggered, [=](bool checked){
//    Q_UNUSED(checked)
//    emit propChanged();
//    });

  mBottom = addAction( QIcon(QStringLiteral(":/pic/dthMirror.png")), tr("Part at bottom side") );
  mBottom->setCheckable(true);
  connect( mBottom, &QAction::triggered, [=](bool) {
    emit propChanged();
    });
  }




void SdPropBarPartImp::setPropPartImp(SdPropPartImp *propPartImp)
  {
  if( propPartImp ) {
    //Set angle
    mDirection->setCurrentText( propPartImp->mAngle.toString() );
    reorderDirection();

    //Set bottom side
    mBottom->setChecked( propPartImp->mSide.isBottom() );
    }
  }




void SdPropBarPartImp::getPropPartImp(SdPropPartImp *propPartImp)
  {
  if( propPartImp ) {
    //Get direction
    QString angle = mDirection->currentText();
    if( !angle.isEmpty() )
      propPartImp->mAngle = SdPropAngle::fromString( angle );

    if( mBottom->isChecked() ) propPartImp->mSide = stmBottom;
    else propPartImp->mSide = stmTop;
    }
  }





void SdPropBarPartImp::reorderDirection()
  {
  QString str = mDirection->currentText();
  if( str.isEmpty() )
    return;
  int i = prevAngle.addDoubleString( str );
  if( i >= 0 )
    mDirection->removeItem( i + 4 );
  mDirection->insertItem( 4, str );
  }


