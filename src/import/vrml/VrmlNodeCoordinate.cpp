#include "VrmlNodeCoordinate.h"
#include "SdScanerVrml.h"

#include "3dr/Sd3drModel.h"

VrmlNodeCoordinate::VrmlNodeCoordinate() :
  VrmlNode()
  {

  }




void VrmlNodeCoordinate::generateFaces(Sd3drModel *model, Sd3drInstance &instance, Sd3drBody &body) const
  {
  Q_UNUSED(instance)
  Q_UNUSED(body)
  if( mIndexConvertor.count() != mPointList.count() ) {
    for( auto p : mPointList )
      mIndexConvertor.append( model->vertexAppend(p.toVector3d()) );
    }
  }



bool VrmlNodeCoordinate::parse(SdScanerVrml *scaner, const QString &fieldType)
  {
  if( fieldType == QStringLiteral("point") ) {
    mPointList.clear();
    scaner->parseVectorTable( mPointList, QStringLiteral("Waiting point list") );
    return true;
    }
  return false;
  }
