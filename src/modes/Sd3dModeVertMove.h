/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d mode used to translate model in vertical area
*/
#ifndef SD3DMODEVERTMOVE_H
#define SD3DMODEVERTMOVE_H

#include "Sd3dModeMat.h"

class Sd3dModeVertMove : public Sd3dModeMat
  {
    float  mOffsetZ;     //!< Current offset for transfer model
    float  mOffsetX;
    float  mScale;       //!< Conversion coef from screen mouse coord to phisical coord of model
    QPoint mStartPoint;  //!< Point where mouse button pressed
    bool   mLeftPressed; //!< Flag for left mouse button pressed
  public:
    Sd3dModeVertMove( SdPItemPart *part );

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

#endif // SD3DMODEVERTMOVE_H
