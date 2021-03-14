#include "Sd3dModel.h"


//!
//! \brief sd3dModelWrite Writes model into JSON array object
//! \param model          3d model
//! \return               JSON array object with writed 3d model
//!
QJsonArray sd3dModelWrite(const Sd3dModel &model)
  {
  QJsonArray ar;
  for( auto const &face : model )
    ar.append( face.write() );
  return ar;
  }




//!
//! \brief sd3dModelRead Reads model from JSON array object
//! \param array         JSON array object from which will be readed 3d model
//! \return              3d model
//!
Sd3dModel sd3dModelRead(const QJsonArray &array)
  {
  Sd3dModel model;
  model.reserve( array.count() );
  Sd3dFace face;
  for( auto it = array.constBegin(); it != array.constEnd(); it++ ) {
    face.read( it->toObject() );
    model.append( face );
    }
  return model;
  }
