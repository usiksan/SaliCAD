/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Context for graphics draw in 3d
*/
#ifndef SDCONTEXT3D_H
#define SDCONTEXT3D_H

#include "SdContext.h"

class QOpenGLFunctions_2_0;

class SdContext3d : public SdContext
  {
    QOpenGLFunctions_2_0 *mFunc; //OpenGL functions
  public:
    SdContext3d();

    // SdContext interface
  public:
    virtual void setPen(int width, QColor color, int lineStyle) override;
    virtual void line(SdPoint a, SdPoint b) override;
    virtual void fillRect(SdRect r) override;
    virtual void arc(SdPoint center, SdPoint start, SdPoint stop) override;
    virtual void circle(SdPoint center, int radius) override;
    virtual void circleFill(SdPoint center, int radius) override;
    virtual void regionFill(const SdPointList &points, const SdPropLine &prop) override;
    virtual void polygon(const SdPointList &points, const SdPolyWindowList &windows, SdLayer *layer) override;
  };

#endif // SDCONTEXT3D_H
