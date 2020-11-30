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
  //#232 = DIRECTION( '', ( 0.000000000000000, -1.00000000000000, 0.000000000000000 ) );
  SdStepParserDirection vectorDirection;

  //#198 = VECTOR( '', #232, 1000.00000000000 );
  SdStepParserParamSkeeper vectorParam0;
  SdStepParserAsParamSingleId vectorParam1( {&vectorDirection} );
  SdStepParserAsParamDouble vectorParam2;
  SdStepParserParamList vector( QStringLiteral("VECTOR"), {&vectorParam0, &vectorParam1, &vectorParam2 } );

  //#197 = CARTESIAN_POINT( '', ( 0.000000000000000, 30.0000000000000, 10.0000000000000 ) );
  SdStepParserCartesianPoint lineCartesianPoint;

  //#171 = LINE( '', #197, #198 );
  SdStepParserParamSkeeper lineParam0;
  SdStepParserAsParamSingleId lineParam1( {&lineCartesianPoint} );
  SdStepParserAsParamSingleId lineParam2( {&vector} );
  SdStepParserParamList line( QStringLiteral("LINE"), {&lineParam0, &lineParam1, &lineParam2 } );

  //#196 = CARTESIAN_POINT( '', ( 0.000000000000000, 0.000000000000000, 10.0000000000000 ) );
  SdStepParserCartesianPoint vertex1CartesianPoint;

  //#170 = VERTEX_POINT( '', #196 );
  SdStepParserParamSkeeper vertex1PointParam0;
  SdStepParserAsParamSingleId vertex1PointParam1( {&vertex1CartesianPoint} );
  SdStepParserParamList vertex1Point( QStringLiteral("VERTEX_POINT"), {&vertex1PointParam0, &vertex1PointParam1 } );

  //#195 = CARTESIAN_POINT( '', ( 0.000000000000000, 30.0000000000000, 10.0000000000000 ) );
  SdStepParserCartesianPoint vertex0CartesianPoint;

  //#169 = VERTEX_POINT( '', #195 );
  SdStepParserParamSkeeper vertex0PointParam0;
  SdStepParserAsParamSingleId vertex0PointParam1( {&vertex0CartesianPoint} );
  SdStepParserParamList vertex0Point( QStringLiteral("VERTEX_POINT"), {&vertex0PointParam0, &vertex0PointParam1 } );

  //#151 = EDGE_CURVE( '', #169, #170, #171, .T. );
  SdStepParserParamSkeeper edgeCurveParam0;
  SdStepParserAsParamSingleId edgeCurveParam1( {&vertex0Point} );
  SdStepParserAsParamSingleId edgeCurveParam2( {&vertex1Point} );
  SdStepParserAsParamSingleId edgeCurveParam3( {&line} );
  SdStepParserAsParamString edgeCurveParam4( {QStringLiteral(".T."), QStringLiteral(".F.") } );
  SdStepParserParamList edgeCurve( QStringLiteral("EDGE_CURVE"), {&edgeCurveParam0, &edgeCurveParam1, &edgeCurveParam2, &edgeCurveParam3, &edgeCurveParam4 } );

  //#103 = ORIENTED_EDGE( '', *, *, #151, .T. );
  SdStepParserParamSkeeper orientedEdgeParam0;
  SdStepParserParamSkeeper orientedEdgeParam1;
  SdStepParserParamSkeeper orientedEdgeParam2;
  SdStepParserAsParamSingleId orientedEdgeParam3( {&edgeCurve} );
  SdStepParserAsParamString orientedEdgeParam4( {QStringLiteral(".T."), QStringLiteral(".F.") } );
  SdStepParserParamList orientedEdge( QStringLiteral("ORIENTED_EDGE"), {&orientedEdgeParam0, &orientedEdgeParam1, &orientedEdgeParam2, &orientedEdgeParam3, &orientedEdgeParam4 }, [this, &vertex0CartesianPoint, &vertex1CartesianPoint] () {
    //Oriented edge parsed. Append vertex's to bound list
    if( mRegion.count() == 0 ) {
      //It's first edge. Append both vertex's
      mRegion.append( vertex0CartesianPoint.to3dPoint() );
      }
    if( mRegion.last() == vertex0CartesianPoint.to3dPoint() && !(mRegion.first() == vertex1CartesianPoint.to3dPoint()) )
      mRegion.append( vertex1CartesianPoint.to3dPoint() );
    } );

  //#84 = EDGE_LOOP( '', ( #103, #104, #105, #106 ) );
  SdStepParserParamSkeeper edgeLoopParam0;
  SdStepParserAsParamIdList edgeLoopParam1( {&orientedEdge} );
  SdStepParserParamList edgeLoop( QStringLiteral("EDGE_LOOP"), {&edgeLoopParam0, &edgeLoopParam1 } );

  //#59 = FACE_OUTER_BOUND( '', #84, .T. );
  SdStepParserParamSkeeper faceOuterBoundParam0;
  SdStepParserAsParamSingleId faceOuterBoundParam1( {&edgeLoop} );
  SdStepParserAsParamString faceOuterBoundParam2( {QStringLiteral(".T."), QStringLiteral(".F.") } );
  SdStepParserParamList faceOuterBound( QStringLiteral("FACE_OUTER_BOUND"), {&faceOuterBoundParam0, &faceOuterBoundParam1, &faceOuterBoundParam2 } );

  SdStepParserCartesianPoint cartesianPoint;
  SdStepParserDirection direction1;
  SdStepParserDirection direction2;

  //#85 = AXIS2_PLACEMENT_3D( '', #107, #108, #109 );
  SdStepParserParamSkeeper axis2Placement3dParam0;
  SdStepParserAsParamSingleId axis2Placement3dParam1( {&cartesianPoint} );
  SdStepParserAsParamSingleId axis2Placement3dParam2( {&direction1} );
  SdStepParserAsParamSingleId axis2Placement3dParam3( {&direction2} );
  SdStepParserParamList axis2Placement3d( QStringLiteral("AXIS2_PLACEMENT_3D"), {&axis2Placement3dParam0, &axis2Placement3dParam1, &axis2Placement3dParam2, &axis2Placement3dParam3 }, [this, &direction1] () {
    mNormal = direction1.to3dPoint();
    } );

  //#60 = PLANE( '', #85 );
  SdStepParserParamSkeeper planeParam0;
  SdStepParserAsParamSingleId planeParam1( {&axis2Placement3d} );
  SdStepParserParamList plane( QStringLiteral("PLANE"), {&planeParam0, &planeParam1 } );

  //#30 = ADVANCED_FACE( '', ( #59 ), #60, .T. );
  SdStepParserParamSkeeper advancedFaceParam0;
  SdStepParserAsParamIdList advancedFaceParam1( {&faceOuterBound} );
  SdStepParserAsParamSingleId advancedFaceParam2( {&plane} );
  SdStepParserAsParamString advancedFaceParam3( {QStringLiteral(".T."), QStringLiteral(".F.") } );
  SdStepParserParamList advancedFace( QStringLiteral("ADVANCED_FACE"), {&advancedFaceParam0, &advancedFaceParam1, &advancedFaceParam2, &advancedFaceParam3 } );




  //#194 = COLOUR_RGB( '', 0.898039221763611, 0.000000000000000, 0.000000000000000 );
  SdStepParserColorRgb color;

  //#168 = FILL_AREA_STYLE_COLOUR( '', #194 );
  SdStepParserParamSkeeper fillAreaStyleColourParam0;
  SdStepParserAsParamSingleId fillAreaStyleColourParam1( {&color} );
  SdStepParserParamList fillAreaStyleColour( QStringLiteral("FILL_AREA_STYLE_COLOUR"), {&fillAreaStyleColourParam0, &fillAreaStyleColourParam1}, [this, &color] () {
    mFaceColor = color.mColor;
    } );

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
  SdStepParserAsParamSingleId styledItemParam2( {&advancedFace} );
  SdStepParserParamList styledItem( QStringLiteral("STYLED_ITEM"), { &styledItemParam0, &styledItemParam1, &styledItemParam2 } );

  QString str = reader.value(faceId);
  bool res = styledItem.parse( str, reader );
  if( res ) {
    qDebug() << "STEP face parsed" << mRegion.count();
    }
  return res;
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




