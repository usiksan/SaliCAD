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
  //Draw graphics
  mObject->forEach( dctPicture, [ctx] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph != nullptr )
      graph->draw(ctx);
    return true;
    } );

  //Draw components without pads
  mObject->forEach( dctPartImp, [ctx] (SdObject *obj) -> bool {
    SdGraphPartImp *imp = dynamic_cast<SdGraphPartImp*>(obj);
    if( imp != nullptr )
      imp->drawWithoutPads(ctx);
    return true;
    } );

  //Draw component pads down stratums
  SdStratum stratum = stmThrow;
  if( mProp.mStratum.isValid() ) stratum = stmThrow & (~mProp.mStratum.getValue());
  mObject->forEach( dctPartImp, [ctx,stratum] (SdObject *obj) -> bool {
    SdGraphPartImp *imp = dynamic_cast<SdGraphPartImp*>(obj);
    if( imp != nullptr )
      imp->drawPads( ctx, stratum, QString() );
    return true;
    } );

  //Draw roads down stratums
  mObject->forEach( dctTraceRoad|dctTraceVia, [ctx,stratum] (SdObject *obj) -> bool {
    SdGraphTraced *trace = dynamic_cast<SdGraphTraced*>(obj);
    if( trace != nullptr )
      trace->drawStratum( ctx, stratum );
    return true;
    } );

  //Draw component pads for current stratum
  if( mProp.mStratum.isValid() ) {
    //Net name for highlighting
    QString netName;
    if( getStep() )
      //If mode in tracing state then assign net name, else no highlighting
      netName = mProp.mNetName.str();
    stratum = mProp.mStratum.getValue();
    mObject->forEach( dctPartImp, [ctx,stratum,netName] (SdObject *obj) -> bool {
      SdGraphPartImp *imp = dynamic_cast<SdGraphPartImp*>(obj);
      if( imp != nullptr )
        imp->drawPads( ctx, stratum, netName );
      return true;
      } );

    mObject->forEach( dctTraceRoad|dctTraceVia, [ctx,stratum] (SdObject *obj) -> bool {
      SdGraphTraced *trace = dynamic_cast<SdGraphTraced*>(obj);
      if( trace != nullptr )
        trace->drawStratum( ctx, stratum );
      return true;
      } );
    }
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
    //Check if current point available

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

  }



void SdModeCRoadEnter::rebuildBarriers()
  {
  //plate()->accumBarriers( dctTraced, )
  }

