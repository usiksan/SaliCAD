/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for plate origin
*/
#include "SdModeCOriginPlate.h"

SdModeCOriginPlate::SdModeCOriginPlate(SdWEditorGraph *editor, SdProjectItem *obj, int osize) :
  SdModeCOrigin( editor, obj, osize )
  {
  mModeIndex = MD_PLATE_ORIGIN;
  }
