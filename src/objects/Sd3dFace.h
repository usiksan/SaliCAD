/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3D face. Face is flat surface bounded by region
*/
#ifndef SD3DFACE_H
#define SD3DFACE_H

#include "SdRect.h"
#include "Sd3dPoint.h"
#include "Sd3dFaceMaterial.h"

#include <QList>

class Sd3dFace
  {
    Sd3dPointList    mRegion;     //!< Face region
    Sd3dPointList    mNormal;     //!< Face normal vector for foregraund and background side detection
    Sd3dFaceMaterial mMaterial;   //!< Face material description
  public:
    Sd3dFace();
    Sd3dFace( const Sd3dPointList &region, const Sd3dPointList &normal, const Sd3dFaceMaterial &faceMaterial );
    Sd3dFace( const Sd3dPointList &region, const Sd3dPointList &normal, quint32 faceColor );
    Sd3dFace( const QList<QVector3D> &region, const QList<QVector3D> &normal, const Sd3dFaceMaterial &faceMaterial );


    //!
    //! \brief paint Draw triangle with OpenGL
    //! \param f     Pointer to OpenGL functions
    //!
    void        paint( QOpenGLFunctions_2_0 *f ) const;

    //!
    //! \brief write Write triangle to json file
    //! \return      JSON object with triangle
    //!
    QJsonObject write() const;

    //!
    //! \brief read Read triangle from json file
    //! \param obj  JSON object with triangle
    //!
    void        read( const QJsonObject &obj );

    //!
    //! \brief overRect Return over rect of face projected to XY surface
    //! \return         Over rect
    //!
    SdRect      overRect() const;

  };

#endif // SD3DFACE_H
