/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for draw circle's
*/
#include "SdModeCLinearCircle.h"
#include "objects/SdEnvir.h"
#include "objects/SdGraphLinearCircle.h"
#include <QDebug>

int SdModeCLinearCircle::mPreviousRadius;

SdModeCLinearCircle::SdModeCLinearCircle(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCLinear( editor, obj )
  {

  }





void SdModeCLinearCircle::drawDynamic(SdContext *ctx)
  {
  ctx->setPen( sdGlobalProp->mLineProp.mWidth.getValue(), sdEnvir->getSysColor(scEnter),
               sdGlobalProp->mLineProp.mType.getValue() );
  if( getStep() == sRadius )
    ctx->circle( mCenter, mRadius );
  if( sdEnvir->mIsSmart && mSmartType )
    ctx->smartPoint( mSmartPoint, mSmartType );
  }




void SdModeCLinearCircle::enterPoint(SdPoint p)
  {
  if( getStep() == sRadius ) {
    mPreviousRadius = mRadius = mCenter.getDistanceInt( p );
    addCircle();
    setStep( sCenter );
    mSmartType = 0;
    }
  else {
    mCenter = p;
    mRadius = 0;
    setStep( sRadius );
    mSmartPoint = mCenter;
    mSmartPoint.rx() += mPreviousRadius;
    mSmartType = snapPrev;
    }
  }



void SdModeCLinearCircle::cancelPoint(SdPoint)
  {
  if( getStep() ) setStep( sCenter );
  else cancelMode();
  }




void SdModeCLinearCircle::movePoint(SdPoint p)
  {
  if( getStep() ) mRadius = mCenter.getDistanceInt( p );
  else mCenter = p;
  //Вычислить предполагаемую точку вывода
  SdSnapInfo snap;
  snap.mSour     = p;
  snap.mSnapMask = sdEnvir->mSmartMask;
  snap.mExclude  = mCenter;
  snap.scan( mObject );
  mSmartType  = snap.mDestMask;
  mSmartPoint = snap.mDest;

  update();
  }




QString SdModeCLinearCircle::getStepHelp() const
  {
  switch( getStep() ) {
    default :
    case sCenter : return QObject::tr("Enter center circle point");
    case sRadius : return QObject::tr("Enter point on circle to define circle radius");
    }
  }




QString SdModeCLinearCircle::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeCLinearCircle.htm" );
  }




QString SdModeCLinearCircle::getStepThema() const
  {
  switch( getStep() ) {
    default :
    case sCenter : return QStringLiteral( MODE_HELP "ModeCLinearCircle.htm#center" );
    case sRadius : return QStringLiteral( MODE_HELP "ModeCLinearCircle.htm#radius" );
    }
  }




int SdModeCLinearCircle::getCursor() const
  {
  return CUR_CIRCLE;
  }



int SdModeCLinearCircle::getIndex() const
  {
  return MD_CIRCLE;
  }




void SdModeCLinearCircle::addCircle()
  {
  addPic( new SdGraphLinearCircle( mCenter, mRadius, sdGlobalProp->mLineProp ), QObject::tr("Insert circle") );
  }




SdPoint SdModeCLinearCircle::enterPrev()
  {
  if( sdEnvir->mIsSmart && mSmartType )
    enterPoint( mSmartPoint );
  return mSmartPoint;
  }

