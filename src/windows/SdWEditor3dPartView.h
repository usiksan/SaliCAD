#ifndef SDWEDITOR3DPARTVIEW_H
#define SDWEDITOR3DPARTVIEW_H

#include "SdWEditor3d.h"
#include "objects/SdPItemPart.h"

class SdWEditor3dPartView : public SdWEditor3d
  {
    Q_OBJECT
    SdPItemPart *mPartPtr; //!< Part viewed with this editor
  public:
    SdWEditor3dPartView( SdPItemPart *item, QWidget *parent = nullptr );

    // SdWEditor interface
  public:
    virtual void onActivateEditor() override;
    virtual void cm3dProgram() override;
  };

#endif // SDWEDITOR3DPARTVIEW_H
