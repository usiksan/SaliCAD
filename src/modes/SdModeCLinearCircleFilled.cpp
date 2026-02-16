/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for draw filled circle's
*/
#include "SdModeCLinearCircleFilled.h"
#include "objects/SdEnvir.h"
#include "objects/SdGraphLinearCircleFilled.h"

#include <QDebug>

SdModeCLinearCircleFilled::SdModeCLinearCircleFilled(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCLinearCircle( editor, obj )
  {

  }




void SdModeCLinearCircleFilled::drawDynamic(SdContext *ctx)
  {
  ctx->setPen( sdGlobalProp->propLine( mObject->getClass() )->mWidth.getValue(), SdEnvir::instance()->getSysColor(scEnter),
               sdGlobalProp->propLine( mObject->getClass() )->mType.getValue() );
  if( getStep() == sRadius )
    ctx->circleFill( mCenter, mRadius );
  if( SdEnvir::instance()->mIsSmart && mSmartType )
    ctx->smartPoint( mSmartPoint, mSmartType );
  }






QString SdModeCLinearCircleFilled::getModeThema() const
  {
  return QStringLiteral( MODE_HELP "ModeCLinearCircleFilled.htm" );
  }





QString SdModeCLinearCircleFilled::getStepThema() const
  {
  switch( getStep() ) {
    default :
    case sCenter : return QStringLiteral( MODE_HELP "ModeCLinearCircleFilled.htm#center" );
    case sRadius : return QStringLiteral( MODE_HELP "ModeCLinearCircleFilled.htm#radius" );
    }
  }





int SdModeCLinearCircleFilled::getIndex() const
  {
  return MD_FILL_CIRCLE;
  }




void SdModeCLinearCircleFilled::addCircle()
  {
  addPic( new SdGraphLinearCircleFilled( mCenter, mRadius, *sdGlobalProp->propLine( mObject->getClass() ) ), QObject::tr("Insert circle") );
  }
