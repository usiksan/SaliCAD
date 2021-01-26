#include "VrmlNodeIndexedFaceSet.h"
#include "SdScanerVrml.h"

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




void VrmlNodeIndexedFaceSet::parse(SdScanerVrml *scaner)
  {
  if( !scaner->tokenNeed( '{', QStringLiteral("No IndexedFaceSet") ) )
    return;

  while( !scaner->matchToken('}') ) {
    if( scaner->isEndOfScan() ) {
      scaner->error( QStringLiteral("Uncompleted IndexedFaceSet") );
      return;
      }
    if( scaner->isError() )
      return;
    QString fieldType;
    if( !scaner->tokenNeedValue( 'n', fieldType, QStringLiteral("Need IndexedFaceSet field") ) )
      return;
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

    else {
      scaner->error( QStringLiteral("Illegal field type for IndexedFaceSet %1").arg(fieldType) );
      return;
      }
    }
  }

