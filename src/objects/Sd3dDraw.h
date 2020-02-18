#ifndef SD3DDRAW_H
#define SD3DDRAW_H

#include "SdPointList.h"
#include "SdRect.h"

#include <QOpenGLFunctions_2_0>

class Sd3dDraw
  {
  public:
    static void flatPanel( QOpenGLFunctions_2_0 *f, SdPointList list, int z, unsigned color );

    static void qube( QOpenGLFunctions_2_0 *f, SdRect rect, int z, unsigned color );
  };

#endif // SD3DDRAW_H
