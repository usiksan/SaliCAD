/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for placement net names
*/
#include "SdModeCNetName.h"
#include "objects/SdGraphNetName.h"
#include "objects/SdPItemSheet.h"
#include "objects/SdContext.h"
#include "objects/SdEnvir.h"
#include "windows/SdWCommand.h"
#include "windows/SdPropBarTextual.h"
#include "windows/SdWEditorGraph.h"
#include <QObject>

SdModeCNetName::SdModeCNetName(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj )
  {

  }


void SdModeCNetName::drawStatic(SdContext *ctx)
  {
  //All objects draw normally except net with netName.
  mObject->forEach( dctAll, [this,ctx] (SdObject *obj) -> bool {
    SdGraphNet *net = dynamic_cast<SdGraphNet*>( obj );
    if( net != nullptr ) {
      if( net->getNetName() != mNetName )
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
  if( !mNetName.isEmpty() ) {
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



void SdModeCNetName::drawDynamic(SdContext *ctx)
  {
  if( getStep() == sPlaceName ) {
    ctx->setOverColor( sdEnvir->getSysColor(scEnter) );
    SdRect r;
    ctx->text( mPrev, r, mNetName, sdGlobalProp->mWireNameProp );
    ctx->resetOverColor();
    }
  }




int SdModeCNetName::getPropBarId() const
  {
  return PB_TEXT;
  }




void SdModeCNetName::propGetFromBar()
  {
  SdPropBarTextual *tbar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
  if( tbar ) {
    tbar->getPropText( &(sdGlobalProp->mWireNameProp) );
    update();
    }
  }




void SdModeCNetName::propSetToBar()
  {
  SdPropBarTextual *tbar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
  if( tbar ) {
    tbar->setPropText( &(sdGlobalProp->mWireNameProp), mEditor->getPPM() );
    }
  }




void SdModeCNetName::enterPoint( SdPoint p )
  {
  if( getStep() == sPlaceName ) {
    mUndo->begin( QObject::tr("Insert sheet net name"), mObject );
    mObject->insertChild( new SdGraphNetName( mPrev, mNetName, sdGlobalProp->mWireNameProp ), mUndo );
    setDirty();
    setDirtyCashe();
    update();
    }
  else {
    if( getSheet()->getNetFromPoint( p, mNetName ) ) {
      mPrev = p;
      setDirty();
      setDirtyCashe();
      setStep( sPlaceName );
      update();
      }
    }
  }




void SdModeCNetName::cancelPoint(SdPoint)
  {
  if( getStep() == sPlaceName ) {
    mNetName.clear();
    setDirty();
    setDirtyCashe();
    setStep( sSelectNet );
    update();
    }
  else cancelMode();
  }




void SdModeCNetName::movePoint( SdPoint p )
  {
  if( getStep() == sPlaceName ) {
    mPrev = p;
    update();
    }
  }




QString SdModeCNetName::getStepHelp() const
  {
  return getStep() == sPlaceName ? QObject::tr("Enter net name place point") : QObject::tr("Select net for name placement");
  }




QString SdModeCNetName::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeCNetName.htm" );
  }




QString SdModeCNetName::getStepThema() const
  {
  return getStep() == sPlaceName ? QStringLiteral( MODE_HELP "ModeCNetName.htm#placeName" ) : QStringLiteral( MODE_HELP "ModeCNetName.htm#selectNet" );
  }




int SdModeCNetName::getCursor() const
  {
  return CUR_ARROW;
  }



int SdModeCNetName::getIndex() const
  {
  return MD_NET_NAME;
  }




SdPItemSheet *SdModeCNetName::getSheet()
  {
  return dynamic_cast<SdPItemSheet*>(mObject);
  }
