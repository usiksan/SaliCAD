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
  //#194 = COLOUR_RGB( '', 0.898039221763611, 0.000000000000000, 0.000000000000000 );
  SdStepParserColorRgb color;

  //#168 = FILL_AREA_STYLE_COLOUR( '', #194 );
  SdStepParserParamSkeeper fillAreaStyleColourParam0;
  SdStepParserAsParamSingleId fillAreaStyleColourParam1( {&color} );
  SdStepParserParamList fillAreaStyleColour( QStringLiteral("FILL_AREA_STYLE_COLOUR"), {&fillAreaStyleColourParam0, &fillAreaStyleColourParam1} );

  //#150 = FILL_AREA_STYLE( '', ( #168 ) );
  SdStepParserParamSkeeper fillAreaStyleParam0;
  SdStepParserAsParamIdList fillAreaStyleParam1( {&fillAreaStyleColour} );
  SdStepParserParamList fillAreaStyle( QStringLiteral("FILL_AREA_STYLE"), {&fillAreaStyleParam0, &fillAreaStyleParam1} );

  //102 = SURFACE_STYLE_FILL_AREA( #150 );
  SdStepParserAsParamSingleId surfaceStyleFillAreaParam0( {&fillAreaStyle} );
  SdStepParserParamList surfaceStyleFillArea( QStringLiteral("SURFACE_STYLE_FILL_AREA"), {&surfaceStyleFillAreaParam0} );

  //83 = SURFACE_SIDE_STYLE( '', ( #102 ) );
  SdStepParserParamSkeeper surfaceSideStyleParam0;
  SdStepParserAsParamIdList surfaceSideStyleParam1( {&surfaceStyleFillArea} );
  SdStepParserParamList surfaceSideStyle( QStringLiteral("SURFACE_SIDE_STYLE"), {&surfaceSideStyleParam0, &surfaceSideStyleParam1} );

  //58 = SURFACE_STYLE_USAGE( .BOTH., #83 );
  SdStepParserAsParamString surfaceStyleUsageParam0( {QStringLiteral(".BOTH.") } );
  SdStepParserAsParamSingleId surfaceStyleUsageParam1( {&surfaceSideStyle} );
  SdStepParserParamList surfaceStyleUsage( QStringLiteral("SURFACE_STYLE_USAGE"), { &surfaceStyleUsageParam0, &surfaceStyleUsageParam1 } );

  //29 = PRESENTATION_STYLE_ASSIGNMENT( ( #58 ) );
  SdStepParserAsParamIdList presentationStyleAssignmentParam0( {&surfaceStyleUsage} );
  SdStepParserParamList presentationStyleAssignment( QStringLiteral("PRESENTATION_STYLE_ASSIGNMENT"), {&presentationStyleAssignmentParam0} );

  //STYLED_ITEM( '', ( #29 ), #30 )
  SdStepParserParamSkeeper styledItemParam0;
  SdStepParserAsParamIdList styledItemParam1( {&presentationStyleAssignment} );
  SdStepParserAsParamSingleId styledItemParam2( {} );
  SdStepParserParamList styledItem( QStringLiteral("STYLED_ITEM"), { &styledItemParam0, &styledItemParam1, &styledItemParam2 } );

  QString line = reader.value(faceId);
  return styledItem.parse( line, reader );
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




