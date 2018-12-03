﻿/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdModeCLinear.h"
#include "windows/SdWEditorGraph.h"
#include "windows/SdWCommand.h"
#include "windows/SdPropBarLinear.h"

SdModeCLinear::SdModeCLinear(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj )
  {
  }


int SdModeCLinear::getPropBarId() const
  {
  return PB_LINEAR;
  }



void SdModeCLinear::propGetFromBar()
  {
  SdPropBarLinear *bar = dynamic_cast<SdPropBarLinear*>( SdWCommand::mbarTable[PB_LINEAR] );
  if( bar ) {
    bar->getPropLine( &(sdGlobalProp->mLineProp), &(sdGlobalProp->mLineEnterType) );
    mEditor->setFocus();
    update();
    }
  }



void SdModeCLinear::propSetToBar()
  {
  SdPropBarLinear *bar = dynamic_cast<SdPropBarLinear*>( SdWCommand::mbarTable[PB_LINEAR] );
  if( bar )
    bar->setPropLine( &(sdGlobalProp->mLineProp), mEditor->getPPM(), sdGlobalProp->mLineEnterType );
  }
