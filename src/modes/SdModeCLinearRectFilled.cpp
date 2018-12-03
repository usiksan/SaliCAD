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
    ctx->setPen( 0, sdEnvir->getSysColor(scEnter), dltSolid );
    ctx->setBrush( sdEnvir->getSysColor(scEnter) );
    ctx->fillRect( SdRect(mFirst,mPrevMove) );
    }
  if( sdEnvir->mIsSmart && mSmartType )
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
  addPic( new SdGraphLinearRectFilled( mFirst, mPrevMove, sdGlobalProp->mLineProp ), QObject::tr("Insert filled rect") );
  }


