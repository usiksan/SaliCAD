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

  Binary operation for union of two face lists
*/
#ifndef SDSCRIPTVALUEOPBINARYFACELISTADD_H
#define SDSCRIPTVALUEOPBINARYFACELISTADD_H

#include "SdScriptValueOpBinaryFloat.h"

class SdScriptValueOpBinaryFaceListAdd : public SdScriptValueOpBinary
  {
  public:
    SdScriptValueOpBinaryFaceListAdd( SdScriptValue *first, SdScriptValue *second ) : SdScriptValueOpBinary( first, second ) {}

    // SdM3dValue interface
  public:
    virtual char  type() const override { return SD_SCRIPT_TYPE_FACE_LIST; }

    //!
    //! \brief toFaceList Convert object to list of face each of which is list of vertex index
    //! \return           List of face each of which is list of vertex index
    //!
    virtual Sd3drFaceList toFaceList() const override { return mFirstOperand->toFaceList() + mSecondOperand->toFaceList(); }
  };

#endif // SDSCRIPTVALUEOPBINARYFACELISTADD_H
