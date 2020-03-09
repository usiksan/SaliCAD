/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base object for 3d graphics
*/
#include "Sd3dObject.h"

Sd3dObject::Sd3dObject() :
  SdObject()
  {

  }

void Sd3dObject::draw(QOpenGLFunctions_2_0 *f) const
  {
  Q_UNUSED(f)
  }


SdClass Sd3dObject::getClass() const
  {
  return dct3D;
  }
