#include "SdModeCPolygonEnter.h"
#include "objects/SdGraphTracedPolygon.h"
#include "objects/SdProp.h"
#include "objects/SdEnvir.h"
#include "objects/SdProject.h"
#include "windows/SdPropBarPolygon.h"
#include "windows/SdWCommand.h"
#include "windows/SdWEditorGraph.h"

SdModeCPolygonEnter::SdModeCPolygonEnter( SdWEditorGraph *editor, SdProjectItem *obj ) :
  SdModeCommon( editor, obj )
  {

  }

void SdModeCPolygonEnter::drawStatic(SdContext *ctx)
  {
  plate()->drawTrace( ctx, mProp.mStratum, mProp.mNetName.str() );
  }


void SdModeCPolygonEnter::drawDynamic(SdContext *ctx)
  {
  if( getStep() == sNextPoint ) {
    ctx->setPen( 0, sdEnvir->getSysColor(scEnter),
                 sdGlobalProp->mLineProp.mType.getValue() );
    ctx->region( mList, false );
    ctx->line( mList.last(), mMiddle );
    if( mMiddle != mPrevMove )
      ctx->line( mMiddle, mPrevMove );

    if( sdEnvir->mIsSmart && mList.count() > 2 )
      ctx->smartPoint( mList.at(0), snapEndPoint );
    }
  }




int SdModeCPolygonEnter::getPropBarId() const
  {
  return PB_POLYGON;
  }




void SdModeCPolygonEnter::propGetFromBar()
  {
  SdPropBarPolygon *bar = dynamic_cast<SdPropBarPolygon*>( SdWCommand::mbarTable[PB_POLYGON] );
  if( bar ) {
    //Retrive properties from polygon properties bar
    bar->getPropPolygon( &mProp, &(sdGlobalProp->mWireEnterType) );
    mEditor->setFocus();
    setDirtyCashe();
    update();
    }
  }




void SdModeCPolygonEnter::propSetToBar()
  {
  SdPropBarPolygon *bar = dynamic_cast<SdPropBarPolygon*>( SdWCommand::mbarTable[PB_POLYGON] );
  if( bar ) {
    //Accum available net names
    QStringList netList = mObject->getProject()->netList();
    //If net name not assigned yet for polygon then assign first one
    if( mProp.mNetName.str().isEmpty() && netList.count() )
      mProp.mNetName = netList.at(0);
    //Setup tracing layer count and trace type
    bar->setPlateAndTrace( plate(), layerTraceRoad );
    bar->setPropPolygon( &mProp, mEditor->getPPM(), sdGlobalProp->mWireEnterType, netList );
    }
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
