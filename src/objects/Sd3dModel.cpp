#include "Sd3dModel.h"


//!
//! \brief sd3dModelWrite Writes model into JSON array object
//! \param model          3d model
//! \return               JSON array object with writed 3d model
//!
QJsonArray sd3dModelWrite(const Sd3dModel &model)
  {
  QJsonArray ar;
  for( auto const &face : model )
    ar.append( face.write() );
  return ar;
  }




//!
//! \brief sd3dModelRead Reads model from JSON array object
//! \param array         JSON array object from which will be readed 3d model
//! \return              3d model
//!
Sd3dModel sd3dModelRead(const QJsonArray &array)
  {
  Sd3dModel model;
  model.reserve( array.count() );
  Sd3dFace face;
  for( auto it = array.constBegin(); it != array.constEnd(); it++ ) {
    face.read( it->toObject() );
    model.append( face );
    }
  return model;
  }



//!
//! \brief sd3dModelWalls Builds walls on base bottom and top regions. Walls builded with color
//! \param bottom         Bottom region of walls
//! \param top            Top region of walls
//! \param color          Color of faces for the walls
//! \param close          If true then append wall with n-1 and 0 index vertex
//! \return               Model of walls
//!
Sd3dModel sd3dModelWalls(const Sd3dRegion &bottom, const Sd3dRegion &top, QColor color, bool close)
  {
  Sd3dModel model;
  Sd3dFace face;
  face.mColor = color;
  for( int i = 0; i < bottom.count() - 1; i++ ) {
    face.mContour.clear();
    face.mContour.append( bottom.at(i) );
    face.mContour.append( bottom.at(i+1) );
    face.mContour.append( top.at(i+1) );
    face.mContour.append( top.at(i) );
    model.append( face );
    }
  if( close ) {
    //Append wall with n-1 and 0 vertex
    face.mContour.clear();
    face.mContour.append( bottom.last() );
    face.mContour.append( bottom.first() );
    face.mContour.append( top.first() );
    face.mContour.append( top.last() );
    model.append( face );
    }
  return model;
  }


//!
//! \brief sd3dModelWall Builds walls on base bottom region and vector of grow. Walls builded with color
//! \param bottom        Bottom region of walls
//! \param grow
//! \param color         Color of faces for the walls
//! \param close         If true then append wall with n-1 and 0 index vertex
//! \return              Model of walls
//!
Sd3dModel sd3dModelWall( const Sd3dRegion &bottom, QVector3D grow, QColor color, bool close)
  {
  return sd3dModelWalls( bottom, sd3dRegionTranslate( bottom, grow ), color, close );
  }


//!
//! \brief sd3dModelExtrude Extrudes model from region in the direction of the normal vector with
//!                         specified shift amount color faces.
//!                         First face is bottom (begin of vector), last face is top (end of vector),
//!                         Middle faces are walls.
//! \param region           Region of bottom of model
//! \param shift            Shift amount of extrude
//! \param color            Face model color
//! \return                 Model extruded from region in the direction of the normal vector
//!
Sd3dModel sd3dModelExtrude(const Sd3dRegion &region, float shift, QColor color)
  {
  Sd3dModel md;
  Sd3dFace bot( region, color );

  //Bottom side
  md.append( bot );

  Sd3dFace top( bot.shift( shift ) );

  //Side walls
  md.append( sd3dModelWalls( region, top.mContour, color, true ) );

  //Top side
  md.append( top );

  return md;
  }


//!
//! \brief sd3dModelCylinder Builds cylinder model from its size
//! \param radius            Radius of circle of footing of cylinder
//! \param height            Height of cylinder
//! \param color             Color of cylinder
//! \return                  Cylinder model
//!
Sd3dModel sd3dModelCylinder(float radius, float height, QColor color)
  {
  return sd3dModelExtrude( sd3dRegionCircle( radius ), height, color );
  }





//!
//! \brief sd3dModelWallEven Builds wall from each pair of vertexes with grow vector and face color
//! \param regionPair        Region pair contains pair of vertexes (segments), count of vertexes must be even
//! \param grow              Wall grow vector
//! \param color             Face color
//! \return                  Model of walls
//!
Sd3dModel sd3dModelWallEven( const Sd3dRegion &regionPair, QVector3D grow, QColor color)
  {
  Sd3dRegion otherRegion( sd3dRegionTranslate( regionPair, grow ) );
  Sd3dModel model;
  model.reserve( regionPair.count() / 2 );
  Sd3dFace face;
  face.mColor = color;
  for( int i = 0; regionPair.count() - 1; i += 2 ) {
    face.mContour.clear();
    face.mContour.append( regionPair.at(i) );
    face.mContour.append( regionPair.at(i+1) );
    face.mContour.append( otherRegion.at(i+1) );
    face.mContour.append( otherRegion.at(i) );
    model.append( face );
    }
  return model;
  }




//!
//! \brief sd3dModelTranslate Translates mode to other model with offset
//! \param model              Source model to translate
//! \param offset             Offset of translation
//! \return                   Translated model
//!
Sd3dModel sd3dModelTranslate(const Sd3dModel &model, QVector3D offset)
  {
  Sd3dModel dest;
  dest.reserve( model.count() );
  for( auto const &face : model )
    dest.append( face.translate( offset ) );
  return dest;
  }




//!
//! \brief sd3dModelMap Convert source mode to destignation with matrix
//! \param model        Source model
//! \param matrix       Conversion matrix
//! \return             Converted model
//!
Sd3dModel sd3dModelMap(const Sd3dModel &model, const QMatrix4x4 &matrix)
  {
  Sd3dModel dest;
  dest.reserve( model.count() );
  for( auto const &face : model )
    dest.append( face.map( matrix ) );
  return dest;
  }




//!
//! \brief sd3dModelMapInPlace Convert model in place using matrix
//! \param model               Model to convert
//! \param matrix              Conversion matrix
//!
void sd3dModelMapInPlace(Sd3dModel &model, const QMatrix4x4 &matrix)
  {
  for( auto &face : model )
    face.mapInPlace( matrix );
  }




//!
//! \brief sd3dModelCurve Creates curve solid model from base region and rotation angle
//! \param region         Source region which will be form curved model
//! \param rotationCenter Center of rotation
//! \param rotationAxis   Axis of rotation
//! \param angle          Angle of rotation
//! \param color          Faces color
//! \param bottomFace     If true then appended bottom face from source region
//! \return               Curved solid model
//!
Sd3dModel sd3dModelCurve(const Sd3dRegion &region, QVector3D rotationCenter, QVector3D rotationAxis, float angle, QColor color, bool bottomFace )
  {
  Sd3dModel model;

  //Bottom face
  if( bottomFace )
    model.append( Sd3dFace( region, color ) );

  //Walls
  Sd3dRegion bot( region );
  if( angle > 0 ) {
    float addon = 10.0;
    do {
      //Build matrix to rotate to current angle
      QMatrix4x4 matrix;
      matrix.translate( -rotationCenter );
      matrix.rotate( qMin( addon, angle ), rotationAxis );
      matrix.translate( rotationCenter );

      //Build rotated redion
      Sd3dRegion top;
      for( auto const &v : qAsConst(region) )
        top.append( matrix.map( v ) );

      //Append walls
      model.append( sd3dModelWalls( bot, top, color, true ) );

      bot = top;
      addon += 10.0;
      }
    while( addon < angle );
    }
  else {
    float addon = -10.0;
    do {
      //Build matrix to rotate to current angle
      QMatrix4x4 matrix;
      matrix.translate( -rotationCenter );
      matrix.rotate( qMax( addon, angle ), rotationAxis );
      matrix.translate( rotationCenter );

      //Build rotated redion
      Sd3dRegion top;
      for( auto const &v : region )
        top.append( matrix.map( v ) );

      //Append walls
      model.append( sd3dModelWalls( bot, top, color, true ) );

      bot = top;
      addon -= 10.0;
      }
    while( addon > angle );
    }

  //Top face
  model.append( Sd3dFace( bot, color ) );

  return model;
  }




//!
//! \brief sd3dModelFlatHole Builds model of flat surface with hole. Outer of surface and hole must contains equals count of edges
//! \param outer             Outer of surface
//! \param hole              Hole
//! \param color             Color of model
//! \return                  Model with hole
//!
Sd3dModel sd3dModelFlatHole(const Sd3dRegion &outer, const Sd3dRegion &hole, QColor color)
  {
  int outerCount = outer.count();
  int holeCount = hole.count();
  Sd3dModel model;
  if( outerCount == holeCount ) {
    //Make as equivalent
    Sd3dFace face;
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
  return model;
  }
