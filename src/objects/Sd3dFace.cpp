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
  return stepParseParams( reader.value(faceId), reader, { &Sd3dFace::stepParamSkeep, &Sd3dFace::stepPresentationStyleAssignmentList, &Sd3dFace::stepAdvancedFace });
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




bool Sd3dFace::stepParseParams(const QString &param, const SdStepReader &reader, QList<Sd3dFace::Sd3dFaceMemberStep> paramHandlers)
  {
  //Parse param to list of params
  QStringList paramList = reader.parseList( param );

  //If params count not equals handler count then return false
  if( paramList.count() == paramHandlers.count() )
    return false;

  //For each params handle it with corresponding handler function
  for( int i = 0; i < paramList.count(); ++i )
    if( !(this->*paramHandlers.at(i))( paramList.at(i), reader ) )
      return false;

  //All params handled successfully
  return true;
  }





bool Sd3dFace::stepParseParamList(const QString &param, const SdStepReader &reader, Sd3dFace::Sd3dFaceMemberStep paramHandler)
  {
  //Parse param to list of params
  QStringList paramList = reader.parseList( param );

  //If no params then return false
  if( paramList.count() == 0 )
    return false;

  //For each params handle it with handler function
  for( auto const &p : paramList )
    if( !(this->*paramHandler)( p, reader ) )
      return false;

  //All params handled successfully
  return true;
  }





bool Sd3dFace::stepParseParamIdList(const QString &param, const SdStepReader &reader, Sd3dFace::Sd3dFaceMemberStep paramHandler)
  {
  //Parse param to list of param id's
  QStringList paramList = reader.parseList( param );

  //If no id's then return false
  if( paramList.count() == 0 )
    return false;

  //For each id's handle it with handler function
  for( auto const &id : paramList )
    if( !(this->*paramHandler)( reader.value( id ), reader ) )
      return false;

  //All id's handled successfully
  return true;
  }




bool Sd3dFace::stepParamSkeep(const QString &param, const SdStepReader &reader)
  {
  Q_UNUSED(param)
  Q_UNUSED(reader)
  return true;
  }

bool Sd3dFace::stepPresentationStyleAssignmentList(const QString &param, const SdStepReader &reader)
  {
  return stepParseParamIdList( param, reader, &Sd3dFace::stepPresentationStyleAssignment );
  }




bool Sd3dFace::stepPresentationStyleAssignment(const QString &param, const SdStepReader &reader)
  {
  if( param.startsWith( QStringLiteral("PRESENTATION_STYLE_ASSIGNMENT")) ) {
    //PRESENTATION_STYLE_ASSIGNMENT contains single param with list of presentation styles
    return stepParseParamList( param, reader, &Sd3dFace::stepSurfaceStyleUsageList );
    }
  qDebug() << "step parse error. None PRESENTATION_STYLE_ASSIGNMENT";
  return false;
  }


bool Sd3dFace::stepSurfaceStyleUsageList(const QString &param, const SdStepReader &reader)
  {
  return stepParseParamIdList( param, reader, &Sd3dFace::stepSurfaceStyleUsage );
  }




bool Sd3dFace::stepSurfaceStyleUsage(const QString &param, const SdStepReader &reader)
  {
  if( param.startsWith( QStringLiteral("SURFACE_STYLE_USAGE")) ) {
    QStringList surfaceList = reader.parseList( param );
    if( surfaceList.count() == 2 ) {
      if( surfaceList.at(0) == QStringLiteral(".BOTH.") )
        return stepSurfaceSideStyle( surfaceList.at(1), reader );
      return true;
      }
    }
  return false;
  }



bool Sd3dFace::stepSurfaceSideStyle(const QString &param, const SdStepReader &reader)
  {
  if( param.startsWith( QStringLiteral("SURFACE_SIDE_STYLE")) ) {
    return stepParseParams( param, reader, { &Sd3dFace::stepParamSkeep, &Sd3dFace::stepSurfaceStyleFillAreaList } );
    }
  return false;
  }




bool Sd3dFace::stepSurfaceStyleFillAreaList(const QString &param, const SdStepReader &reader)
  {
  return stepParseParamIdList( param, reader, &Sd3dFace::stepSurfaceStyleFillArea );
  }



bool Sd3dFace::stepSurfaceStyleFillArea(const QString &param, const SdStepReader &reader)
  {
  if( param.startsWith( QStringLiteral("SURFACE_STYLE_FILL_AREA")) ) {
    return stepParseParamList( param, reader, &Sd3dFace::stepFillAreaStyle );
    }
  return false;
  }



bool Sd3dFace::stepFillAreaStyle(const QString &param, const SdStepReader &reader)
  {
  if( param.startsWith( QStringLiteral("FILL_AREA_STYLE")) ) {
    return stepParseParams( param, reader, { &Sd3dFace::stepParamSkeep, &Sd3dFace::stepFillAreaStyleColorList } );
    }
  return false;
  }

bool Sd3dFace::stepFillAreaStyleColorList(const QString &param, const SdStepReader &reader)
  {
  return stepParseParamIdList( param, reader, &Sd3dFace::stepFillAreaStyleColor );
  }




bool Sd3dFace::stepFillAreaStyleColor(const QString &param, const SdStepReader &reader)
  {
  if( param.startsWith( QStringLiteral("FILL_AREA_STYLE")) ) {
    return stepParseParams( param, reader, { &Sd3dFace::stepParamSkeep, &Sd3dFace::stepFillAreaStyleColorList } );
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
