/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graph editor for pcb layout
*/
#ifndef SDWEDITORGRAPHPLATE_H
#define SDWEDITORGRAPHPLATE_H

#include "SdWEditorGraph.h"
#include "objects/SdPItemPlate.h"


class SdWEditorGraphPlate : public SdWEditorGraph
  {
    Q_OBJECT

    SdPItemPlate *mPlate;          //Edit plate
    int           mRuleErrorIndex; //Current rule error index
  public:
    SdWEditorGraphPlate( SdPItemPlate *pcb, QWidget *parent = nullptr );

    //Update tracing status info: unconnected nets and rule errors
    void                   updateTracingInfo();

    // SdWEditor interface
  public:
    virtual SdProjectItem *getProjectItem() const override;

    virtual void           cmEditUndo() override;

    virtual void           cmModeComponent() override;
    virtual void           cmNetSetup() override;
    virtual void           cmModeDisconnect() override;
    virtual void           cmModeLink() override;
    virtual void           cmModeMovePart() override;
    virtual void           cmModePartHighlight() override;
    virtual void           cmModeEditWire() override;
    virtual void           cmModeRoadEnter() override;
    virtual void           cmModeRoadMove() override;
    virtual void           cmModePolygon() override;
    virtual void           cmModeDeleteWire() override;
    virtual void           cmModePad() override;
    virtual void           onActivateEditor() override;
    virtual void           cmPads() override;
    virtual void           cmPcbStratum() override;
    virtual void           cmModePartSelect( QStringList list ) override;
    virtual void           cmModeViaEnter() override;
    virtual void           cmModeOrigin() override;

    //Pcb rules
    virtual void           cmRulesEdit() override;
    virtual void           cmRulesCheck() override;
    virtual void           cmRulesErrorNext() override;


    //Edit properties of selected objects
    virtual void           cmEditProperties() override;

    //Sheet and pcb
    virtual void           cmRenumeration() override;

    //Move component reference
    virtual void           cmModeReferenceMove() override;

    //Move component value
    virtual void           cmModeValueMove() override;

    //Export command
    virtual void           cmFileExport() override;

    //True if traced window
    virtual bool           isTraced() const override { return true; }
  };


#endif // SDWEDITORGRAPHPLATE_H
