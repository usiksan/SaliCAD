/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d mode used to translate model in horizontal area
*/
#ifndef SD3DMODEHORZMOVE_H
#define SD3DMODEHORZMOVE_H

#include "Sd3dModeMat.h"
#include "objects/SdPItemPart.h"

class Sd3dModeHorzMove : public Sd3dModeMat
  {
    float  mOffsetX;     //!< Current offset for transfer model
    float  mOffsetY;
    float  mScale;       //!< Conversion coef from screen mouse coord to phisical coord of model
    QPoint mStartPoint;  //!< Point where mouse button pressed
    bool   mLeftPressed; //!< Flag for left mouse button pressed
  public:
    Sd3dModeHorzMove( SdPItemPart *part );

    // Sd3dMode interface
  public:
    virtual void mousePressEvent(SdWView3d *viewer, QMouseEvent *event) override;
    virtual void mouseReleaseEvent(SdWView3d *viewer, QMouseEvent *event) override;
    virtual bool mouseMoveEvent(SdWView3d *viewer, QMouseEvent *event) override;
    virtual void draw3d(QOpenGLFunctions_2_0 *f) override;
  };

#endif // SD3DMODEHORZMOVE_H
