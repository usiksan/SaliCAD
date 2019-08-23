/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graph editor for symbol precentation of component
*/

#ifndef SDWEDITORGRAPHSYMBOL_H
#define SDWEDITORGRAPHSYMBOL_H

#include "SdWEditorGraph.h"
#include "objects/SdPItemSymbol.h"

class SdWEditorGraphSymbol : public SdWEditorGraph
  {
    Q_OBJECT

    SdPItemSymbol *mSymbol;
  public:
    SdWEditorGraphSymbol( SdPItemSymbol *sym, QWidget *parent = nullptr );

    // SdWEditor interface
  public:
    virtual SdProjectItem *getProjectItem() const override;
    virtual void           onActivateEditor() override;
    virtual void           cmModePin() override;
    virtual void           cmModeReference() override;
    virtual void           cmModeOrigin() override;
    virtual void           cmModeValue() override;


    //Export command
    virtual void           cmFileExport() override;

    // SdWEditorGraph interface
  public:
    virtual bool           showFields() override;
  };

#endif // SDWEDITORGRAPHSYMBOL_H
