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

    SdPItemPlate *mPlate;
  public:
    SdWEditorGraphPlate( SdPItemPlate *pcb, QWidget *parent = nullptr );

    // SdWEditor interface
  public:
    virtual SdProjectItem *getProjectItem() const override;
    virtual void cmModeComponent() override;
    virtual void cmNetSetup() override;
    virtual void cmModeDisconnect() override;
    virtual void cmModeLink() override;
    virtual void cmModeMovePart() override;
    virtual void cmModePlace() override;
    virtual void cmModeEditWire() override;
    virtual void cmModeWire() override;
    virtual void cmModePolygon() override;
    virtual void cmModeDeleteWire() override;
    virtual void cmModePad() override;
    virtual void onActivateEditor() override;
  };

#endif // SDWEDITORGRAPHPLATE_H
