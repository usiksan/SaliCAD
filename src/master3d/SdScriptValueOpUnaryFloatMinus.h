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

  Negatiation of float value
*/
#ifndef SDSCRIPTVALUEOPUNARYFLOATMINUS_H
#define SDSCRIPTVALUEOPUNARYFLOATMINUS_H

#include "SdScriptValueOpUnary.h"

class SdScriptValueOpUnaryFloatMinus : public SdScriptValueOpUnary
  {
  public:
    SdScriptValueOpUnaryFloatMinus(SdM3dValuePtr op) : SdScriptValueOpUnary(op) {}


    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char  type() const override { return SD_SCRIPT_TYPE_FLOAT; }

    //!
    //! \brief toFloat Convert object ot float value
    //! \return        float value
    //!
    virtual float toFloat() const override { return -mOperand->toFloat(); }
  };

#endif // SDSCRIPTVALUEOPUNARYFLOATMINUS_H
