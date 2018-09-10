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

    //Browse part implement in sheet
    virtual void           cmModeBrowse(SdProjectItem *plate) override;

    //Edit properties of selected objects
    virtual void           cmEditProperties() override;

    virtual void cmModeNetList() override;

    //Perform project renumeration of component references
    virtual void           cmRenumeration() override;

    //Move component reference
    virtual void           cmModeReferenceMove() override;

    //Move component value
    virtual void           cmModeValueMove() override;

  };

#endif // SDWEDITORGRAPHSHEET_H
