/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for enter plate trace polygon
*/
#include "SdModeCPolygonEnter.h"
#include "objects/SdGraphTracedPolygon.h"
#include "objects/SdProp.h"
#include "objects/SdEnvir.h"
#include "objects/SdProject.h"
#include "windows/SdPropBarPolygon.h"
#include "windows/SdWCommand.h"
#include "windows/SdWEditorGraph.h"

#include <QDebug>

SdModeCPolygonEnter::SdModeCPolygonEnter( SdWEditorGraph *editor, SdProjectItem *obj ) :
  SdModeCommon( editor, obj ),
  mSmart( SdPoint::farPoint() )
  {

  }




void SdModeCPolygonEnter::drawStatic(SdContext *ctx)
  {
  plate()->drawTrace( ctx, mProp.mStratum, mProp.mNetName.string() );
  }





void SdModeCPolygonEnter::drawDynamic(SdContext *ctx)
  {
  if( getStep() == sNextPoint ) {
    ctx->setPen( 0, SdEnvir::instance()->getSysColor(scEnter),
                 sdGlobalProp->propLine( mObject->getClass() )->mType );
    ctx->region( mList, false );
    ctx->line( mList.last(), mMiddle );
    if( mMiddle != mPrevMove )
      ctx->line( mMiddle, mPrevMove );

    if( SdEnvir::instance()->mIsSmart && mList.count() > 2 )
      ctx->smartPoint( mList.at(0), snapEndPoint );
    }
  else {
    //Draw snap point with which we retrieve net name for polygon
    if( !mSmart.isFar() && SdEnvir::instance()->mIsSmart )
      ctx->smartPoint( mSmart, snapEndPoint );
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
    bar->getPropPolygon( mProp, &(sdGlobalProp->mWireEnterType) );
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
    if( mProp.mNetName.string().isEmpty() && netList.count() )
      mProp.mNetName = netList.at(0);
    //Setup tracing layer count and trace type
    bar->setPlateAndTrace( plate(), layerTraceRoad );
    bar->setPropPolygon( mProp, mEditor->getPPM(), sdGlobalProp->mWireEnterType, netList );
    }
  }





void SdModeCPolygonEnter::enterPoint(SdPoint enter)
  {
  if( getStep() == sNextPoint ) {
    SdPoint first;
    first = mList.last();
    mPrevMove = calcMiddlePoint( first, enter, sdGlobalProp->mWireEnterType );
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
    mMiddle = calcMiddlePoint( mList.last(), mPrevMove, sdGlobalProp->mWireEnterType );
  else {
    //For first point we snap point to nearest net to retrive net name for polygon
    SdSnapInfo info;
    info.mSour = mPrevMove;
    info.mSnapMask = snapNearestNet | snapNearestPin;
    info.mStratum = stmThrough;
    info.scan( plate(), dctTraced );
    if( info.isFound() )
      mSmart = info.mDest;
    else
      mSmart = SdPoint::farPoint();
    }
  update();
  }





SdPoint SdModeCPolygonEnter::enterPrev()
  {
  if( getStep() == sNextPoint && mList.count() > 2 ) {
    addPic( new SdGraphTracedPolygon( mProp, mList, SdPolyWindowList() ), QObject::tr("Insert region") );
    setStep( sFirstPoint );
    return mList.at(0);
    }
  else {
    //Retrieve net name for polygon from current smart point
    if( !mSmart.isFar() ) {
      QString netName;
      SdPvStratum stratum = mProp.mStratum;
      SdPoint p(mSmart);
      plate()->forEach( dctTraced, [&netName,&stratum,p] ( SdObject *obj ) -> bool {
        SdPtr<SdGraphTraced> traced(obj);
        if( traced.isValid() )
          return !traced->isPointOnNet( p, stratum, netName, stratum );
        return true;
        });
      //qDebug() << "smart enter" << netName;
      mProp.mNetName = netName;
      mProp.mStratum = stratum;
      propSetToBar();
      }
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




void SdModeCPolygonEnter::keyDown(int key, QChar ch)
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
  SdModeCommon::keyDown( key, ch );
  }
