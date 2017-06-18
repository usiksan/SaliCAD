/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdModeCLinearArea.h"

SdModeCLinearArea::SdModeCLinearArea(SdWEditorGraph *editor, SdProjectItem *obj)
  {

  }

void SdModeCLinearArea::drawDynamic(SdContext *ctx)
  {
  if( getStep() == sNextPoint ) {
    ctx->setPen( sdGlobalProp->mLineProp.mWidth.getValue(), sdEnvir->getSysColor(scEnter),
                 sdGlobalProp->mLineProp.mType.getValue() );
    ctx->region( mList, false );
    ctx->line( mList.last(), mMiddle );
    if( mMiddle != mPrevMove )
      ctx->line( mMiddle, mPrevMove );
    }
  }
