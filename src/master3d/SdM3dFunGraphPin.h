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
      SdM3dFunction( SD_SCRIPT_TYPE_GRAPH, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_STRING, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_STRING, SD_SCRIPT_TYPE_STRING, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_STRING )
      {

      }

    // SdM3dValue interface
  public:
    virtual SdScriptVal2dGraph toGraph() const override
      {
      return SdScriptVal2dGraph( mParamList[0]->toVertex(), mParamList[1]->toString(), mParamList[2]->toVertex(), mParamList[3]->toString(), mParamList[4]->toString(), mParamList[5]->toVertex(), mParamList[6]->toString() );
      }
  };

#endif // SDM3DFUNGRAPHPIN_H
