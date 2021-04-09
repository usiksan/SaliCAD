/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Move for simple view 3d scene. Does nothing more.
*/
#ifndef SD3DMODEVIEW_H
#define SD3DMODEVIEW_H

#include "Sd3dMode.h"

class Sd3dModeView : public Sd3dMode
  {
  public:
    Sd3dModeView();

    // Sd3dMode interface
  public:
    virtual bool    draw3d(QOpenGLFunctions_2_0 *f) override;
    virtual int     modeId() const override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
  };

#endif // SD3DMODEVIEW_H
