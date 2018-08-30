#include "SdModeCPolygonEnter.h"
#include "objects/SdGraphTracedPolygon.h"

SdModeCPolygonEnter::SdModeCPolygonEnter()
  {

  }


void SdModeCPolygonEnter::drawDynamic(SdContext *ctx)
  {
  }




int SdModeCPolygonEnter::getPropBarId() const
  {
  return PB_POLYGON;
  }




void SdModeCPolygonEnter::propGetFromBar()
  {
  }




void SdModeCPolygonEnter::propSetToBar()
  {
  }





void SdModeCPolygonEnter::enterPoint(SdPoint enter)
  {
  if( getStep() == sNextPoint ) {
    SdPoint first;
    first = mList.last();
    mPrevMove = calcMiddlePoint( first, enter, sdGlobalProp->mLineEnterType );
    if( mPrevMove == first ) mPrevMove = enter;
    //If point is close region then append region
    if( mPrevMove == mList.first() && mList.count() > 2 ) {
      addPic( new SdGraphTracedPolygon( mProp, mList, SdPolyWindowList() ), QObject::tr("Insert region") );
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
    setStep( sNextPoint );
    }
  update();
  }





void SdModeCPolygonEnter::cancelPoint(SdPoint)
  {
  if( getStep() ) { setStep( sFirstPoint ); update(); }
  else cancelMode();
  }




void SdModeCPolygonEnter::movePoint(SdPoint p)
  {
  mPrevMove = p;
  if( getStep() == sNextPoint )
    mMiddle = calcMiddlePoint( mList.last(), mPrevMove, sdGlobalProp->mLineEnterType );
  update();
  }





SdPoint SdModeCPolygonEnter::enterPrev()
  {
  if( getStep() == sNextPoint && mList.count() > 2 ) {
    addPic( new SdGraphTracedPolygon( mProp, mList, SdPolyWindowList() ), QObject::tr("Insert region") );
    setStep( sFirstPoint );
    return mList.at(0);
    }
  return SdPoint();
  }






QString SdModeCPolygonEnter::getStepHelp() const
  {
  return getStep() == sNextPoint ? QObject::tr("Enter next point of polygon") : QObject::tr("Enter first point of polygon");
  }




QString SdModeCPolygonEnter::getModeThema() const
  {
  return QString( MODE_HELP "ModeCPolygonEnter.htm" );
  }





QString SdModeCPolygonEnter::getStepThema() const
  {
  return getStep() == sNextPoint ? QString( MODE_HELP "ModeCPolygonEnter.htm#nextPoint" ) : QString( MODE_HELP "ModeCPolygonEnter.htm#firstPoint" );
  }




int SdModeCPolygonEnter::getCursor() const
  {
  return CUR_REGION;
  }




int SdModeCPolygonEnter::getIndex() const
  {
  return MD_POLYGON;
  }
