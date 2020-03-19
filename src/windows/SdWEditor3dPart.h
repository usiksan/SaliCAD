/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Editor for 3d part.

  This class is wrapper for real 3d widget, which inserted into this editor.
*/
#ifndef SDWEDITOR3DPART_H
#define SDWEDITOR3DPART_H

#include "SdWEditor3d.h"

class SdWEditor3dPart : public SdWEditor3d
  {
    Q_OBJECT
  public:
    SdWEditor3dPart( SdProjectItem *item, QWidget *parent = nullptr );

    // SdWEditor interface
  public:
//    virtual SdProjectItem *getProjectItem() const override;
    virtual void           onActivateEditor() override;
  };

#endif // SDWEDITOR3DPART_H
