/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d mode used to rotate model in horizontal area
*/
#ifndef SD3DMODEHORZROTATE_H
#define SD3DMODEHORZROTATE_H

#include "Sd3dModeMat.h"

class Sd3dModeHorzRotate : public Sd3dModeMat
  {
    float  mAngleZ;      //!< Current angle around Z axis
    float  mAngleX;      //!< Current angle around X axis
    QPoint mStartPoint;  //!< Point where mouse button pressed
    bool   mLeftPressed; //!< Flag for left mouse button pressed
  public:
    Sd3dModeHorzRotate( SdPItemPart *part );

    // Sd3dMode interface
  public:
    virtual void    mousePressEvent(SdWView3d *viewer, QMouseEvent *event) override;
    virtual void    mouseReleaseEvent(SdWView3d *viewer, QMouseEvent *event) override;
    virtual bool    mouseMoveEvent(SdWView3d *viewer, QMouseEvent *event) override;
    virtual bool    draw3d(QOpenGLFunctions_2_0 *f) override;
    virtual int     modeId() const override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
  };

#endif // SD3DMODEHORZROTATE_H
