#ifndef SDWEDITOR3D_H
#define SDWEDITOR3D_H

#include "SdWEditor.h"

class SdWEditor3d : public SdWEditor
  {
    Q_OBJECT

    QWidget       *mView;
    SdProjectItem *mItem;
  public:
    SdWEditor3d( SdProjectItem *item, QWidget *parent = nullptr );

    // SdWEditor interface
  public:
    virtual SdProjectItem *getProjectItem() const override;
  };

#endif // SDWEDITOR3D_H
