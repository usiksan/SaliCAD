#include "Sd3dGraphModel.h"
#include "Sd3dDraw.h"

Sd3dGraphModel::Sd3dGraphModel()
  {

  }




//!
//! \brief matrixMapInPlace Mapping all faces of model throught given matrix. Operation maked in place
//! \param matrix           Mapping matrix
//!
void Sd3dGraphModel::matrixMapInPlace(QMatrix4x4 matrix)
  {
  sd3dModelMapInPlace( mModel, matrix );
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




//!
//! \brief cloneFrom Overrided function. We copy model from source object
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void Sd3dGraphModel::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  Sd3dGraph::cloneFrom( src, copyMap, next );
  SdPtrConst<Sd3dGraphModel> model(src);
  if( model.isValid() )
    mModel = model->mModel;
  }







SdRect Sd3dGraphModel::getOverRect() const
  {
  return SdRect{};
  }



void Sd3dGraphModel::draw3d(QOpenGLFunctions_2_0 *f) const
  {
  Sd3dDraw::drawModel( f, mModel );
  }





