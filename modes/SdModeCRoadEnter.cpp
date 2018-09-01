/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for enter plate trace road
*/
#include "SdModeCRoadEnter.h"
#include "objects/SdGraphPartImp.h"
#include "objects/SdProp.h"
#include "objects/SdEnvir.h"
#include "objects/SdGraphTracedRoad.h"
#include "windows/SdPropBarRoad.h"
#include "windows/SdWCommand.h"
#include "windows/SdWEditorGraph.h"

SdModeCRoadEnter::SdModeCRoadEnter(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj )
  {

  }


void SdModeCRoadEnter::drawStatic(SdContext *ctx)
  {
  plate()->drawTrace( ctx, mProp.mStratum, getStep() ? mProp.mNetName.str() : QString() );
  }






void SdModeCRoadEnter::drawDynamic(SdContext *ctx)
  {
  //Draw active segment
  if( getStep() ) {
    SdLayer *layer = sdEnvir->mCacheForRoad.getLayer(mProp.mStratum);
    if( layer != nullptr ) ctx->setPen( mProp.mWidth, layer, dltSolid );
    else ctx->setPen( mProp.mWidth, sdEnvir->getSysColor(scEnter), dltSolid );
    ctx->line( mFirst, mMiddle );
    }
  else {
    //When start enter smart point is nearest unconnected pin
    if( sdEnvir->mIsWireSmart )
      ctx->smartPoint( mFirst, snapEndPoint );
    }
  }



int SdModeCRoadEnter::getPropBarId() const
  {
  return PB_ROAD;
  }



void SdModeCRoadEnter::propGetFromBar()
  {
  SdPropBarRoad *bar = dynamic_cast<SdPropBarRoad*>( SdWCommand::mbarTable[PB_ROAD] );
  if( bar ) {
    bar->getPropRoad( &mProp, &(sdGlobalProp->mWireEnterType) );
    mEditor->setFocus();
    setDirtyCashe();
    update();
    }
  }




void SdModeCRoadEnter::propSetToBar()
  {
  SdPropBarRoad *bar = dynamic_cast<SdPropBarRoad*>( SdWCommand::mbarTable[PB_ROAD] );
  if( bar ) {
    //Setup tracing layer count and trace type
    bar->setPlateAndTrace( plate(), layerTraceRoad );
    bar->setPropRoad( &mProp, mEditor->getPPM(), sdGlobalProp->mWireEnterType );
    }
  }




void SdModeCRoadEnter::enterPoint(SdPoint p)
  {
  if( getStep() ) {
    //Entered next road path point
    if( mFirst == p ) {
      //Circle stratum change
      SdStratum st = mStack.stratumNext( mProp.mStratum );
      if( st == mProp.mStratum ) {
        //No stratum stack at this point
        //Try insert Via
        }
      else {
        mProp.mStratum = st;
        }
      //Update rules

      }
    else {
      //Append new segment
      addPic( new SdGraphTracedRoad( mProp, mFirst, mMiddle ), QObject::tr("Insert trace road") );
      plate()->setDirtyRatNet();
      mFirst = mMiddle;
      rebuildBarriers();
      }
    }
  else {
    //Entered first road path point
    //Find whose net it contained
    //At first find on current stratum
    QString netName;
    int destStratum;
    getNetOnPoint( mFirst, mProp.mStratum, &netName, &destStratum );

    if( netName.isEmpty() )
      //No net on this point at current stratum
      //Try on all stratums
      getNetOnPoint( mFirst, stmThrow, &netName, &destStratum );

    if( !netName.isEmpty() ) {
      //mFirst = p;
      mMiddle = mFirst;
      mStack = destStratum;
      mProp.mNetName = netName;
      mProp.mStratum = mStack.stratumFirst(mStack);
      plate()->ruleBlockForNet( mProp.mStratum.getValue(), mProp.mNetName.str(), mRule );
      mProp.mWidth   = mRule.mRules[ruleRoadWidth];
      propSetToBar();
      setStep(sNextPoint);
      rebuildBarriers();
      }
    }
  setDirtyCashe();
  update();
  }




void SdModeCRoadEnter::cancelPoint(SdPoint)
  {
  if( getStep() ) {
    setDirtyCashe();
    setStep( sFirstPoint );
    }
  else cancelMode();
  }




void SdModeCRoadEnter::movePoint(SdPoint p)
  {
  if( getStep() == sNextPoint ) {
    mMiddle = calcMiddlePoint( mFirst, p, sdGlobalProp->mWireEnterType );
    //mPrevMove = p;
    //Check if current point available
    SdPoint vertex = checkRoad( mFirst, mMiddle );
    mMiddle = vertex;

    }
  else {
    mPrevMove = p;
    calcFirstSmartPoint();
    }
  update();
  }



QString SdModeCRoadEnter::getStepHelp() const
  {
  return getStep() == sNextPoint ? QObject::tr("Enter next point of road polyline") : QObject::tr("Select point to start road");
  }



QString SdModeCRoadEnter::getModeThema() const
  {
  return  QString( MODE_HELP "ModeCRoadEnter.htm" );
  }




QString SdModeCRoadEnter::getStepThema() const
  {
  return getStep() == sNextPoint ? QString( MODE_HELP "ModeCRoadEnter.htm#nextPoint" ) : QString( MODE_HELP "ModeCRoadEnter.htm#firstPoint" );
  }



int SdModeCRoadEnter::getCursor() const
  {
  return CUR_WIRE;
  }



int SdModeCRoadEnter::getIndex() const
  {
  return MD_ROAD_ENTER;
  }



void SdModeCRoadEnter::getNetOnPoint(SdPoint p, SdStratum s, QString *netName, int *destStratum)
  {
  plate()->forEach( dctTraced, [p,s,netName,destStratum] (SdObject *obj) -> bool {
    SdGraphTraced *traced = dynamic_cast<SdGraphTraced*>(obj);
    if( traced != nullptr )
      traced->isPointOnNet( p, s, netName, destStratum );
    return true;
    });
  }




void SdModeCRoadEnter::calcFirstSmartPoint()
  {
  SdSnapInfo info;
  info.mSour = mPrevMove;
  info.mSnapMask = snapNearestNet | snapNearestPin;
  info.mStratum = stmThrow;
  bool res = false;
  plate()->forEach( dctTraced, [&info,&res] (SdObject *obj) -> bool {
    SdGraphTraced *traced = dynamic_cast<SdGraphTraced*>(obj);
    if( traced != nullptr )
      res = traced->snapPoint( &info ) || res;
    return true;
    });

  if( res )
    mFirst = info.mDest;
  else
    mFirst = mPrevMove;
  }




void SdModeCRoadEnter::calcNextSmartPoint()
  {
  SdSnapInfo info;
  info.mSour = mPrevMove;
  info.mSnapMask = snapNearestNetNet | snapNearestNetPin | snapExcludeSour;
  info.mStratum = mProp.mStratum;
  info.mNetName = mProp.mNetName.str();
  bool res = false;
  plate()->forEach( dctTraced, [&info,&res] (SdObject *obj) -> bool {
    SdPtr<SdGraphTraced> traced(obj);
    if( traced.isValid() )
      res = traced->snapPoint( &info ) || res;
    return true;
    });
  }




//For all barriers: we check intersection point of line(p1,p2) and
SdPoint SdModeCRoadEnter::checkRoad(SdPoint p1, SdPoint p2) const
  {
  //If start point inside barriers then no possible trace
  if( isBarriersContains( mRoads, p1 ) )
    return p1;

  //Source line
  QLineF sf(p1.toPointF(),p2.toPointF());
  //Point for save intersection
  QPointF d;
  for( const SdBarrier &b : mRoads )
    if( b.mNetName != mProp.mNetName.str() ) {
      //For each edge of polygon we check intersection with our sf line
      for( int i = 1; i < b.mPolygon.count(); i++ ) {
        //Take next edge of polygon
        QLineF df(b.mPolygon.at(i-1),b.mPolygon.at(i));
        //If lines intersected then update sf
        if( df.intersect( sf, &d ) == QLineF::BoundedIntersection ) {
          p2 = d.toPoint();
          sf.setP2( d );
          }
        }
      //Complete with final edge
      QLineF df(b.mPolygon.first(), b.mPolygon.last() );
      //If lines intersected then update sf
      if( df.intersect( sf, &d ) == QLineF::BoundedIntersection ) {
        p2 = d.toPoint();
        sf.setP2( d );
        }
      }
  return p2;
  }




//Check if point p is inside any barrier and return true or false if not
bool SdModeCRoadEnter::isBarriersContains(const SdBarrierList &bar, SdPoint p) const
  {
  QPointF pf(p.toPointF());
  for( const SdBarrier &b : bar )
    if( b.mNetName != mProp.mNetName.str() && b.mPolygon.containsPoint(pf, Qt::OddEvenFill) )
      return true;
  return false;
  }



void SdModeCRoadEnter::rebuildBarriers()
  {
  //Accum barriers for current segment
  mRoads.clear();
  plate()->accumBarriers( dctTraced, mRoads, mProp.mStratum, ruleRoadRoad, mRule );
  }

