#include "Sd3dGraphModel.h"
#include "Sd3dDraw.h"
#include "SdJsonIO.h"

Sd3dGraphModel::Sd3dGraphModel()
  {

  }

Sd3dGraphModel::Sd3dGraphModel(const Sd3dModel &model) :
  mModel(model)
  {

  }




























//!
//! \brief matrixMapInPlace Mapping all faces of model throught given matrix. Operation maked in place
//! \param matrix           Mapping matrix
//!
void Sd3dGraphModel::matrixMapInPlace(QMatrix4x4 matrix)
  {
  //sd3dModelMapInPlace( mModel, matrix );
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




//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void Sd3dGraphModel::json(SdJsonWriter &js) const
  {
  mModel.json( js );
  Sd3dGraph::json( js );
  }





//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void Sd3dGraphModel::json(const SdJsonReader &js)
  {
  mModel.json( js );
  Sd3dGraph::json( js );
  }







SdRect Sd3dGraphModel::getOverRect() const
  {
  return SdRect{};
  }



void Sd3dGraphModel::draw3d(QOpenGLFunctions_2_0 *f) const
  {
  mModel.draw3d( f );
  }





