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

  The function builds wall from two faces
*/
#ifndef SDSCRIPTVALUEFUNLFACEWALL_H
#define SDSCRIPTVALUEFUNLFACEWALL_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunLFaceWall : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunLFaceWall() :
      SdScriptValueFunction( SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_BOOL )
      {
      }

    //!
    //! \brief toFaceList Convert object to list of face each of which is list of vertex index
    //! \return           List of face each of which is list of vertex index
    //!
    virtual Sd3drFaceList  toFaceList() const override { return Sd3drModel::faceListWall( mParamList[0]->toFace(), mParamList[1]->toFace(), mParamList[2]->toBool() ); }

  };

#endif // SDSCRIPTVALUEFUNLFACEWALL_H
