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
#ifndef SDSCRIPTVALUEFUNLFACEWALLLIST_H
#define SDSCRIPTVALUEFUNLFACEWALLLIST_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunLFaceWallList : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunLFaceWallList() :
      //                                                      botFaceList              topFaceList             close
      SdScriptValueFunction( SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_BOOL )
      {
      }

    //!
    //! \brief toFaceList Convert object to list of face each of which is list of vertex index
    //! \return           List of face each of which is list of vertex index
    //!
    virtual Sd3drFaceList  toFaceList() const override { return Sd3dModel::faceListWallList( mParamList[0]->toFaceList(), mParamList[1]->toFaceList(), mParamList[2]->toBool() ); }

  };


#endif // SDSCRIPTVALUEFUNLFACEWALLLIST_H
