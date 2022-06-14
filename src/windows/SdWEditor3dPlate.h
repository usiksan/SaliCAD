#ifndef SDWEDITOR3DPLATE_H
#define SDWEDITOR3DPLATE_H

#include "SdWEditor3d.h"

class SdPItemPlate;

class SdWEditor3dPlate : public SdWEditor3d
  {
    Q_OBJECT

    SdPItemPlate *mPlate;
  public:
    SdWEditor3dPlate( SdPItemPlate *item, QWidget *parent = nullptr );

    // SdWEditor interface
  public:
    virtual void onActivateEditor() override;
  };

#endif // SDWEDITOR3DPLATE_H
