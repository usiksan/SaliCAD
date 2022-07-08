/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base editor for 3d views.

  This class is wrapper for real 3d widget, which inserted into this editor.
*/
#ifndef SDWEDITOR3D_H
#define SDWEDITOR3D_H

#include "SdWEditor.h"

class SdWView3d;
class Sd3dGraph;

class SdWEditor3d : public SdWEditor
  {
    Q_OBJECT

    SdProjectItem *mItem; //!< Item used by editor
  protected:
    SdWView3d     *mView; //!< Real 3d widget
  public:
    SdWEditor3d( SdProjectItem *item, QWidget *parent = nullptr );

    void setProjectItem( SdProjectItem *itm );

    // QWidget interface
  protected:
    virtual void           paintEvent(QPaintEvent *event) override;

    // SdWEditor interface
  public:
    virtual SdProjectItem *getProjectItem() const override;
    virtual bool           is3d() const override;
    virtual void           onActivateEditor() override;
    virtual void           cmEditUndo() override;

  };

#endif // SDWEDITOR3D_H
