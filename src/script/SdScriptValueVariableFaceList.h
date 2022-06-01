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

  Variable of segment
*/
#ifndef SDSCRIPTVALUEVARIABLEFACELIST_H
#define SDSCRIPTVALUEVARIABLEFACELIST_H

#include "SdScriptValueVariable.h"

class SdScriptValueVariableFaceList : public SdScriptValueVariable
  {
    Sd3drFaceList mFaceList; //!< FaceList value of variable
  public:
    SdScriptValueVariableFaceList() {}



    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char          type() const override { return SD_SCRIPT_TYPE_FACE_LIST; }

    //!
    //! \brief toFaceList Convert object to list of face each of which is list of vertex index
    //! \return           List of face each of which is list of vertex index
    //!
    virtual Sd3drFaceList toFaceList() const override { return mFaceList; }

    // SdM3dVariable interface
  public:
    //!
    //! \brief assign Assignment value to variable
    //! \param src    source of value
    //!
    virtual void          assign(SdScriptValuePtr src) override { mFaceList = src->toFaceList(); }
  };

#endif // SDSCRIPTVALUEVARIABLEFACELIST_H
