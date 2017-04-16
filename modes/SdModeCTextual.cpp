/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdModeCTextual.h"

SdModeCTextual::SdModeCTextual(SdWEditorGraph *editor, SdProjectItem *obj) :
  SdModeCommon( editor, obj )
  {

  }


void SdModeCTextual::keyDown(int key, QChar ch)
  {
  }

int SdModeCTextual::getCursor() const
  {
  }
