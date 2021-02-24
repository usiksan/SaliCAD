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
      //Build normals
      SdM3dRegion normals;
      if( srcFace.mContour.count() >= 3 ) {
        QVector3D normal = QVector3D::normal( srcFace.mContour.at(0), srcFace.mContour.at(1), srcFace.mContour.at(2) );
        for( int i = 0; i < srcFace.mContour.count(); i++ )
          normals.append( normal );
        }
      faseSet->faceAdd( Sd3dFace( srcFace.mContour, normals, material ) );
      qDebug() << srcFace.mContour;
      }
    mPart->insertChild( faseSet, nullptr );
    }
  }
