/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graph editor for schematic sheet
*/
#ifndef SDWEDITORGRAPHSHEET_H
#define SDWEDITORGRAPHSHEET_H

#include "SdWEditorGraph.h"
#include "objects/SdPItemSheet.h"



class SdWEditorGraphSheet : public SdWEditorGraph
  {
    Q_OBJECT

    SdPItemSheet *mSheet;
  public:
    SdWEditorGraphSheet( SdPItemSheet *sch, QWidget *parent = 0 );

    // SdWEditor interface
  public:
    virtual SdProjectItem *getProjectItem() override;
    virtual void           onActivateEditor() override;
    virtual void           cmModeComponent() override;
    virtual void           cmModeNet() override;
    virtual void cmModeBus() override;
    virtual void cmModeDisconnect() override;
    virtual void cmModePcbArea() override;
    virtual void cmModeNetName() override;
    virtual void cmModeNetList() override;

    // SdWEditorGraph interface
  public:
    virtual double getPPM() const override;
  };

#endif // SDWEDITORGRAPHSHEET_H
