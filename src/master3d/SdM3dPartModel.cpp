#include "SdM3dPartModel.h"
#include "objects/SdPItemPart.h"
#include "objects/Sd3dFaceSet.h"

#include <QDebug>

SdM3dPartModel::SdM3dPartModel(SdPItemPart *part) :
  SdM3dVariable(),
  mPart(part)
  {

  }



void SdM3dPartModel::assign(SdM3dValuePtr src)
  {
  SdM3dModel model = src->toModel();
  if( model.count() ) {
    Sd3dFaceSet *faseSet = new Sd3dFaceSet();
    for( const auto &srcFace : model ) {
      //Convert srcFace to Sd3dFace
      Sd3dFaceMaterial material;
      material.setColor( srcFace.mColor );
      faseSet->faceAdd( Sd3dFace( srcFace.mContour, SdM3dRegion(), material ) );
      qDebug() << srcFace.mContour;
      }
    mPart->insertChild( faseSet, nullptr );
    }
  }
