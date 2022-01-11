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

  Array of 3d vertex.
*/
#ifndef SDSCRIPTVALUEARRAY3DVERTEX_H
#define SDSCRIPTVALUEARRAY3DVERTEX_H

#include "SdScriptValueArray.h"

class SdScriptValueArray3dVertex : public SdScriptValueArray
  {
  public:
    SdScriptValueArray3dVertex() {}

    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char                type() const override { return SD_SCRIPT_TYPE_REGION; }

    //!
    //! \brief toRegion Convert object to 3d region which is closed region of 3d vertex
    //! \return         3d region which is closed region of 3d vertex
    //!
    virtual SdScriptVal3dRegion toRegion() const override;

    // SdM3dArray interface
  public:
    //!
    //! \brief typeOfElement Return type of element of array
    //! \return              Type of element of array
    //!
    virtual char                typeOfElement() const override { return SD_SCRIPT_TYPE_VERTEX; }
  };

#endif // SDSCRIPTVALUEARRAY3DVERTEX_H
