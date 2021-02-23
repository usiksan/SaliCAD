/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function add extrudes the model from the top side of source model in the direction of the vector
*/
#ifndef SDM3DFUNMODELADDEXTRUDE_H
#define SDM3DFUNMODELADDEXTRUDE_H

#include "SdM3dFunction.h"

class SdM3dFunModelAddExtrude : public SdM3dFunction
  {
  public:
    SdM3dFunModelAddExtrude();

    // SdM3dValue interface
  public:
    virtual SdM3dModel toModel() const override;

    static  SdM3dModel modelAddExtrude( SdM3dModel src, QVector3D vector, QColor color );
  };

#endif // SDM3DFUNMODELADDEXTRUDE_H
