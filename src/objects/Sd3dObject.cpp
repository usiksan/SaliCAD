#include "Sd3dObject.h"

Sd3dObject::Sd3dObject() :
  SdObject()
  {

  }


SdClass Sd3dObject::getClass() const
  {
  return dct3D;
  }
