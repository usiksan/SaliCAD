/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDWEDITORGRAPHPART_H
#define SDWEDITORGRAPHPART_H

#include "SdWEditorGraph.h"
#include "objects/SdPItemPart.h"


class SdWEditorGraphPart : public SdWEditorGraph
  {
    Q_OBJECT

  public:
    SdWEditorGraphPart(SdPItemPart *part, QWidget *parent);
  };

#endif // SDWEDITORGRAPHPART_H
