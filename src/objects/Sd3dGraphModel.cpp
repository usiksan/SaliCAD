#include "Sd3dGraphModel.h"
#include "Sd3dDraw.h"

Sd3dGraphModel::Sd3dGraphModel()
  {

  }

void Sd3dGraphModel::writeObject(QJsonObject &obj) const
  {
  Sd3dGraph::writeObject( obj );
  obj.insert( QStringLiteral("model"), sd3dModelWrite(mModel) );
  }



void Sd3dGraphModel::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  Sd3dGraph::readObject( map, obj );
  mModel = sd3dModelRead( obj.value( QStringLiteral("model") ).toArray() );
  }



SdRect Sd3dGraphModel::getOverRect() const
  {
  return SdRect{};
  }



void Sd3dGraphModel::draw3d(QOpenGLFunctions_2_0 *f) const
  {
  Sd3dDraw::drawModel( f, mModel );
  }





