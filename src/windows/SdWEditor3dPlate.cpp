#include "SdWEditor3dPlate.h"
#include "objects/SdPItemPlate.h"

SdWEditor3dPlate::SdWEditor3dPlate( SdPItemPlate *item, QWidget *parent ) :
  SdWEditor3d( item, parent ),
  mPlate(item)
  {

  }

void SdWEditor3dPlate::onActivateEditor()
  {
  SdWEditor3d::onActivateEditor();

  mPlate->rebuild3dModel();
  }
