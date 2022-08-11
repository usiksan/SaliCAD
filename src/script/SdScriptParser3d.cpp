#include "SdScriptParser3d.h"

//Predefined variable to insert faces into part
#include "SdScriptPartModel.h"
//Predefined variable to insert 2d graphics into part
#include "SdScriptPartGraph.h"
#include "SdScriptValueVariableMatrix.h"
#include "SdScriptValueVariableFloat.h"

#include "SdScriptValueFunSelectColor.h"
#include "SdScriptValueFunSelectPad.h"

#include "SdScriptValueFunStringPadRectThrou.h"
#include "SdScriptValueFunStringPadCircleThrou.h"
#include "SdScriptValueFunStringPinIndex.h"
#include "SdScriptValueFunStringPinMatrix.h"

#include "SdScriptValueFunMatrix.h"
#include "SdScriptValueFunMatrixRotate.h"
#include "SdScriptValueFunMatrixScale.h"
#include "SdScriptValueFunMatrixTranslate.h"

#include "SdScriptValueFunAFloatArc.h"

#include "SdScriptValueFunVertexBuild.h"
#include "SdScriptValueFunVertexOffset.h"

#include "SdScriptValueFunFaceBevelXY.h"
#include "SdScriptValueFunFaceCircle.h"
#include "SdScriptValueFunFaceCircleSide.h"
#include "SdScriptValueFunFaceCurveXZ.h"
#include "SdScriptValueFunFaceDuplicate.h"
#include "SdScriptValueFunFaceDuplicateGrow.h"
#include "SdScriptValueFunFaceDuplicateScale.h"
#include "SdScriptValueFunFaceDuplicateShift.h"
#include "SdScriptValueFunFaceEllipse.h"
#include "SdScriptValueFunFaceEllipseSide.h"
#include "SdScriptValueFunFaceEquidistantXY.h"
#include "SdScriptValueFunFaceFlat.h"
#include "SdScriptValueFunFaceFlatMatrix.h"
#include "SdScriptValueFunFacePart.h"
#include "SdScriptValueFunFaceRect.h"
#include "SdScriptValueFunFaceRectRound.h"
#include "SdScriptValueFunFaceRectSide.h"

#include "SdScriptValueFunLFaceAppend.h"
#include "SdScriptValueFunLFaceBodyBeveled.h"
#include "SdScriptValueFunLFaceBox.h"
#include "SdScriptValueFunLFaceBoxMatrix.h"
#include "SdScriptValueFunLFaceCurveXZ.h"
#include "SdScriptValueFunLFaceCylinder.h"
#include "SdScriptValueFunLFaceCylinderMatrix.h"
#include "SdScriptValueFunLFaceDuplicate.h"
#include "SdScriptValueFunLFaceExtrude.h"
#include "SdScriptValueFunLFaceExtrudeShift.h"
#include "SdScriptValueFunLFaceHexagon.h"
#include "SdScriptValueFunLFaceHolesXY.h"
#include "SdScriptValueFunLFaceIndexed.h"
#include "SdScriptValueFunLFacePinCurveOne.h"
#include "SdScriptValueFunLFacePinCurveTwo.h"
#include "SdScriptValueFunLFaceRotation.h"
#include "SdScriptValueFunLFaceSimplifyXY.h"
#include "SdScriptValueFunLFaceUnion.h"
#include "SdScriptValueFunLFaceWall.h"
#include "SdScriptValueFunLFaceWallBevelXY.h"
#include "SdScriptValueFunLFaceWallDoubleBevelXY.h"
#include "SdScriptValueFunLFaceWallIndexed.h"
#include "SdScriptValueFunLFaceWallList.h"
#include "SdScriptValueFunLFaceWallRound.h"
#include "SdScriptValueFunLFaceWalls.h"



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
  addFunction( QStringLiteral("selectColor"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunSelectColor(); }, QStringLiteral("selectColor( string color ) Press F2 to select\nConvert textual color in format \"#rrggbb\"") );
  addFunction( QStringLiteral("selectPad"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunSelectPad(); }, QStringLiteral("selectColor( string padDescription ) Press F2 to master") );

  addFunction( QStringLiteral("stringPadRectThrough"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunStringPadRectThrou(); }, QStringLiteral("stringPadRectThrough( float width, float height, float holeDiamentr, float mask )") );
  addFunction( QStringLiteral("stringPadCircleThrough"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunStringPadCircleThrou(); }, QStringLiteral("stringPadCircleThrough( float padDiametr, float holeDiametr, float mask ) ") );
  addFunction( QStringLiteral("stringPinIndex"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunStringPinIndex(); }, QStringLiteral("stringPinIndex( float pinIndex )\nBuild pin number as index (14)") );
  addFunction( QStringLiteral("stringPinMatrix"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunStringPinMatrix(); }, QStringLiteral("stringPinMatrix( float row, float columnt )\nBuild pin number as matrix (D23) for bga") );

  addFunction( QStringLiteral("matrix"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunMatrix(); }, QStringLiteral("matrix( float angle, float rotateX, float rotateY, float rotateZ, float offX, float offY, float offZ )") );
  addFunction( QStringLiteral("matrixRotate"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunMatrixRotate(); }, QStringLiteral("matrixRotate( matrix src, float angle, float rotateX, float rotateY, float rotateZ )") );
  addFunction( QStringLiteral("matrixScale"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunMatrixScale(); }, QStringLiteral("matrixScale( matrix src, float scaleX, float scaleY, float scaleZ )") );
  addFunction( QStringLiteral("matrixTranslate"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunMatrixTranslate(); }, QStringLiteral("matrixTranslate( matrix src, float offX, float offY, float offZ )") );

  addFunction( QStringLiteral("arc"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunAFloatArc(); }, QStringLiteral("arc( float radius, float angleStart, float angleStop, float sideCount )") );

  addFunction( QStringLiteral("vertex"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunVertexBuild(model); }, QStringLiteral("vertex( float x, float y, float z )") );
  addFunction( QStringLiteral("vertexOffset"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunVertexOffset(model); }, QStringLiteral("vertexOffset( vertex src, float offX, float offY, float OffZ )") );
//  addFunction( QStringLiteral("vertexTranslate"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunVertexTranslate(); } );
//  addFunction( QStringLiteral("vertexCenterOfRegion"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunVertexCenterOfRegion(); } );

  addFunction( QStringLiteral("faceBevelXY"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceBevelXY(model); }, QStringLiteral("faceBevelXY( face src, float radius )") );
  addFunction( QStringLiteral("faceCircle"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceCircle(model); }, QStringLiteral("faceCircle( float radius, float stepDegree, matrix transfer )") );
  addFunction( QStringLiteral("faceCircleSide"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceCircleSide(model); }, QStringLiteral("faceCircleSide( float radius, float sideCount, matrix transfer )") );
  addFunction( QStringLiteral("faceCurveXZ"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceCurveXZ(model); }, QStringLiteral("faceCurveXZ( face src, float radius, float angleSrc, float angleDst)") );
  addFunction( QStringLiteral("faceDuplicate"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceDuplicate(model); }, QStringLiteral("faceDuplicate( face src, matrix transfer )") );
  addFunction( QStringLiteral("faceDuplicateGrow"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceDuplicateGrow(model); }, QStringLiteral("faceDuplicateGrow( face src, float growx, float growy, matrix transfer )") );
  addFunction( QStringLiteral("faceDuplicateScale"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceDuplicateScale(model); }, QStringLiteral("faceDuplicateScale( face src, float scalex, float scaley, float radius )") );
  addFunction( QStringLiteral("faceDuplicateShift"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceDuplicateShift(model); }, QStringLiteral("faceDuplicateShift( face src, float normalOffset )") );
  addFunction( QStringLiteral("faceEllipse"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceEllipse(model); }, QStringLiteral("faceEllipse( float radiusx, float radiusy, float stepDegree, matrix transfer )") );
  addFunction( QStringLiteral("faceEllipseSide"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceEllipseSide(model); }, QStringLiteral("faceEllipse( float radiusx, float radiusy, float sideCount, matrix transfer )") );
  addFunction( QStringLiteral("faceEquidistantXY"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceEquidistantXY(model); }, QStringLiteral("faceEquidistantXY( face src, float distance, matrix transfer )") );
  addFunction( QStringLiteral("faceFlat"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceFlat(model); }, QStringLiteral("faceFlat( vertex firstPoint, float orientation, float offsetPairs[] ) ") );
  addFunction( QStringLiteral("faceFlatMatrix"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceFlatMatrix(model); }, QStringLiteral("faceFlatMatrix( float offsetPairs[], matrix transfer ) ") );
  addFunction( QStringLiteral("facePart"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunFacePart(); }, QStringLiteral("facePart( face src, float indexList[] )") );
  addFunction( QStringLiteral("faceRect"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceRect(model); }, QStringLiteral("faceRect( float lenght, float width, matrix transfer )") );
  addFunction( QStringLiteral("faceRectRound"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceRectRound(model); }, QStringLiteral("faceRectRound( float lenght, float width, float radius, float stepDegree, matrix transfer )") );
  addFunction( QStringLiteral("faceRectSide"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceRectSide(model); }, QStringLiteral("faceRectSide( float lenght, float width, float sideCount, matrix transfer )") );

  addFunction( QStringLiteral("faceListAppend"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceAppend(); }, QStringLiteral("faceListAppend( faceList src1, faceList src2 )") );
  addFunction( QStringLiteral("faceListBodyBeveled"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceBodyBeveled(model); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceListBox"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceBox(model); }, QStringLiteral("faceListBox( float lenght, float width, float height )") );
  addFunction( QStringLiteral("faceListBoxMatrix"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceBoxMatrix(model); }, QStringLiteral("faceListBoxMatrix( float lenght, float width, float height, matrix transfer )") );
  addFunction( QStringLiteral("faceListCurveXZ"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceCurveXZ(model); }, QStringLiteral("faceListCurveXY( face src, float radius, float angleSrc, float angleDst)") );
  addFunction( QStringLiteral("faceListCylinder"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceCylinder(model); }, QStringLiteral("faceListCylinder( float radius, float height )") );
  addFunction( QStringLiteral("faceListCylinderMatrix"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceCylinderMatrix(model); }, QStringLiteral("faceListCylinderMatrix( float radius, float height, matrix transfer )") );
  addFunction( QStringLiteral("faceListDuplicate"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceDuplicate(model); }, QStringLiteral("faceListDuplicate( faceList src, matrix transfer )") );
  addFunction( QStringLiteral("faceListExtrude"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceExtrude(model); }, QStringLiteral("faceListExtrude( face src, matrix transfer )") );
  addFunction( QStringLiteral("faceListExtrudeShift"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceExtrudeShift(model); }, QStringLiteral("faceListExtrude( face src, float normalHeight )") );
  addFunction( QStringLiteral("faceListHexagon"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceHexagon(model); }, QStringLiteral("faceListHexagon( float lenght, float topLenght, float height, float width )") );
  addFunction( QStringLiteral("faceListHolesXY"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceHolesXY(model); }, QStringLiteral("faceListHoles( face src, faceList holeList )") );
  addFunction( QStringLiteral("faceListIndexed"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceIndexed(); }, QStringLiteral("faceListIndexed( faceList src, float indexes[] )") );
  addFunction( QStringLiteral("faceListPinCurveOne"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFacePinCurveOne(model); }, QStringLiteral("faceListPinCurveOne( face src, float firstLen, float middleLen, float radius, float angle, float sideCount )") );
  addFunction( QStringLiteral("faceListPinCurveTwo"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFacePinCurveTwo(model); }, QStringLiteral("faceListPinCurveTwo( face src, float firstLen, float middleLen, float lastLen, float radius, float angleFirst, float angleSecond, float sideCount )") );
  addFunction( QStringLiteral("faceListRotation"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceRotation(model); }, QStringLiteral("faceListRotation( float angleStart, float angleStop, float sideCount, matrix transfer, floatArray pathPairs )") );
  addFunction( QStringLiteral("faceListSimplify"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceSimplifyXY(model); }, QStringLiteral("faceListSimplify( face src )") );
  addFunction( QStringLiteral("faceListUnion"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceAppend(); }, QStringLiteral("faceListUnion( faceList src1, faceList src2 )") );
  addFunction( QStringLiteral("faceListWall"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceWall(); }, QStringLiteral("faceListWall( face src, face dst, bool close )") );
  addFunction( QStringLiteral("faceListWallBevelXY"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceWallBevelXY(model); }, QStringLiteral("faceListWallBevelXY( face bot, face top, float radius, float stepDegree )") );
  addFunction( QStringLiteral("faceListWallDoubleBevelXY"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceWallDoubleBevelXY(model); }, QStringLiteral("faceListWallBevelXY( face bot, face top, float radiusBot, float radiusTop, float stepDegreeBot, float stepDegreeTop, float height )") );
  addFunction( QStringLiteral("faceListWallIndexed"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceWallIndexed(); }, QStringLiteral("faceListWallIndexed( face src, face dst, float indexes[] )") );
  addFunction( QStringLiteral("faceListWallList"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceWallList(); }, QStringLiteral("faceListWallList( faceList botFaceList, faceList topFaceList, bool close )") );
  addFunction( QStringLiteral("faceListWallRound"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceWallRound(model); }, QStringLiteral("faceListWallRound( face bot, face top, float scaleX, float scaleY, float radius, float stepDegree )") );
  addFunction( QStringLiteral("faceListWalls"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceWalls(); }, QStringLiteral("faceListWalls( faceList layers, bool close )") );
  addFunction( QStringLiteral("faceListPinTqfp"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunModelPinTqfp(model); }, QStringLiteral("") );

  addFunction( QStringLiteral("model"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunModelBuild(); }, QStringLiteral("model( color ambient, color diffuse, color specular, faceList faces[], matrix transfer )") );
  addFunction( QStringLiteral("modelAppend"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunModelAppend(); }, QStringLiteral("modelAppend( model src, color ambient, color diffuse, color specular, faceList faces[] )") );
  addFunction( QStringLiteral("modelCopy"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunModelCopy(); }, QStringLiteral("modelCopy( model src, matrix trasfer )") );

  addFunction( QStringLiteral("graphLine"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunGraphLine(); }, QStringLiteral("graphLine( float [startX,startY], float [stopX,xtopY] )") );
  addFunction( QStringLiteral("graphCircle"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunGraphCircle(); }, QStringLiteral("graphCircle( float [centerX, centerY, radius] )") );
  addFunction( QStringLiteral("graphRect"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunGraphRect(); }, QStringLiteral("graphRect( float [corner1X,corner1Y], float[corner2X,corner2Y] )") );
  addFunction( QStringLiteral("graphRectFilled"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunGraphRectFilled(); }, QStringLiteral("graphRectFilled( float [corner1X,corner1Y], float[corner2X,corner2Y] )") );
  addFunction( QStringLiteral("graphPin"), [] () -> SdScriptValueFunction* { return new SdM3dFunGraphPin(); }, QStringLiteral("graphPin( float [pinPosX,pinPosY], string pad, float [numberPosX,numberPosY], string number, string numberAttr, float [namePosX,namePosY], string nameAttr )") );
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
