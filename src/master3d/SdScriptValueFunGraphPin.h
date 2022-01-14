/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Script language is C-style hi-level language for programming 3d models, 2d parametric graphics and
  schematic calculations.

  Embedded function.

  The function builds pin from its vertex, pad description, pin name and pin number positions,
  text attributes
*/
#ifndef SDSCRIPTVALUEFUNGRAPHPIN_H
#define SDSCRIPTVALUEFUNGRAPHPIN_H

#include "SdScriptValueFunction.h"

class SdM3dFunGraphPin : public SdScriptValueFunction
  {
  public:
    SdM3dFunGraphPin() :
      //                      pin                   pinPos                 pad                    numberPos              number                 numberAttr             namePos                nameAttr
      SdScriptValueFunction( SD_SCRIPT_TYPE_GRAPH, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_STRING, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_STRING, SD_SCRIPT_TYPE_STRING, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_STRING )
      {

      }

    // SdM3dValue interface
  public:
    //!
    //! \brief toGraph Convert object to 2d graph
    //! \return        2d graph
    //!
    virtual SdScriptVal2dGraph toGraph() const override
      {
      return SdScriptVal2dGraph( mParamList[0]->toVertex(), mParamList[1]->toString(), mParamList[2]->toVertex(), mParamList[3]->toString(), mParamList[4]->toString(), mParamList[5]->toVertex(), mParamList[6]->toString() );
      }
  };

#endif // SDSCRIPTVALUEFUNGRAPHPIN_H
