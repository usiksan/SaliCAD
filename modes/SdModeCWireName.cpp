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
#include "SdModeCWireName.h"
#include "objects/SdContainerSheetNet.h"
#include "objects/SdGraphWireName.h"
#include "objects/SdPItemSheet.h"
#include "objects/SdContext.h"
#include "objects/SdEnvir.h"
#include "windows/SdWCommand.h"
#include "windows/SdPropBarTextual.h"
#include "windows/SdWEditorGraph.h"
#include <QObject>

SdModeCWireName::SdModeCWireName(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj ),
  mNet(nullptr),
  mShow(nullptr)
  {

  }


void SdModeCWireName::drawStatic(SdContext *ctx)
  {
  //All objects draw normally except mShow net. It show selected
  mObject->forEach( dctAll, [this, ctx] (SdObject *obj) -> bool {
    if( obj->getParent() != mShow ) {
      SdGraph *graph = dynamic_cast<SdGraph*>( obj );
      if( graph )
        graph->draw( ctx );
      }
    return true;
    });

  //Draw mShow net if present
  if( mShow ) {
    ctx->setOverColor( sdEnvir->getSysColor(scEnter) );
    mShow->forEach( dctAll, [ctx] (SdObject *obj) -> bool {
      SdGraph *graph = dynamic_cast<SdGraph*>( obj );
      if( graph )
        graph->draw( ctx );
      return true;
      });
    ctx->resetOverColor();
    }
  }



void SdModeCWireName::drawDynamic(SdContext *ctx)
  {
  if( getStep() == sPlaceName ) {
    ctx->setOverColor( sdEnvir->getSysColor(scEnter) );
    SdRect r;
    ctx->text( mPrev, r, mName, sdGlobalProp->mWireNameProp );
    ctx->resetOverColor();
    }
  }




int SdModeCWireName::getPropBarId() const
  {
  return PB_TEXT;
  }




void SdModeCWireName::propGetFromBar()
  {
  SdPropBarTextual *tbar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
  if( tbar ) {
    tbar->getPropText( &(sdGlobalProp->mWireNameProp) );
    update();
    }
  }




void SdModeCWireName::propSetToBar()
  {
  SdPropBarTextual *tbar = dynamic_cast<SdPropBarTextual*>( SdWCommand::getModeBar(PB_TEXT) );
  if( tbar ) {
    tbar->setPropText( &(sdGlobalProp->mWireNameProp), mEditor->getPPM() );
    }
  }




void SdModeCWireName::enterPoint( SdPoint p )
  {
  if( getStep() == sPlaceName ) {
    mUndo->begin( QObject::tr("Insert sheet net name") );
    mNet->insertChild( new SdGraphWireName( mPrev, sdGlobalProp->mWireNameProp ), mUndo );
    setDirty();
    setDirtyCashe();
    update();
    }
  else {
    if( getSheet()->getNetFromPoint( p, mName ) ) {
      mNet = getSheet()->netGet( mName );
      setStep( sPlaceName );
      update();
      }
    }
  }




void SdModeCWireName::cancelPoint(SdPoint)
  {
  if( getStep() == sPlaceName ) {
    setStep( sSelectNet );
    update();
    }
  }




void SdModeCWireName::movePoint( SdPoint p )
  {
  if( getStep() == sPlaceName ) {
    mPrev = p;
    update();
    }
  }




QString SdModeCWireName::getStepHelp() const
  {
  return getStep() == sPlaceName ? QObject::tr("Enter net name place point") : QObject::tr("Select net for name placement");
  }




QString SdModeCWireName::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeCWireName.htm" );
  }




QString SdModeCWireName::getStepThema() const
  {
  return getStep() == sPlaceName ? QStringLiteral( MODE_HELP "ModeCWireName.htm#placeName" ) : QStringLiteral( MODE_HELP "ModeCWireName.htm#selectNet" );
  }




int SdModeCWireName::getCursor() const
  {
  return CUR_ARROW;
  }



int SdModeCWireName::getIndex() const
  {
  return MD_NET_NAME;
  }




SdPItemSheet *SdModeCWireName::getSheet()
  {
  return dynamic_cast<SdPItemSheet*>(mObject);
  }
