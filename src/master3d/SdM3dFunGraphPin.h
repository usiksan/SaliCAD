/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds pin from its vertex, pad description, pin name and pin number positions,
  text attributes
*/
#ifndef SDM3DFUNGRAPHPIN_H
#define SDM3DFUNGRAPHPIN_H

#include "SdM3dFunction.h"

class SdM3dFunGraphPin : public SdM3dFunction
  {
  public:
    SdM3dFunGraphPin() :
      //                 pin              pinPos              pad                numberPos         number             numberAttr           namePos            nameAttr
      SdM3dFunction( SDM3D_TYPE_GRAPH, SDM3D_TYPE_VERTEX, SDM3D_TYPE_STRING, SDM3D_TYPE_VERTEX, SDM3D_TYPE_STRING, SDM3D_TYPE_STRING, SDM3D_TYPE_VERTEX, SDM3D_TYPE_STRING )
      {

      }

    // SdM3dValue interface
  public:
    virtual SdM3dGraph toGraph() const override
      {
      return SdM3dGraph( mParamList[0]->toVertex(), mParamList[1]->toString(), mParamList[2]->toVertex(), mParamList[3]->toString(), mParamList[4]->toString(), mParamList[5]->toVertex(), mParamList[6]->toString() );
      }
  };

#endif // SDM3DFUNGRAPHPIN_H
