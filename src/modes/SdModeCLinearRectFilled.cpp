/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for filled rectangle
*/
#include "SdModeCLinearRectFilled.h"
#include "objects/SdGraphLinearRectFilled.h"
#include "objects/SdEnvir.h"

SdModeCLinearRectFilled::SdModeCLinearRectFilled(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCLinearRect( editor, obj )
  {

  }




void SdModeCLinearRectFilled::drawDynamic(SdContext *ctx)
  {
  if( getStep() == sSecondCorner ) {
    ctx->setPen( 0, SdEnvir::instance()->getSysColor(scEnter), dltSolid );
    ctx->setBrush( SdEnvir::instance()->getSysColor(scEnter) );
    ctx->fillRect( SdRect(mFirst,mPrevMove) );
    }
  if( SdEnvir::instance()->mIsSmart && mSmartType )
    ctx->smartPoint( mSmartPoint, mSmartType );
  }




QString SdModeCLinearRectFilled::getModeThema() const
  {
  return QString( MODE_HELP "ModeCLinearRectFilled.htm" );
  }




QString SdModeCLinearRectFilled::getStepThema() const
  {
  return getStep() == sSecondCorner ? QString( MODE_HELP "ModeCLinearRectFilled.htm#secondCorner" ) : QString( MODE_HELP "ModeCLinearRectFilled.htm#first" );
  }




int SdModeCLinearRectFilled::getIndex() const
  {
  return MD_FILL_RECT;
  }




//Insert rectangle into graph object
void SdModeCLinearRectFilled::addRect()
  {
  addPic( new SdGraphLinearRectFilled( mFirst, mPrevMove, *sdGlobalProp->propLine( mObject->getClass() ) ), QObject::tr("Insert filled rect") );
  }


