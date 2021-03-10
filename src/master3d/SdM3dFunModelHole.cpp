/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds model from outer region and inner (hole) region
*/
#include "SdM3dFunModelHole.h"

SdM3dFunModelHole::SdM3dFunModelHole() :
  SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_REGION, SDM3D_TYPE_REGION, SDM3D_TYPE_COLOR )
  {

  }


SdM3dModel SdM3dFunModelHole::toModel() const
  {
  return modelHole( mParamList[0]->toRegion(), mParamList[1]->toRegion(), mParamList[2]->toColor() );
  }




SdM3dModel SdM3dFunModelHole::modelHole(SdM3dRegion outer, SdM3dRegion hole, QColor color)
  {
  int outerCount = outer.count();
  int holeCount = hole.count();
  SdM3dModel model;
  if( outerCount == holeCount ) {
    //Make as equivalent
    SdM3dFace face;
    face.mColor = color;
    face.mContour.reserve(4);
    for( int i = 0; i < outerCount; i++ ) {
      QVector3D v0 = outer.at(i);
      QVector3D v1 = i + 1 < outerCount ? outer.at( i + 1 ) : outer.first();
      QVector3D v2 = i + 1 < holeCount ? hole.at( i + 1 ) : hole.first();
      QVector3D v3 = hole.at(i);
      face.mContour.clear();
      face.mContour << v0 << v1 << v2 << v3;
      model.append( face );
      }
    }
  else if( outerCount == 4 && (holeCount & 3) == 0 )
    return modelHoleSquareCircle( outer, hole, color );
  else if( holeCount == 4 && (holeCount & 3) == 0 )
    return modelHoleSquareCircle( hole, outer, color );
  //Can't build model
  return model;
  }





SdM3dModel SdM3dFunModelHole::modelHoleSquareCircle(SdM3dRegion square, SdM3dRegion circle, QColor color)
  {
  int a360 = circle.count();
  int a90 = a360 / 4;
  int a180 = a90 * 2;
  int a270 = a90 * 3;
  SdM3dModel model;
  SdM3dFace face;
  face.mColor = color;

  //Quadrant 0
  for( int i = 0; i < a90; i++ )
    face.mContour.append( circle.at(i) );
  face.mContour.append( square.at(2) );
  model.append( face );

  face.mContour.clear();
  face.mContour << circle.at(0) << square.at(2) << square.at(1);
  model.append( face );

  //Quadrant 1
  for( int i = a90; i < a180; i++ )
    face.mContour.append( circle.at(i) );
  face.mContour.append( square.at(3) );
  model.append( face );

  face.mContour.clear();
  face.mContour << circle.at(a90) << square.at(3) << square.at(2);
  model.append( face );


  //Quadrant 2
  for( int i = a180; i < a270; i++ )
    face.mContour.append( circle.at(i) );
  face.mContour.append( square.at(0) );
  model.append( face );

  face.mContour.clear();
  face.mContour << circle.at(a180) << square.at(0) << square.at(3);
  model.append( face );


  //Quadrant 3
  for( int i = a270; i < a360; i++ )
    face.mContour.append( circle.at(i) );
  face.mContour.append( square.at(1) );
  model.append( face );

  face.mContour.clear();
  face.mContour << circle.at(a270) << square.at(1) << square.at(0);
  model.append( face );

  return model;
  }



