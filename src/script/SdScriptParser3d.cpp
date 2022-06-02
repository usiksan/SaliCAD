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

#include "SdScriptValueFunLFaceIndexed.h"
#include "SdScriptValueFunLFaceWall.h"
#include "SdScriptValueFunLFaceWallIndexed.h"
#include "SdScriptValueFunLFaceAppend.h"
#include "SdScriptValueFunLFaceUnion.h"
#include "SdScriptValueFunLFaceBox.h"
#include "SdScriptValueFunLFaceCylinder.h"
#include "SdScriptValueFunLFaceHexagon.h"
#include "SdScriptValueFunLFaceBodyBeveled.h"

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
  addFunction( QStringLiteral("stringPadRectThrough"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunStringPadRectThrou(); } );
  addFunction( QStringLiteral("stringPadCircleThrough"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunStringPadCircleThrou(); } );
  addFunction( QStringLiteral("stringPinIndex"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunStringPinIndex(); } );
  addFunction( QStringLiteral("stringPinMatrix"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunStringPinMatrix(); } );

  addFunction( QStringLiteral("matrix"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunMatrix(); } );

  addFunction( QStringLiteral("vertex"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunVertexBuild(model); } );
  addFunction( QStringLiteral("vertexOffset"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunVertexOffset(model); } );
//  addFunction( QStringLiteral("vertexTranslate"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunVertexTranslate(); } );
//  addFunction( QStringLiteral("vertexCenterOfRegion"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunVertexCenterOfRegion(); } );

  addFunction( QStringLiteral("faceRect"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceRect(model); } );
  addFunction( QStringLiteral("faceCircle"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceCircle(model); } );
  addFunction( QStringLiteral("facePart"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunFacePart(); } );
  addFunction( QStringLiteral("faceFlat"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceFlat(model); } );
  addFunction( QStringLiteral("faceDuplicate"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceDuplicate(model); } );
  addFunction( QStringLiteral("faceDuplicateShift"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceDuplicateShift(model); } );
  //addFunction( QStringLiteral("faceTriangle"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceTriangle(); } );

  addFunction( QStringLiteral("faceListIndexed"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceIndexed(); } );
  addFunction( QStringLiteral("faceListWall"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceWall(); } );
  addFunction( QStringLiteral("faceListWallIndexed"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceWallIndexed(); } );
  addFunction( QStringLiteral("faceListAppend"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceAppend(); } );
  addFunction( QStringLiteral("faceListUnion"), [] () -> SdScriptValueFunction* { return new SdM3dFunLFaceUnion(); } );
  addFunction( QStringLiteral("faceListBox"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceBox(model); } );
  addFunction( QStringLiteral("faceListCylinder"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceCylinder(model); } );
  addFunction( QStringLiteral("faceListHexagon"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceHexagon(model); } );
  addFunction( QStringLiteral("faceListBodyBeveled"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunLFaceBodyBeveled(model); } );
  addFunction( QStringLiteral("faceListPinTqfp"), [model] () -> SdScriptValueFunction* { return new SdScriptValueFunModelPinTqfp(model); } );

  addFunction( QStringLiteral("model"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunModelBuild(); } );
  addFunction( QStringLiteral("modelCopy"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunModelCopy(); } );

  addFunction( QStringLiteral("graphLine"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunGraphLine(); } );
  addFunction( QStringLiteral("graphCircle"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunGraphCircle(); } );
  addFunction( QStringLiteral("graphRect"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunGraphRect(); } );
  addFunction( QStringLiteral("graphRectFilled"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunGraphRectFilled(); } );
  addFunction( QStringLiteral("graphPin"), [] () -> SdScriptValueFunction* { return new SdM3dFunGraphPin(); } );
  }




SdScriptProgrammPtr SdScriptParser3d::parse3d(const QString src, SdPItemPart *part, Sd3dModel *model)
  {
  //Insert predefined variables
  mVariables.insert( QStringLiteral("partModel"), new SdScriptPartModel( model ) );
  mVariables.insert( QStringLiteral("partFlat"), new SdScriptPartGraph( part ) );
  mVariables.insert( QStringLiteral("matrix1"), new SdScriptValueVariableMatrix() );

  return parse( src );
  }
