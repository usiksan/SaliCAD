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

  The function builds pad description on pad diametr, hole diametr and mask addon
*/
#ifndef SDSCRIPTVALUEFUNSTRINGPADCIRCLETHROU_H
#define SDSCRIPTVALUEFUNSTRINGPADCIRCLETHROU_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunStringPadCircleThrou : public SdScriptValueFunction
  {
  public:
    //                                                               pad                    pad diametr           hole diametr          mask addon
    SdScriptValueFunStringPadCircleThrou() : SdScriptValueFunction( SD_SCRIPT_TYPE_STRING, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toString Convert object to string
    //! \return         String
    //!
    virtual QString toString() const override
      {
      return QString("c%1d%2m%3").arg( mParamList[0]->toFloat(), 0, 'f', 1 )
          .arg( mParamList[1]->toFloat(), 0, 'f', 1 )
          .arg( mParamList[2]->toFloat(), 0, 'f', 1 );
      }
  };

#endif // SDSCRIPTVALUEFUNSTRINGPADCIRCLETHROU_H
