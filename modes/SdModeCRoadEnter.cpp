#include "SdModeCRoadEnter.h"
#include "objects/SdGraphPartImp.h"
#include "objects/SdProp.h"
#include "objects/SdEnvir.h"
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
  SdStratum stratum;
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
    QString netName = mProp.mNetName.str();
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
    update();
    }
  }




void SdModeCRoadEnter::propSetToBar()
  {
  SdPropBarRoad *bar = dynamic_cast<SdPropBarRoad*>( SdWCommand::mbarTable[PB_ROAD] );
  if( bar )
    bar->setPropRoad( &mProp, mEditor->getPPM(), sdGlobalProp->mWireEnterType );
  }




void SdModeCRoadEnter::enterPoint(SdPoint)
  {
  }




void SdModeCRoadEnter::cancelPoint(SdPoint)
  {
  }

void SdModeCRoadEnter::movePoint(SdPoint)
  {
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



void SdModeCRoadEnter::rebuildBarriers()
  {

  }

