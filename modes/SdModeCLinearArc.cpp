/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for draw arc's
*/
#include "SdModeCLinearArc.h"
#include "objects/SdEnvir.h"
#include "objects/SdGraphLinearArc.h"
#include <QDebug>

SdPoint
SdModeCLinearArc::mStartOffset,
SdModeCLinearArc::mStopOffset;


SdModeCLinearArc::SdModeCLinearArc(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCLinear( editor, obj )
  {

  }


void SdModeCLinearArc::enterPoint( SdPoint enter )
  {
  switch( getStep() ) {
    case sCenter :
      mCenter = enter;
      mPrev = mCenter;
      setStep( sStart );
      mSmartPoint = mCenter + mStartOffset;
      mSmartType  = snapPrev;
      break;
    case sStart :
      mStart = enter;
      mPrev = enter;
      setStep( sStop );
      mSmartPoint = mCenter + mStopOffset;
      mSmartType  = snapPrev;
      break;
    case sStop :
      mPrev = calcArcStop( mCenter, mStart, enter );
      addPic(  new SdGraphLinearArc( mCenter, mStart, mPrev, sdGlobalProp->mLineProp ), QObject::tr("Insert arc") );
      setStep( sCenter );
      mStartOffset = mStart - mCenter;
      mStopOffset = mPrev - mCenter;
      mSmartType  = 0;
      break;
    }
  update();
  }




void SdModeCLinearArc::cancelPoint(SdPoint)
  {
  if( getStep() != sCenter ) setStep( sCenter );
  else cancelMode();
  }




void SdModeCLinearArc::movePoint(SdPoint p)
  {
  mPrev = p;

  //Вычислить предполагаемую точку вывода
  SdSnapInfo snap;
  snap.mSour     = mPrev;
  snap.mSnapMask = sdEnvir->mSmartMask;
  snap.mExclude  = mCenter;
  snap.mFlag     = dsifExExcl;
  snap.calculate( mObject );
  mSmartType  = snap.mDestMask;
  mSmartPoint = snap.mDest;

  update();
  }




SdPoint SdModeCLinearArc::enterPrev()
  {
  if( sdEnvir->mIsSmart && mSmartType )
    enterPoint( mSmartPoint );
  return mSmartPoint;
  }




QString SdModeCLinearArc::getStepHelp() const
  {
  switch( getStep() ) {
    default :
    case sCenter : return QObject::tr("Enter center arc point");
    case sStart  : return QObject::tr("Enter start arc point");
    case sStop   : return QObject::tr("Enter stop arc point");
    }
  }




QString SdModeCLinearArc::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeCLinearArc.htm" );
  }




QString SdModeCLinearArc::getStepThema() const
  {
  switch( getStep() ) {
    default :
    case sCenter : return QStringLiteral( MODE_HELP "ModeCLinearArc.htm#center" );
    case sStart  : return QStringLiteral( MODE_HELP "ModeCLinearArc.htm#start" );
    case sStop   : return QStringLiteral( MODE_HELP "ModeCLinearArc.htm#stop" );
    }
  }




int SdModeCLinearArc::getCursor() const
  {
  return CUR_ARC;
  }




int SdModeCLinearArc::getIndex() const
  {
  return MD_ARC;
  }




void SdModeCLinearArc::drawDynamic(SdContext *ctx)
  {
  ctx->setPen( sdGlobalProp->mLineProp.mWidth.getValue(), sdEnvir->getSysColor(scEnter),
               sdGlobalProp->mLineProp.mType.getValue() );
  switch( getStep() ) {
    case sStop :
      //Выбор воторой точки
      //First point selection
      ctx->arc( mCenter, mStart, mPrev );
      //break is absent purposefully
    case sStart :
      //
      //В том числе выбор первой точки
      ctx->line( mCenter, mPrev );
    }
  if( sdEnvir->mIsSmart && mSmartType )
    ctx->smartPoint( mSmartPoint, mSmartType );
  }
