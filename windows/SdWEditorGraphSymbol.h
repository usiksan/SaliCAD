/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
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
    SdWEditorGraphSymbol( SdPItemSymbol *sym, QWidget *parent = 0 );

    // SdWEditor interface
  public:
    virtual SdProjectItem *getProjectItem() override;
    virtual void           onActivateEditor() override;

    // SdWEditorGraph interface
  public:
    virtual double getPPM() const override;
  };

#endif // SDWEDITORGRAPHSYMBOL_H
