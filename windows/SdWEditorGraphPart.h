/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graph editor for part of component
*/
#ifndef SDWEDITORGRAPHPART_H
#define SDWEDITORGRAPHPART_H

#include "SdWEditorGraph.h"
#include "objects/SdPItemPart.h"


class SdWEditorGraphPart : public SdWEditorGraph
  {
    Q_OBJECT

    SdPItemPart *mPart; //Editing part
  public:
    SdWEditorGraphPart(SdPItemPart *part, QWidget *parent);

    void setPart( SdPItemPart *part );

    // SdWEditor interface
  public:
    virtual SdProjectItem *getProjectItem() override;
    virtual void           onActivateEditor() override;
    virtual void           cmModePin() override;
    virtual void           cmModeReference() override;
    virtual void           cmModeOrigin() override;

    // SdWEditorGraph interface
  public:
    virtual double getPPM() const override;
  };

#endif // SDWEDITORGRAPHPART_H
