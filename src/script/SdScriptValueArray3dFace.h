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

  Array of 3d faces.
*/
#ifndef SDSCRIPTVALUEARRAY3DFACE_H
#define SDSCRIPTVALUEARRAY3DFACE_H

#include "SdScriptValueArray.h"

class SdScriptValueArray3dFace : public SdScriptValueArray
  {
  public:
    SdScriptValueArray3dFace() {}

    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char               type() const override  { return SD_SCRIPT_TYPE_FACE_LIST; }

    //!
    //! \brief toFaceList Convert object to list of face each of which is list of vertex index
    //! \return           List of face each of which is list of vertex index
    //!
    virtual Sd3drFaceList      toFaceList() const override;

    // SdM3dArray interface
  public:
    //!
    //! \brief typeOfElement Return type of element of array
    //! \return              Type of element of array
    //!
    virtual char               typeOfElement() const override { return SD_SCRIPT_TYPE_FACE; }
  };

#endif // SDSCRIPTVALUEARRAY3DFACE_H
