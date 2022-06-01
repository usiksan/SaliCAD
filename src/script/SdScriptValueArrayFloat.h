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

  Array of floats
*/
#ifndef SDSCRIPTVALUEARRAYFLOAT_H
#define SDSCRIPTVALUEARRAYFLOAT_H

#include "SdScriptValueArray.h"

class SdScriptValueArrayFloat : public SdScriptValueArray
  {
  public:
    SdScriptValueArrayFloat() {}

    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char              type() const override { return SD_SCRIPT_TYPE_AFLOAT; }

    //!
    //! \brief toIndex Convert object to float list
    //! \return        Float list
    //!
    virtual QList<float>      toFloatList() const override;

    // SdM3dArray interface
  public:
    //!
    //! \brief typeOfElement Return type of element of array
    //! \return              Type of element of array
    //!
    virtual char              typeOfElement() const override  { return SD_SCRIPT_TYPE_FLOAT; }
  };

#endif // SDSCRIPTVALUEARRAYFLOAT_H
