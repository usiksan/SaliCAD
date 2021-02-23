/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  Creates copy of source model translated with vector
*/
#include "SdM3dFunModelTranslate.h"
#include "SdM3dFunFaceTranslate.h"

SdM3dFunModelTranslate::SdM3dFunModelTranslate() :
  SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_MODEL, SDM3D_TYPE_VERTEX )
  {

  }


SdM3dModel SdM3dFunModelTranslate::toModel() const
  {
  return modelTranslate( mParamList[0]->toModel(), mParamList[1]->toVertex() );
  }




//!
//! \brief modelTranslate Creates copy of source model translated with vector
//! \param model          Source model
//! \param offset         Offset vector
//! \return               Translated model
//!
SdM3dModel SdM3dFunModelTranslate::modelTranslate(SdM3dModel model, QVector3D offset)
  {
  SdM3dModel dest;
  for( auto const &face : model )
    dest.append( SdM3dFunFaceTranslate::faceTranslate( face, offset ) );
  return dest;
  }
