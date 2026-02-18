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
#include "objects/SdPvAngle.h"
#include "objects/SdPropPartImp.h"
#include "objects/SdEnvir.h"
#include "SdWCommand.h"
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>


static SdStringHistory prevAngle;

SdPropBarPartImp::SdPropBarPartImp(const QString title) :
  QToolBar(title)
  {
  insertAction( nullptr, SdWCommand::cmViewLayers );

  addWidget( new QLabel(tr("Direction:")) );
  mDirection = new QComboBox();
  mDirection->setMinimumWidth(150);
  //Enable handle enter angles
  mDirection->setEditable(true);
  //Main angles
//  mDirection->addItem( QIcon(QString(":/pic/textDirLR.png")), QString("0") );
//  mDirection->addItem( QIcon(QString(":/pic/textDirBT.png")), QString("90") );
//  mDirection->addItem( QIcon(QString(":/pic/textDirRL.png")), QString("180") );
//  mDirection->addItem( QIcon(QString(":/pic/textDirTB.png")), QString("270") );
  //Try angles without icon
  mDirection->addItem( QString("0.000") );
  mDirection->addItem( QString("90.000") );
  mDirection->addItem( QString("180.000") );
  mDirection->addItem( QString("270.000") );
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

  //Cursor align to greed or not
  mAlignToGrid = addAction( QIcon(QString(":/pic/alignGrid.png")), tr("Align cursor to grid") );
  mAlignToGrid->setCheckable(true);
  connect( mAlignToGrid, &QAction::triggered, [=](bool checked){
    SdEnvir::instance()->mCursorAlignGrid = checked;
    });

  mTop = addAction( QIcon(QStringLiteral(":/pic/flipSideTop.png")), tr("Part at top side") );
  mTop->setCheckable(true);
  connect( mTop, &QAction::triggered, [=](bool) {
    if( mBottom->isChecked() ) {
      mBottom->setChecked(false);
      mTop->setChecked(true);
      }
    emit propChanged();
    });

  mBottom = addAction( QIcon(QStringLiteral(":/pic/flipSideBottom.png")), tr("Part at bottom side") );
  mBottom->setCheckable(true);
  connect( mBottom, &QAction::triggered, [=](bool) {
    if( mTop->isChecked() ) {
      mTop->setChecked(false);
      mBottom->setChecked(true);
      }
    emit propChanged();
    });
  }




void SdPropBarPartImp::setPropPartImp(SdPropPartImp *propPartImp)
  {
  //Update grid align button
  mAlignToGrid->setChecked( SdEnvir::instance()->mCursorAlignGrid );

  if( propPartImp ) {
    //Set angle
    mDirection->setCurrentText( propPartImp->mAngle.toString() );
    reorderDirection();

    //Set bottom side
    mBottom->setChecked( propPartImp->mSide.isBottom() );
    mTop->setChecked( propPartImp->mSide.isTop() );
    }
  }




void SdPropBarPartImp::getPropPartImp(SdPropPartImp *propPartImp)
  {
  if( propPartImp ) {
    //Get direction
    QString angle = mDirection->currentText();
    if( !angle.isEmpty() )
      propPartImp->mAngle = SdPvAngle::fromString( angle );

    if( mBottom->isChecked() && !mTop->isChecked() ) propPartImp->mSide = stmBottom;
    if( !mBottom->isChecked() && mTop->isChecked() ) propPartImp->mSide = stmTop;
    }
  }





void SdPropBarPartImp::reorderDirection()
  {
  QString str = mDirection->currentText();
  if( str.isEmpty() || str == QString("0.000") || str == QString("90.000") ||
      str == QString("180.000") || str == QString("270.000") )
    return;
  int i = prevAngle.addDoubleString( str );
  if( i >= 0 )
    mDirection->removeItem( i + 4 );
  mDirection->insertItem( 4, str );
  mDirection->setCurrentText( str );
  }


