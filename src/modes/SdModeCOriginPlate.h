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
#ifndef SDMODECORIGINPLATE_H
#define SDMODECORIGINPLATE_H

#include "SdModeCOrigin.h"


class SdModeCOriginPlate : public SdModeCOrigin
  {
  public:
    SdModeCOriginPlate( SdWEditorGraph *editor, SdProjectItem *obj, int osize );
  };

#endif // SDMODECORIGINPLATE_H
