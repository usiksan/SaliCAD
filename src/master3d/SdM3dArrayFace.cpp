#include "SdM3dArrayFace.h"

SdM3dArrayFace::SdM3dArrayFace()
  {

  }


char SdM3dArrayFace::type() const
  {
  return SDM3D_TYPE_MODEL;
  }

SdM3dModel SdM3dArrayFace::toModel() const
  {
  SdM3dModel model;
  for( auto ptr : mArray )
    model.append( ptr->toFace() );
  return model;
  }

char SdM3dArrayFace::typeOfElement() const
  {
  return SDM3D_TYPE_FACE;
  }
