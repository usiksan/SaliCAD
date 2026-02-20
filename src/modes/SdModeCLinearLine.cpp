/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for line
*/
#include "SdModeCLinearLine.h"
#include "objects/SdGraphLinearLine.h"
#include "objects/SdEnvir.h"
#include "objects/SdSnapInfo.h"
#include <QObject>

SdPoint SdModeCLinearLine::mOffset; //Предположительная очередная точка

SdModeCLinearLine::SdModeCLinearLine(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCLinear( editor, obj )
  {

  }




void SdModeCLinearLine::drawDynamic(SdContext *ctx)
  {
  if( getStep() == sNextPoint ) {
    ctx->setPen( sdGlobalProp->propLine( mObject->getClass() )->mWidth, SdEnvir::instance()->getSysColor(scEnter),
                 sdGlobalProp->propLine( mObject->getClass() )->mType );
    ctx->line( mFirst, mMiddle );
    if( mMiddle != mPrevMove )
      ctx->line( mMiddle, mPrevMove );

//    ctx->setFont( sdGlobalProp->mTextProp );
//    SdRect over;
//    ctx->text( mPrevMove, over, QString("Primer"), 90000, 1, 1 );
    }
  if( SdEnvir::instance()->mIsSmart && mSmartType )
    ctx->smartPoint( mSmartPoint, mSmartType );
  }




void SdModeCLinearLine::enterPoint(SdPoint enter)
  {
  if( getStep() ) {
    mPrevMove = calcMiddlePoint( mFirst, enter, sdGlobalProp->mLineEnterType );
    if( mPrevMove == mFirst ) mPrevMove = enter;
    //Добавить сегмент
    if( mPrevMove != mFirst )
      addPic( new SdGraphLinearLine( mFirst, mPrevMove, *sdGlobalProp->propLine( mObject->getClass() ) ), QObject::tr("Insert line") );
    mOffset = mPrevMove - mFirst;
    mFirst = mPrevMove;
    mPrevMove = enter;
    }
  else {
    mFirst = enter;
    mPrevMove = mFirst;
    mMiddle = mFirst;
    setStep( sNextPoint );

    //Установить разумную точку как относительное смещение
    mSmartPoint = mPrevMove + mOffset;
    mSmartType  = snapPrev;
    }
  update();
  }




void SdModeCLinearLine::cancelPoint(SdPoint)
  {
  if( getStep() ) { setStep( sFirstPoint ); update(); }
  else cancelMode();
  }




void SdModeCLinearLine::movePoint( SdPoint p )
  {
  if( p == mPrevMove )
    return;

  mPrevMove = p;
  if( getStep() ) mMiddle = calcMiddlePoint( mFirst, mPrevMove, sdGlobalProp->mLineEnterType );
  else mFirst = p;

  //Вычислить предполагаемую точку вывода
  SdSnapInfo snap;
  snap.mSour     = mPrevMove;
  snap.mSnapMask = SdEnvir::instance()->mSmartMask | snapExcludeExcl;
  snap.mExclude  = mFirst;
  snap.scan( mObject );
  mSmartType  = snap.mDestMask;
  mSmartPoint = snap.mDest;

  update();
  }




SdPoint SdModeCLinearLine::enterPrev()
  {
  if( SdEnvir::instance()->mIsSmart && mSmartType ) {
    if( getStep() ) {
      enterPoint( mSmartPoint );
      cancelPoint( mSmartPoint );
      }
    else
      enterPoint( mSmartPoint );
    }
  return mSmartPoint;
  }




QString SdModeCLinearLine::getStepHelp() const
  {
  return getStep() == sNextPoint ? QObject::tr("Enter next point of polyline") : QObject::tr("Enter first line point");
  }




QString SdModeCLinearLine::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeCLinearLine.htm" );
  }




QString SdModeCLinearLine::getStepThema() const
  {
  return getStep() == sNextPoint ? QString( MODE_HELP "ModeCLinearLine.htm#nextPoint" ) : QString( MODE_HELP "ModeCLinearLine.htm#firstPoint" );
  }




int SdModeCLinearLine::getCursor() const
  {
  return CUR_LINE;
  }




int SdModeCLinearLine::getIndex() const
  {
  return MD_LINE;
  }
