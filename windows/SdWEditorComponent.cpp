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
#include "SdWEditorComponent.h"


SdWEditorComponent::SdWEditorComponent(SdPItemComponent *comp, QWidget *parent) :
  SdWEditor( parent ),
  mComponent(comp)
  {

  }




SdProjectItem *SdWEditorComponent::getProjectItem()
  {
  return mComponent;
  }




void SdWEditorComponent::onActivateEditor()
  {
  }

