#include "Sd3dFace.h"

#include <QDebug>

Sd3dFace::Sd3dFace()
  {

  }




//!
//! \brief readStep Reads one face from step file reader
//! \param faceId   Face id for reading
//! \param reader   STEP file reader
//!
bool Sd3dFace::readStep(const QString &faceId, const SdStepReader &reader)
  {
  QStringList paramList = reader.parseListFromValue( faceId );
  if( paramList.count() == 3 ) {
    if( !stepPresentationStyleAssignment( paramList.at(1), reader ) )
      return false;
    if( !stepAdvancedFace( paramList.at(2), reader ) )
      return false;
    }
  return true;
  }





//!
//! \brief paint Draw triangle with OpenGL
//! \param f     Pointer to OpenGL functions
//!
void Sd3dFace::paint(QOpenGLFunctions_2_0 *f) const
  {

  }





//!
//! \brief write Write triangle to json file
//! \return      JSON object with triangle
//!
QJsonObject Sd3dFace::write() const
  {

  }




//!
//! \brief read Read triangle from json file
//! \param obj  JSON object with triangle
//!
void Sd3dFace::read(const QJsonObject &obj)
  {

  }

bool Sd3dFace::stepPresentationStyleAssignment(const QString &param, const SdStepReader &reader)
  {
  QStringList styleList = reader.parseList( param );
  if( styleList.count() == 0 )
    return false;
  QString presentationStyleAssignment = reader.value( styleList.at(0) );
  if( presentationStyleAssignment.startsWith( QStringLiteral("PRESENTATION_STYLE_ASSIGNMENT")) ) {
    styleList = reader.parseList( presentationStyleAssignment );
    if( styleList.count() == 1 ) {
      return stepSurfaceStyleUsage( reader.value( styleList.at(0) ), reader );
      }
    return true;
    }
  return false;
  }




bool Sd3dFace::stepSurfaceStyleUsage(const QString &param, const SdStepReader &reader)
  {
  if( param.startsWith( QStringLiteral("SURFACE_STYLE_USAGE")) ) {
    QStringList surfaceList = reader.parseList( param );
    if( surfaceList.count() == 2 ) {
      return stepSurfaceStyleSide( surfaceList.at(1), reader );
      }
    }
  return false;
  }



bool Sd3dFace::stepSurfaceStyleSide(const QString &param, const SdStepReader &reader)
  {
  if( param.startsWith( QStringLiteral("SURFACE_SIDE_STYLE")) ) {
    QStringList surfaceList = reader.parseList( param );
    if( surfaceList.count() == 2 ) {
      surfaceList = reader.parseList( );
      return stepSurfaceStyleSide( surfaceList.at(1), reader );
      }
    }
  return false;
  }




bool Sd3dFace::stepAdvancedFace(const QString &param, const SdStepReader &reader)
  {
  QString advancedFace = reader.value( param );
  if( advancedFace.startsWith( QStringLiteral("ADVANCED_FACE") ) ) {
    QStringList faceList = reader.parseList( advancedFace );
    qDebug() << "advancedFace" << faceList;
    }
  return true;
  }
