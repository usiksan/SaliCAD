/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3D face. Face is flat surface bounded by region
*/
#ifndef SD3DFACE_H
#define SD3DFACE_H

#include "Sd3dPoint.h"

#include <QList>

class Sd3dFace
  {
    QList<Sd3dPoint> mRegion;
  public:
    Sd3dFace();
  };

#endif // SD3DFACE_H
