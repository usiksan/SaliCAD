/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphics editor, common part
*/

#ifndef SDWEDITORGRAPH_H
#define SDWEDITORGRAPH_H

#include "SdWEditor.h"


class SdWEditorGraph : public SdWEditor
  {
    Q_OBJECT

  public:
    SdWEditorGraph( QWidget *parent = 0 );
  };

#endif // SDWEDITORGRAPH_H
