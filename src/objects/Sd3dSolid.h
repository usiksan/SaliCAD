/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d solid
  Solid consists from one or more faces which are representes as regions.
  Color of all faces is equals of solid color
  Solid can be single or matrix of solids
  Assume coords in mm
*/
#ifndef SD3DSOLID_H
#define SD3DSOLID_H

#include "Sd3dRegion.h"

#include <QColor>
#include <QJsonObject>


class Sd3dSolid
  {
    QList<Sd3dRegion> mFaces;        //!< Faces of solid
    QColor            mColor;        //!< Color of solid (of all solids faces)
    int               mRows;         //!< Row count of solid instances
    int               mColumns;      //!< Column count of solid instances
    QVector3D         mRowOffset;    //!< Distance between solids in row
    QVector3D         mColumnOffset; //!< Distance between solids in column
  public:
    Sd3dSolid();
  };

#endif // SD3DSOLID_H
