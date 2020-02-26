#include "Sd3dObject.h"

Sd3dObject::Sd3dObject() :
  SdObject()
  {

  }

void Sd3dObject::draw(QOpenGLFunctions_2_0 *f)
  {
  Q_UNUSED(f)
  }


SdClass Sd3dObject::getClass() const
  {
  return dct3D;
  }
