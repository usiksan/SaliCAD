#include "SdScriptParser3d.h"

//Predefined variable to insert faces into part
#include "SdScriptPartModel.h"
//Predefined variable to insert 2d graphics into part
#include "SdScriptPartGraph.h"


#include "SdScriptValueFunStringPadRectThrou.h"
#include "SdScriptValueFunStringPadCircleThrou.h"
#include "SdScriptValueFunStringPinIndex.h"
#include "SdScriptValueFunStringPinMatrix.h"

#include "SdScriptValueFunVertexBuild.h"
#include "SdScriptValueFunVertexOffset.h"
#include "SdScriptValueFunVertexTranslate.h"
#include "SdScriptValueFunVertexCenterOfRegion.h"

#include "SdScriptValueFunRegionRect.h"
#include "SdScriptValueFunRegionCircle.h"
#include "SdScriptValueFunRegionTranslate.h"

#include "SdScriptValueFunFaceBuild.h"
#include "SdScriptValueFunFaceTranslate.h"
#include "SdScriptValueFunFaceTriangle.h"

#include "SdScriptValueFunModelWall.h"
#include "SdScriptValueFunModelWallEven.h"
#include "SdScriptValueFunModelExtrude.h"
#include "SdScriptValueFunModelBox.h"
#include "SdScriptValueFunModelCylinder.h"
#include "SdScriptValueFunModelTranslate.h"
#include "SdScriptValueFunModelPinTqfp.h"
#include "SdScriptValueFunModelHexagon.h"
#include "SdScriptValueFunModelCopy.h"
#include "SdScriptValueFunModelBodyBeveled.h"

#include "SdScriptValueFunGraphLine.h"
#include "SdScriptValueFunGraphCircle.h"
#include "SdScriptValueFunGraphRect.h"
#include "SdScriptValueFunGraphRectFilled.h"
#include "SdScriptValueFunGraphPin.h"


SdScriptParser3d::SdScriptParser3d(QTableWidget *tableWidget) :
  SdScriptParser(tableWidget)
  {
  addFunction( QStringLiteral("stringPadRectThrough"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunStringPadRectThrou(); } );
  addFunction( QStringLiteral("stringPadCircleThrough"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunStringPadCircleThrou(); } );
  addFunction( QStringLiteral("stringPinIndex"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunStringPinIndex(); } );
  addFunction( QStringLiteral("stringPinMatrix"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunStringPinMatrix(); } );

  addFunction( QStringLiteral("vertex"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunVertexBuild(); } );
  addFunction( QStringLiteral("vertexOffset"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunVertexOffset(); } );
  addFunction( QStringLiteral("vertexTranslate"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunVertexTranslate(); } );
  addFunction( QStringLiteral("vertexCenterOfRegion"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunVertexCenterOfRegion(); } );

  addFunction( QStringLiteral("regionRect"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunRegionRect(); } );
  addFunction( QStringLiteral("regionCircle"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunRegionCircle(); } );
  addFunction( QStringLiteral("regionTranslate"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunRegionTranslate(); } );

  addFunction( QStringLiteral("face"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceBuild(); } );
  addFunction( QStringLiteral("faceTranslate"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceTranslate(); } );
  addFunction( QStringLiteral("faceTriangle"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunFaceTriangle(); } );

  addFunction( QStringLiteral("modelWall"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunModelWall(); } );
  addFunction( QStringLiteral("modelWallEven"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunModelWallEven(); } );
  addFunction( QStringLiteral("modelExtrude"), [] () -> SdScriptValueFunction* { return new SdM3dFunModelExtrude(); } );
  addFunction( QStringLiteral("modelBox"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunModelBox(); } );
  addFunction( QStringLiteral("modelCylinder"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunModelCylinder(); } );
  addFunction( QStringLiteral("modelTranslate"), [] () -> SdScriptValueFunction* { return new SdM3dFunModelTranslate(); } );
  addFunction( QStringLiteral("modelPinTqfp"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunModelPinTqfp(); } );
  addFunction( QStringLiteral("modelHexagon"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunModelHexagon(); } );
  addFunction( QStringLiteral("modelCopy"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunModelCopy(); } );
  addFunction( QStringLiteral("modelBodyBeveled"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunModelBodyBeveled(); } );

  addFunction( QStringLiteral("graphLine"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunGraphLine(); } );
  addFunction( QStringLiteral("graphCircle"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunGraphCircle(); } );
  addFunction( QStringLiteral("graphRect"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunGraphRect(); } );
  addFunction( QStringLiteral("graphRectFilled"), [] () -> SdScriptValueFunction* { return new SdScriptValueFunGraphRectFilled(); } );
  addFunction( QStringLiteral("graphPin"), [] () -> SdScriptValueFunction* { return new SdM3dFunGraphPin(); } );
  }

SdScriptProgrammPtr SdScriptParser3d::parse3d(const QString src, SdPItemPart *part)
  {
  //Insert predefined variables
  mVariables.insert( QStringLiteral("partModel"), new SdScriptPartModel( part ) );
  mVariables.insert( QStringLiteral("partFlat"), new SdScriptPartGraph( part ) );

  return parse( src );
  }
