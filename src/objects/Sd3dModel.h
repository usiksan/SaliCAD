/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d object which represents face set model (Model which consists of face set)
  Each face is flate surface bounded polyline
  With comparison Sd3dFaceSet this object contains simplified faces (no normal, single face color)
*/
#ifndef SD3DMODEL_H
#define SD3DMODEL_H

#include "Sd3dFace.h"

using Sd3dModel = QList<Sd3dFace>;

//!
//! \brief sd3dModelWrite Writes model into JSON array object
//! \param model          3d model
//! \return               JSON array object with writed 3d model
//!
QJsonArray sd3dModelWrite( const Sd3dModel &model );


//!
//! \brief sd3dModelRead Reads model from JSON array object
//! \param array         JSON array object from which will be readed 3d model
//! \return              3d model
//!
Sd3dModel  sd3dModelRead( const QJsonArray &array );

#endif // SD3DMODEL_H
