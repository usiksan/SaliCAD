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

  The function builds string of pad description for rectangle pad with through hole
*/
#ifndef SDSCRIPTVALUEFUNSTRINGPADRECTTHROU_H
#define SDSCRIPTVALUEFUNSTRINGPADRECTTHROU_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunStringPadRectThrou : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunStringPadRectThrou() : SdScriptValueFunction( SD_SCRIPT_TYPE_STRING, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toString Convert object to string
    //! \return         String
    //!
    virtual QString toString() const override
      {
      return QString("r%1x%2d%3m%4").arg( mParamList[0]->toFloat(), 0, 'f', 1 )
          .arg( mParamList[1]->toFloat(), 0, 'f', 1 )
          .arg( mParamList[2]->toFloat(), 0, 'f', 1 )
          .arg( mParamList[3]->toFloat(), 0, 'f', 1 );
      }
  };

#endif // SDSCRIPTVALUEFUNSTRINGPADRECTTHROU_H
