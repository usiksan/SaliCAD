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
#ifndef SD3DFACEEX_H
#define SD3DFACEEX_H

#include "SdRect.h"
#include "Sd3dPoint.h"
#include "Sd3dFaceMaterial.h"

#include <QList>

class Sd3dFaceEx
  {
    Sd3dPointList    mRegion;     //!< Face region
    Sd3dPointList    mNormal;     //!< Face normal vector for foregraund and background side detection
    Sd3dFaceMaterial mMaterial;   //!< Face material description
  public:
    Sd3dFaceEx();
    Sd3dFaceEx( const Sd3dPointList &region, const Sd3dPointList &normal, const Sd3dFaceMaterial &faceMaterial );
    Sd3dFaceEx( const Sd3dPointList &region, const Sd3dPointList &normal, quint32 faceColor );
    Sd3dFaceEx( const QList<QVector3D> &region, const QList<QVector3D> &normal, const Sd3dFaceMaterial &faceMaterial );


    //!
    //! \brief paint Draw triangle with OpenGL
    //! \param f     Pointer to OpenGL functions
    //!
    void        paint( QOpenGLFunctions_2_0 *f ) const;

    //!
    //! \brief json Overloaded function to write object content into json writer
    //! \param js   Json writer
    //!
    void        json( SvJsonWriter &js ) const;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //! \param js   Json reader
    //!
    void        json( const SvJsonReader &js);

    //!
    //! \brief overRect Return over rect of face projected to XY surface
    //! \return         Over rect
    //!
    SdRect      overRect() const;

  };

#endif // SD3DFACEEX_H
