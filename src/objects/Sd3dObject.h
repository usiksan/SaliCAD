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
#ifndef SD3DOBJECT_H
#define SD3DOBJECT_H

#include "SdObject.h"
#include <QOpenGLFunctions_2_0>


class Sd3dObject : public SdObject
  {
  public:
    Sd3dObject();

    virtual void draw( QOpenGLFunctions_2_0 *f ) const;

    // SdObject interface
  public:
    virtual SdClass getClass() const override;
  };

#endif // SD3DOBJECT_H
