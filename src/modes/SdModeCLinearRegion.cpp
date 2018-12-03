/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for closed regions
*/
#include "SdModeCLinearRegion.h"
#include "objects/SdGraphLinearRegion.h"
#include "objects/SdEnvir.h"
#include "objects/SdSnapInfo.h"
#include <QObject>

SdModeCLinearRegion::SdModeCLinearRegion(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCLinear( editor, obj )
  {

  }




//Draw dinamic part of mode scene
void SdModeCLinearRegion::drawDynamic(SdContext *ctx)
  {
  if( getStep() == sNextPoint ) {
    ctx->setPen( sdGlobalProp->mLineProp.mWidth.getValue(), sdEnvir->getSysColor(scEnter),
                 sdGlobalProp->mLineProp.mType.getValue() );
    ctx->region( mList, false );
    ctx->line( mList.last(), mMiddle );
    if( mMiddle != mPrevMove )
      ctx->line( mMiddle, mPrevMove );

    if( sdEnvir->mIsSmart && mList.count() > 2 )
      ctx->smartPoint( mList.at(0), snapEndPoint );
    }
  }




void SdModeCLinearRegion::enterPoint(SdPoint enter)
  {
  if( getStep() == sNextPoint ) {
    SdPoint first;
    first = mList.last();
    mPrevMove = calcMiddlePoint( first, enter, sdGlobalProp->mLineEnterType );
    if( mPrevMove == first ) mPrevMove = enter;
    //If point is close region then append region
    if( mPrevMove == mList.first() && mList.count() > 2 ) {
      addRegion();
      setStep( sFirstPoint );
      }
    else {
      //If enter point equals last point then remove last point
      if( first == mPrevMove ) {
        mList.removeLast();
        if( mList.count() == 0 )
          setStep( sNextPoint );
        }
      else {
        mList.append( mPrevMove );
        mPrevMove = enter;
        }
      }
    }
  else {
    mList.clear();
    mList.append( enter );
    mPrevMove = enter;
    mMiddle = enter;
    setStep( sNextPoint );
    }
  update();
  }




void SdModeCLinearRegion::cancelPoint(SdPoint)
  {
  if( getStep() ) { setStep( sFirstPoint ); update(); }
  else cancelMode();
  }




void SdModeCLinearRegion::movePoint(SdPoint p)
  {
  mPrevMove = p;
  if( getStep() == sNextPoint )
    mMiddle = calcMiddlePoint( mList.last(), mPrevMove, sdGlobalProp->mLineEnterType );
  update();
  }




SdPoint SdModeCLinearRegion::enterPrev()
  {
  if( getStep() == sNextPoint && mList.count() > 2 ) {
    addRegion();
    setStep( sFirstPoint );
    return mList.at(0);
    }
  return SdPoint();
  }



QString SdModeCLinearRegion::getStepHelp() const
  {
  return getStep() == sNextPoint ? QObject::tr("Enter next point of region") : QObject::tr("Enter first point of region");
  }




QString SdModeCLinearRegion::getModeThema() const
  {
  return QString( MODE_HELP "ModeCLinearRegion.htm" );
  }




QString SdModeCLinearRegion::getStepThema() const
  {
  return getStep() == sNextPoint ? QString( MODE_HELP "ModeCLinearRegion.htm#nextPoint" ) : QString( MODE_HELP "ModeCLinearRegion.htm#firstPoint" );
  }




int SdModeCLinearRegion::getCursor() const
  {
  return CUR_REGION;
  }




int SdModeCLinearRegion::getIndex() const
  {
  return MD_REGION;
  }




void SdModeCLinearRegion::keyDown(int key, QChar ch)
  {
  if( key == Qt::Key_Backspace ) {
    //Remove last entered point
    if( getStep() == sNextPoint && mList.count() > 1 ) {
      mList.removeLast();
      mMiddle = calcMiddlePoint( mList.last(), mPrevMove, sdGlobalProp->mLineEnterType );
      update();
      return;
      }
    }
  SdModeCLinear::keyDown( key, ch );
  }



//Append region to edit object
void SdModeCLinearRegion::addRegion()
  {
  addPic( new SdGraphLinearRegion( mList, sdGlobalProp->mLineProp ), QObject::tr("Insert region") );
  }
