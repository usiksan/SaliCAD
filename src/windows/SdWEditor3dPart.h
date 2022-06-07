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

#include "objects/SdPItemPart.h"
#include "SdWEditor3d.h"

class SdWEditor3dPart : public SdWEditor3d
  {
    Q_OBJECT

    SdPItemPart *mPartPtr; //!< Part viewed with this editor
  public:
    SdWEditor3dPart( SdPItemPart *item, QWidget *parent = nullptr );

    // SdWEditor interface
  public:
//    virtual SdProjectItem *getProjectItem() const override;
    virtual void onActivateEditor() override;
    virtual void cm3dMaster() override;
    virtual void cm3dProgram() override;

    // SdWEditor interface
  public:
    virtual void cm3dModeView() override;
    virtual void cm3dModeHorzMove() override;
    virtual void cm3dModeHorzRotate() override;
    virtual void cm3dModeVertMove() override;
    virtual void cm3dModeVertRotate() override;
    virtual void cm3dModeFaceColor() override;
    virtual void cm3dImportStl() override;
    virtual void cm3dImportStep() override;
    virtual void cm3dImportVrml() override;

  private:
    //!
    //! \brief importModel Does import 3d model into part item. Previous 3d model removed.
    //! \param model       Model to insert into part item
    //! \param undoTitle   Undo title text
    //!
    void         importModel( Sd3dGraph *model, const QString undoTitle );
  };

#endif // SDWEDITOR3DPART_H
