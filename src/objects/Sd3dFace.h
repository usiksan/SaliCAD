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

#include "Sd3dPoint.h"
#include "step/SdStepReader.h"


#include <QList>

class Sd3dFace
  {
    QList<Sd3dPoint> mRegion;     //!< Face region
    Sd3dPoint        mNormal;     //!< Face normal vector for foregraund and background side detection
    quint32          mFaceColor;  //!< Face color
  public:
    Sd3dFace();
    Sd3dFace( QList<Sd3dPoint> region, Sd3dPoint normal, quint32 faceColor );


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

  };

#endif // SD3DFACE_H
