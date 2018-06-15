#include "SdModeCRoadEnter.h"
#include "objects/SdGraphPartImp.h"

SdModeCRoadEnter::SdModeCRoadEnter()
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
  SdStratum stratum()
  //All objects draw normally except pads connected to netName.
  mObject->forEach( dctAll, [this,ctx] (SdObject *obj) -> bool {
    SdGraphNet *net = dynamic_cast<SdGraphNet*>( obj );
    if( net != nullptr ) {
      if( !mShowNet || net->getNetName() != mNetName )
        net->draw( ctx );
      }
    else {
      SdGraph *graph = dynamic_cast<SdGraph*>( obj );
      if( graph != nullptr )
        graph->draw( ctx );
      }
    return true;
    });

  //Draw if net present
  if( !mNetName.isEmpty() && mShowNet ) {
    ctx->setOverColor( sdEnvir->getSysColor(scEnter) );
    mObject->forEach( dctNetWire | dctNetName, [this,ctx] (SdObject *obj) -> bool {
      SdGraphNet *net = dynamic_cast<SdGraphNet*>( obj );
      if( net != nullptr && net->getNetName() == mNetName )
        net->draw( ctx );
      return true;
      });
    ctx->resetOverColor();
    }
  }

void SdModeCRoadEnter::drawDynamic(SdContext *ctx)
  {
  }

int SdModeCRoadEnter::getPropBarId() const
  {
  }

void SdModeCRoadEnter::propGetFromBar()
  {
  }

void SdModeCRoadEnter::propSetToBar()
  {
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
  }

QString SdModeCRoadEnter::getModeThema() const
  {
  }

QString SdModeCRoadEnter::getStepThema() const
  {
  }

int SdModeCRoadEnter::getCursor() const
  {
  }

int SdModeCRoadEnter::getIndex() const
  {
  }
