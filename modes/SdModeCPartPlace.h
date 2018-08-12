/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part implement placement mode
*/
#ifndef SDMODECPARTPLACE_H
#define SDMODECPARTPLACE_H

#include "SdModeCommon.h"


class SdModeCPartPlace : public SdModeCommon
  {
  public:
    SdModeCPartPlace( SdWEditorGraph *editor, SdProjectItem *obj );
  };

#endif // SDMODECPARTPLACE_H
