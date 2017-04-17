/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDMODECOMMON_H
#define SDMODECOMMON_H

#include "SdMode.h"

class SdModeCommon : public SdMode
  {
  protected:
    SdPoint mPrev; //Предыдущая точка (при перемещении)
  public:
    SdModeCommon( SdWEditorGraph *editor, SdProjectItem *obj );

  };

#endif // SDMODECOMMON_H
