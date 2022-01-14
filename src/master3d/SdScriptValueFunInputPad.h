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

  The function builds and binds input field for pad
*/
#ifndef SDSCRIPTVALUEFUNINPUTPAD_H
#define SDSCRIPTVALUEFUNINPUTPAD_H

#include "SdScriptValueFunInput.h"

class SdScriptValueFunInputPad : public SdScriptValueFunInput
  {
    QString mPreviousPad;
  public:
    SdScriptValueFunInputPad(QTableWidget *tableWidget);

    // SdM3dValue interface
  public:
    //!
    //! \brief toString Convert object to string
    //! \return         String
    //!
    virtual QString toString() const override;
  };

#endif // SDSCRIPTVALUEFUNINPUTPAD_H
