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
    SdWEditorGraphSheet( SdPItemSheet *sch, QWidget *parent = nullptr );

    // SdWEditor interface
  public:
    virtual SdProjectItem *getProjectItem() const override;
    virtual void           onActivateEditor() override;

    //Fragment insertion mode
    virtual void           cmModeFragment() override;

    //Component insertion mode
    virtual void           cmModeComponent() override;

    //Sheet net wire insertion mode
    virtual void           cmModeNet() override;

    //Sheet bus insertion mode
    virtual void           cmModeBus() override;

    //Sheet net wire disconnect from pin mode
    virtual void           cmModeDisconnect() override;

    //Sheet pcb area insertion mode
    virtual void           cmModePcbArea() override;

    //Sheet net name insertion mode
    virtual void           cmModeNetName() override;
    virtual void cmModeNetList() override;
  };

#endif // SDWEDITORGRAPHSHEET_H
