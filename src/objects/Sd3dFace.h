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
    QList<Sd3dPoint> mRegion; //! Face region
    Sd3dPoint        mNormal; //! Face normal vector for foregraund and background side detection
    quint32          mColor;  //! Face color
  public:
    Sd3dFace();

    //!
    //! \brief readStep Reads one face from step file reader
    //! \param faceId   Face id for reading
    //! \param reader   STEP file reader
    //!
    bool        readStep( const QString &faceId, const SdStepReader &reader );

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

  private:
    bool stepPresentationStyleAssignment( const QString &param, const SdStepReader &reader );
    bool stepSurfaceStyleUsage( const QString &param, const SdStepReader &reader );
    bool stepSurfaceStyleSide( const QString &param, const SdStepReader &reader );
    bool stepAdvancedFace( const QString &param, const SdStepReader &reader );
  };

#endif // SD3DFACE_H
