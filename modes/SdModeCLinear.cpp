/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdModeCLinear.h"

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
  }

void SdModeCLinear::propSetToBar()
  {
  }
