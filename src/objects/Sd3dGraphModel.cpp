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



void Sd3dGraphModel::scriptSet(const QString script, SdUndo *undo)
  {
  if( undo )
    undo->script( &mModelScript, &mModel );
  mModelScript = script;
  mModel.build( mModelScript );
  }






//!
//! \brief matrixMapInPlace Mapping all faces of model throught given matrix. Operation maked in place
//! \param matrix           Mapping matrix
//!
void Sd3dGraphModel::matrixMapInPlace(QMatrix4x4 matrix)
  {
  mTransform *= matrix;
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
  if( model.isValid() ) {
    mModel        = model->mModel;
    mModelScript = model->mModelScript;
    mTransform    = model->mTransform;
    }
  }




//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void Sd3dGraphModel::json(SdJsonWriter &js) const
  {
  js.jsonMatrix4x4( QStringLiteral("Transform"), mTransform );
  if( mModelScript.isEmpty() )
    //If script not assigned then we save model itself
    mModel.json( js );
  else
    //If script present we save script as more compact than model
    js.jsonString( QStringLiteral("Script"), mModelScript );
  Sd3dGraph::json( js );
  }





//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void Sd3dGraphModel::json(const SdJsonReader &js)
  {
  js.jsonMatrix4x4( QStringLiteral("Transform"), mTransform );
  if( js.contains(QStringLiteral("Script")) ) {
    //Script present, we read it and build model
    js.jsonString( QStringLiteral("Script"), mModelScript );
    mModel.build( mModelScript );
    }
  else
    //Script not present, we read model
    mModel.json( js );
  Sd3dGraph::json( js );
  }







SdRect Sd3dGraphModel::getOverRect() const
  {
  return SdRect{};
  }



void Sd3dGraphModel::draw3d(QOpenGLFunctions_2_0 *f) const
  {
  f->glPushMatrix();
  f->glMultMatrixf( mTransform.constData() );
  mModel.draw3d( f );
  f->glPopMatrix();
  }





