/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d object which represents face set model (Model which consists of face set)
  Each face is flate surface bounded polyline
  With comparison Sd3dFaceSet this object contains simplified
*/
#ifndef SD3DMODEL_H
#define SD3DMODEL_H

#include "Sd3dObject.h"

class Sd3dModel : public Sd3dObject
  {
  public:
    Sd3dModel();
  };

#endif // SD3DMODEL_H
