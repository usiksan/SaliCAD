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

  Build color structure with individual color channel values
*/
#ifndef SDSCRIPTVALUEFUNCOLORBUILD_H
#define SDSCRIPTVALUEFUNCOLORBUILD_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunColorBuild : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunColorBuild();

    // SdM3dValue interface
  public:
    //!
    //! \brief toColor Convert object to color
    //! \return        Color
    //!
    virtual QColor toColor() const override;

    //!
    //! \brief colorBuild Convert individual color channel values to color
    //! \param r          Red
    //! \param g          Green
    //! \param b          Blue
    //! \param a          Alpha
    //! \return           Color structure
    //!
    static  QColor colorBuild( float r, float g, float b, float a );
  };


#endif // SDSCRIPTVALUEFUNCOLORBUILD_H
