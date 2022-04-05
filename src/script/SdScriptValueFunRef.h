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

class SdObject;

class SdScriptValueFunRef : public SdScriptValueFunction
  {
    SdObject *mRef;
    QString   mParam;
  public:
    SdScriptValueFunRef();

    //!
    //! \brief title Return variable title
    //! \return      Variable title
    //!
    QString title() const;

    QString valueStr() const;

    float   valueGet() const;

    void    valueSet( float v );
  };

#endif // SDSCRIPTVALUEFUNREF_H
