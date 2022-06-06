#include "SdScriptParser3d.h"

//Predefined variable to insert faces into part
#include "SdScriptPartModel.h"
//Predefined variable to insert 2d graphics into part
#include "SdScriptPartGraph.h"
#include "SdScriptValueVariableMatrix.h"
#include "SdScriptValueVariableFloat.h"


#include "SdScriptValueFunStringPadRectThrou.h"
#include "SdScriptValueFunStringPadCircleThrou.h"
#include "SdScriptValueFunStringPinIndex.h"
#include "SdScriptValueFunStringPinMatrix.h"

#include "SdScriptValueFunMatrix.h"
#include "SdScriptValueFunMatrixRotate.h"
#include "SdScriptValueFunMatrixScale.h"
#include "SdScriptValueFunMatrixTranslate.h"


#include "SdScriptValueFunVertexBuild.h"
#include "SdScriptValueFunVertexOffset.h"

#include "SdScriptValueFunFaceRect.h"
#include "SdScriptValueFunFaceCircle.h"
#include "SdScriptValueFunFacePart.h"
#include "SdScriptValueFunFaceFlat.h"
#include "SdScriptValueFunFaceDuplicate.h"
#include "SdScriptValueFunFaceDuplicateShift.h"
#include "SdScriptValueFunFaceCurveXZ.h"

#include "SdScriptValueFunLFaceIndexed.h"
#include "SdScriptValueFunLFaceWall.h"
#include "SdScriptValueFunLFaceWallIndexed.h"
#include "SdScriptValueFunLFaceAppend.h"
#include "SdScriptValueFunLFaceUnion.h"
#include "SdScriptValueFunLFaceBox.h"
#include "SdScriptValueFunLFaceCylinder.h"
#include "SdScriptValueFunLFaceHexagon.h"
#include "SdScriptValueFunLFaceBodyBeveled.h"
#include "SdScriptValueFunLFaceCurveXZ.h"
#include "SdScriptValueFunLFaceExtrude.h"
#include "SdScriptValueFunLFaceExtrudeShift.h"
#include "SdScriptValueFunLFacePinCurveOne.h"
#include "SdScriptValueFunLFacePinCurveTwo.h"

#include "SdScriptValueFunModelAppend.h"
#include "SdScriptValueFunModelCopy.h"
#include "SdScriptValueFunModelBuild.h"
#include "SdScriptValueFunModelPinTqfp.h"

#include "SdScriptValueFunGraphLine.h"
#include "SdScriptValueFunGraphCircle.h"
#include "SdScriptValueFunGraphRect.h"
#include "SdScriptValueFunGraphRectFilled.h"
#include "SdScriptValueFunGraphPin.h"


SdScriptParser3d::SdScriptParser3d(QTableWidget *tableWidget, Sd3dModel *model ) :
  SdScriptParser(tableWidget)
  {
  addFunction( QStringLiteral("stringPadRectThrough"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunStringPadRectThrou(); }, QStringLiteral("stringPadRectThrough( float width, float height, float holeDiamentr, float mask )") );
  addFunction( QStringLiteral("stringPadCircleThrough"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunStringPadCircleThrou(); }, QStringLiteral("stringPadCircleThrough( float padDiametr, float holeDiametr, float mask ) ") );
  addFunction( QStringLiteral("stringPinIndex"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunStringPinIndex(); }, QStringLiteral("stringPinIndex( float pinIndex )\nBuild pin number as index (14)") );
  addFunction( QStringLiteral("stringPinMatrix"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunStringPinMatrix(); }, QStringLiteral("stringPinMatrix( float row, float columnt )\nBuild pin number as matrix (D23) for bga") );

  addFunction( QStringLiteral("matrix"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunMatrix(); }, QStringLiteral("matrix( float angle, float rotateX, float rotateY, float rotateZ, float offX, float offY, float offZ )") );
  addFunction( QStringLiteral("matrixRotate"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunMatrixRotate(); }, QStringLiteral("matrix( matrix src, float angle, float rotateX, float rotateY, float rotateZ )") );
  addFunction( QStringLiteral("matrixScale"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunMatrixScale(); }, QStringLiteral("matrix( matrix src, float scaleX, float scaleY, float scaleZ )") );
  addFunction( QStringLiteral("matrixTranslate"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunMatrixTranslate(); }, QStringLiteral("matrix( matrix src, float offX, float offY, float offZ )") );

  addFunction( QStringLiteral("vertex"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunVertexBuild(model); }, QStringLiteral("vertex( float x, float y, float z )") );
  addFunction( QStringLiteral("vertexOffset"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunVertexOffset(model); }, QStringLiteral("vertexOffset( vertex src, float offX, float offY, float OffZ )") );
//  addFunction( QStringLiteral("vertexTranslate"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunVertexTranslate(); } );
//  addFunction( QStringLiteral("vertexCenterOfRegion"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunVertexCenterOfRegion(); } );

  addFunction( QStringLiteral("faceRect"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceRect(model); }, QStringLiteral("faceRect( float lenght, float width, matrix transfer )") );
  addFunction( QStringLiteral("faceCircle"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceCircle(model); }, QStringLiteral("faceCircle( float radius, float stepDegree, matrix transfer )") );
  addFunction( QStringLiteral("facePart"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunFacePart(); }, QStringLiteral("facePart( face src, float indexList[] )") );
  addFunction( QStringLiteral("faceFlat"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceFlat(model); }, QStringLiteral("faceFlat( vertex firstPoint, float orientation, float offsetPairs[] ) ") );
  addFunction( QStringLiteral("faceDuplicate"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceDuplicate(model); }, QStringLiteral("faceDuplicate( face src, matrix transfer )") );
  addFunction( QStringLiteral("faceDuplicateShift"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceDuplicateShift(model); }, QStringLiteral("faceDuplicateShift( face src, float normalOffset )") );
  addFunction( QStringLiteral("faceCurveXY"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceCurveXZ(model); }, QStringLiteral("faceCurveXY( face src, float radius, float angleSrc, float angleDst)") );
  //addFunction( QStringLiteral("faceTriangle"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceTriangle(); } );

  addFunction( QStringLiteral("faceListExtrude"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceExtrude(model); }, QStringLiteral("faceListExtrude( face src, matrix transfer )") );
  addFunction( QStringLiteral("faceListExtrudeShift"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceExtrudeShift(model); }, QStringLiteral("faceListExtrude( face src, float normalHeight )") );
  addFunction( QStringLiteral("faceListIndexed"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceIndexed(); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceListWall"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceWall(); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceListWallIndexed"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceWallIndexed(); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceListAppend"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceAppend(); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceListUnion"), [] () -> SdScriptValueFunction* { return new SdM3dFunLFaceUnion(); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceListBox"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceBox(model); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceListCylinder"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceCylinder(model); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceListCurveXZ"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceCurveXZ(model); }, QStringLiteral("faceListCurveXY( face src, float radius, float angleSrc, float angleDst)") );
  addFunction( QStringLiteral("faceListPinCurveOne"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFacePinCurveOne(model); }, QStringLiteral("faceListPinCurveOne( face src, float firstLen, float middleLen, float radius, float angle, float sideCount )") );
  addFunction( QStringLiteral("faceListPinCurveTwo"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFacePinCurveTwo(model); }, QStringLiteral("faceListPinCurveTwo( face src, float firstLen, float middleLen, float lastLen, float radius, float angleFirst, float angleSecond, float sideCount )") );
  addFunction( QStringLiteral("faceListHexagon"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceHexagon(model); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceListBodyBeveled"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceBodyBeveled(model); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceListPinTqfp"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunModelPinTqfp(model); }, QStringLiteral("") );

  addFunction( QStringLiteral("model"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunModelBuild(); }, QStringLiteral("model( color ambient, color diffuse, color specular, faceList faces[], matrix transfer )") );
  addFunction( QStringLiteral("modelAppend"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunModelAppend(); }, QStringLiteral("modelAppend( model src, color ambient, color diffuse, color specular, faceList faces[] )") );
  addFunction( QStringLiteral("modelCopy"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunModelCopy(); }, QStringLiteral("modelCopy( model src, matrix trasfer )") );

  addFunction( QStringLiteral("graphLine"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunGraphLine(); }, QStringLiteral("") );
  addFunction( QStringLiteral("graphCircle"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunGraphCircle(); }, QStringLiteral("") );
  addFunction( QStringLiteral("graphRect"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunGraphRect(); }, QStringLiteral("") );
  addFunction( QStringLiteral("graphRectFilled"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunGraphRectFilled(); }, QStringLiteral("") );
  addFunction( QStringLiteral("graphPin"), [] () -> SdScriptValueFunction* { return new SdM3dFunGraphPin(); }, QStringLiteral("") );
  }




SdScriptProgrammPtr SdScriptParser3d::parse3d(const QString src, SdPItemPart *part, Sd3dModel *model)
  {
  //Insert predefined variables
  mVariables.insert( QStringLiteral("partModel"), new SdScriptPartModel( model ) );
  mVariables.insert( QStringLiteral("partFlat"), new SdScriptPartGraph( part ) );
  mVariables.insert( QStringLiteral("matrix1"), new SdScriptValueVariableMatrix() );
  mVariables.insert( QStringLiteral("orientationXY"), new SdScriptValueVariableFloat(0.0) );
  mVariables.insert( QStringLiteral("orientationXZ"), new SdScriptValueVariableFloat(1.0) );
  mVariables.insert( QStringLiteral("orientationYZ"), new SdScriptValueVariableFloat(2.0) );

  return parse( src );
  }
