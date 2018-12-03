/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for part pin origin
*/
#ifndef SDMODECORIGINPART_H
#define SDMODECORIGINPART_H

#include "SdModeCOrigin.h"

class SdModeCOriginPart : public SdModeCOrigin
  {
  public:
    SdModeCOriginPart( SdWEditorGraph *editor, SdProjectItem *obj, int osize );
  };

#endif // SDMODECORIGINPART_H
