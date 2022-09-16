#include "VrmlNodeIndexedFaceSet.h"
#include "SdScanerVrml.h"
#include "3dr/Sd3drModel.h"

VrmlNodeIndexedFaceSet::VrmlNodeIndexedFaceSet() :
  VrmlNode(),
  mColor(nullptr),
  mCoordinate(nullptr),
  mNormal(nullptr),
  mTexCoord(nullptr),
  mCcw(true),
  mColorIndex(),
  mColorPerVertex(true),
  mConvex(true),
  mCoordIndex(),
  mCreaseAngle(0.0),
  mNormalIndex(),
  mNormalPerVertex(true),
  mSolid(true),
  mTexCoordIndex()
  {

  }



VrmlNodeIndexedFaceSet::VrmlNodeIndexedFaceSet(const VrmlNodeIndexedFaceSet *faceSet) :
  VrmlNode( faceSet ),
  mColor( makeCopy(faceSet->mColor) ),
  mCoordinate( makeCopy(faceSet->mCoordinate) ),
  mNormal( makeCopy(faceSet->mNormal) ),
  mTexCoord( makeCopy(faceSet->mTexCoord) ),
  mCcw( faceSet->mCcw ),
  mColorIndex( faceSet->mColorIndex ),
  mColorPerVertex( faceSet->mColorPerVertex ),
  mConvex( faceSet->mConvex ),
  mCoordIndex( faceSet->mCoordIndex ),
  mCreaseAngle( faceSet->mCreaseAngle ),
  mNormalIndex( faceSet->mNormalIndex ),
  mNormalPerVertex( faceSet->mNormalPerVertex ),
  mSolid( faceSet->mSolid ),
  mTexCoordIndex( faceSet->mTexCoordIndex )
  {

  }

VrmlNodeIndexedFaceSet::~VrmlNodeIndexedFaceSet()
  {
  deleteNode( mColor );
  deleteNode( mCoordinate );
  deleteNode( mNormal );
  deleteNode( mTexCoord );
  }





bool VrmlNodeIndexedFaceSet::parse(SdScanerVrml *scaner, const QString &fieldType)
  {
  if( fieldType == QStringLiteral("color") )
    mColor = parse2Declaration( scaner );

  else if( fieldType == QStringLiteral("coord") )
    mCoordinate = parse2Declaration( scaner );

  else if( fieldType == QStringLiteral("normal") )
    mNormal = parse2Declaration( scaner );

  else if( fieldType == QStringLiteral("texCoord") )
    mTexCoord = parse2Declaration( scaner );

  else if( fieldType == QStringLiteral("ccw") )
    scaner->tokenNeedValueBool( mCcw, QStringLiteral("Need ccw value") );

  else if( fieldType == QStringLiteral("colorIndex") )
    scaner->parseInt32Table( mColorIndex, QStringLiteral("Need color index table") );

  else if( fieldType == QStringLiteral("colorPerVertex") )
    scaner->tokenNeedValueBool( mColorPerVertex, QStringLiteral("Need colorPerVertex value") );

  else if( fieldType == QStringLiteral("convex") )
    scaner->tokenNeedValueBool( mConvex, QStringLiteral("Need convex value") );

  else if( fieldType == QStringLiteral("coordIndex") )
    scaner->parseInt32Table( mCoordIndex, QStringLiteral("Need coordIndex table") );

  else if( fieldType == QStringLiteral("creaseAngle") )
    scaner->tokenNeedValueFloat( 'f', mCreaseAngle, QStringLiteral("No creaseAngle") );

  else if( fieldType == QStringLiteral("normalIndex") )
    scaner->parseInt32Table( mNormalIndex, QStringLiteral("Need normalIndex table") );

  else if( fieldType == QStringLiteral("normalPerVertex") )
    scaner->tokenNeedValueBool( mNormalPerVertex, QStringLiteral("Need normalPerVertex value") );

  else if( fieldType == QStringLiteral("solid") )
    scaner->tokenNeedValueBool( mSolid, QStringLiteral("Need solid value") );

  else if( fieldType == QStringLiteral("texCoordIndex") )
    scaner->parseInt32Table( mTexCoordIndex, QStringLiteral("Need texCoordIndex table") );

  else return false;
  return true;
  }




void VrmlNodeIndexedFaceSet::generateFaces(Sd3drModel *model, Sd3drInstance &instance, Sd3drBody &body) const
  {
  VrmlNodeCoordinate *coordinate = dynamic_cast<VrmlNodeCoordinate*>( mCoordinate );
  VrmlNodeColor *color = dynamic_cast<VrmlNodeColor*>( mColor );
  if( coordinate != nullptr ) {
    coordinate->generateFaces( model, instance, body );
    if( mColorIndex.count() && color != nullptr ) {
      //Different colors for different faces
      //We try group all face with same color into one body
      int faceIndex = 0;
      int faceColorIndex = mColorIndex.at(0);
      body.clear();
      body.colorListSet( color->material(faceColorIndex) );
      Sd3drFace face;
      for( auto coordIndex : mCoordIndex ) {
        if( coordIndex < 0 ) {
          //Vertex accumulate complete
          //Normal accum complete
          body.faceAppend( face );
          face.clear();
          faceIndex++;
          if( faceIndex < mColorIndex.count() && mColorIndex.at(faceIndex) != faceColorIndex ) {
            faceColorIndex = mColorIndex.at(faceIndex);
            //Next face with different color, so we append current body to instance and
            // begin next body with new color
            instance.add( body );
            body.clear();
            body.colorListSet( color->material(faceColorIndex) );
            }
          }
        else face.append( coordinate->modelIndex(coordIndex) );
        }
      }
    else {
      //Single predefined color for all faces
      Sd3drFace face;
      for( auto coordIndex : mCoordIndex ) {
        if( coordIndex < 0 ) {
          //Vertex accumulate complete
          //Normal accum complete
          body.faceAppend( face );
          face.clear();
          }
        else face.append( coordinate->modelIndex(coordIndex) );
        }
      }
    }
  }
