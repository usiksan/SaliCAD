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
    QList<Sd3dPoint> mRegion;     //! Face region
    Sd3dPoint        mNormal;     //! Face normal vector for foregraund and background side detection
    quint32          mFaceColor;  //! Face color
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
    //Member function pointer
    using Sd3dFaceMemberStep = bool (Sd3dFace::*)( const QString &param, const SdStepReader &reader );

    bool stepParseParams( const QString &param, const SdStepReader &reader, QList<Sd3dFaceMemberStep> paramHandlers );

    bool stepParseParamList( const QString &param, const SdStepReader &reader, Sd3dFaceMemberStep paramHandler );
    bool stepParseParamIdList( const QString &param, const SdStepReader &reader, Sd3dFaceMemberStep paramHandler );

    bool stepParamSkeep( const QString &param, const SdStepReader &reader );
    bool stepPresentationStyleAssignmentList( const QString &param, const SdStepReader &reader );
    bool stepPresentationStyleAssignment( const QString &param, const SdStepReader &reader );
    bool stepSurfaceStyleUsageList( const QString &param, const SdStepReader &reader );
    bool stepSurfaceStyleUsage( const QString &param, const SdStepReader &reader );
    bool stepSurfaceSideStyle( const QString &param, const SdStepReader &reader );
    bool stepSurfaceStyleFillAreaList( const QString &param, const SdStepReader &reader );
    bool stepSurfaceStyleFillArea( const QString &param, const SdStepReader &reader );
    bool stepFillAreaStyle( const QString &param, const SdStepReader &reader );
    bool stepFillAreaStyleColorList( const QString &param, const SdStepReader &reader );
    bool stepFillAreaStyleColor( const QString &param, const SdStepReader &reader );
    bool stepAdvancedFace( const QString &param, const SdStepReader &reader );
  };

#endif // SD3DFACE_H
