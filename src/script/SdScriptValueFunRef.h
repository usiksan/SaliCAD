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

  This function appends one field to program param table as component parameter or sheet parameter
*/
#ifndef SDSCRIPTVALUEFUNREF_H
#define SDSCRIPTVALUEFUNREF_H

#include "SdScriptValueFunction.h"
#include "SdScriptParamRef.h"


class SdScriptValueFunRef : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunRef();

    //!
    //! \brief dimensions Returns dimensions of reference
    //! \return           Dimensions of reference
    //!
    //! In schematic calculations all values are physical. For example 10kOm
    //! Dimension will return "Om" in this case
    QString dimension() const { return mParamList[0]->toString(); }

    //!
    //! \brief row Returns row of reference
    //! \return    Row of reference
    //!
    //! Some values of components are not any but member one of some Rows (aka E12, E24 and so on).
    //! This function return souch row. For example "E6"
    QString row() const { return mParamList[1]->toString(); }
  };

#endif // SDSCRIPTVALUEFUNREF_H
