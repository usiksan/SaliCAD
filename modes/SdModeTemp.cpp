/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base mode for temporary modes.
*/
#include "SdModeTemp.h"

SdModeTemp::SdModeTemp( SdWEditorGraph *editor, SdProjectItem *obj ) :
  SdMode( editor, obj ),
  mMainMode(0)
  {

  }


void SdModeTemp::draw(SdContext *context)
  {
  //Draw full picture
  mObject->draw( context );
  }
