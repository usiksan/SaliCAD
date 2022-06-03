#include "SdScriptParser3d.h"

//Predefined variable to insert faces into part
#include "SdScriptPartModel.h"
//Predefined variable to insert 2d graphics into part
#include "SdScriptPartGraph.h"
#include "SdScriptValueVariableMatrix.h"


#include "SdScriptValueFunStringPadRectThrou.h"
#include "SdScriptValueFunStringPadCircleThrou.h"
#include "SdScriptValueFunStringPinIndex.h"
#include "SdScriptValueFunStringPinMatrix.h"

#include "SdScriptValueFunMatrix.h"

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
  addFunction( QStringLiteral("stringPadRectThrough"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunStringPadRectThrou(); }, QStringLiteral("stringPadRectThrough( float width, float height, float diamentr, float mask )") );
  addFunction( QStringLiteral("stringPadCircleThrough"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunStringPadCircleThrou(); }, QStringLiteral("") );
  addFunction( QStringLiteral("stringPinIndex"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunStringPinIndex(); }, QStringLiteral("") );
  addFunction( QStringLiteral("stringPinMatrix"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunStringPinMatrix(); }, QStringLiteral("") );

  addFunction( QStringLiteral("matrix"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunMatrix(); }, QStringLiteral("") );

  addFunction( QStringLiteral("vertex"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunVertexBuild(model); }, QStringLiteral("") );
  addFunction( QStringLiteral("vertexOffset"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunVertexOffset(model); }, QStringLiteral("") );
//  addFunction( QStringLiteral("vertexTranslate"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunVertexTranslate(); } );
//  addFunction( QStringLiteral("vertexCenterOfRegion"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunVertexCenterOfRegion(); } );

  addFunction( QStringLiteral("faceRect"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceRect(model); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceCircle"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceCircle(model); }, QStringLiteral("") );
  addFunction( QStringLiteral("facePart"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunFacePart(); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceFlat"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceFlat(model); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceDuplicate"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceDuplicate(model); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceDuplicateShift"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceDuplicateShift(model); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceCurveXY"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceCurveXZ(model); }, QStringLiteral("") );
  //addFunction( QStringLiteral("faceTriangle"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceTriangle(); } );

  addFunction( QStringLiteral("faceListIndexed"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceIndexed(); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceListWall"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceWall(); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceListWallIndexed"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceWallIndexed(); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceListAppend"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceAppend(); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceListUnion"), [] () -> SdScriptValueFunction* { return new SdM3dFunLFaceUnion(); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceListBox"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceBox(model); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceListCylinder"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceCylinder(model); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceListCurveXZ"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceCurveXZ(model); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceListHexagon"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceHexagon(model); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceListBodyBeveled"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceBodyBeveled(model); }, QStringLiteral("") );
  addFunction( QStringLiteral("faceListPinTqfp"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunModelPinTqfp(model); }, QStringLiteral("") );

  addFunction( QStringLiteral("model"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunModelBuild(); }, QStringLiteral("") );
  addFunction( QStringLiteral("modelCopy"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunModelCopy(); }, QStringLiteral("") );

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

  return parse( src );
  }
