/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Visual Editor for component
*/
#ifndef SDWEDITORCOMPONENT_H
#define SDWEDITORCOMPONENT_H

#include "SdWEditor.h"
#include "objects/SdPItemComponent.h"


class SdWEditorComponent : public SdWEditor
  {
    SdPItemComponent *mComponent;
  public:
    SdWEditorComponent(SdPItemComponent *comp, QWidget *parent);

    // SdWEditor interface
  public:
    virtual SdProjectItem *getProjectItem() override;
    virtual void           onActivateEditor() override;
  };

#endif // SDWEDITORCOMPONENT_H
