#include "VrmlNodePointSet.h"

VrmlNodePointSet::VrmlNodePointSet() :
  VrmlNode(),
  mColor(nullptr),
  mCoordinate(nullptr)
  {

  }



VrmlNodePointSet::VrmlNodePointSet(const VrmlNodePointSet *ps) :
  VrmlNode( ps ),
  mColor( makeCopy(ps->mColor) ),
  mCoordinate( makeCopy(ps->mCoordinate) )
  {

  }



VrmlNodePointSet::~VrmlNodePointSet()
  {
  deleteNode( mColor );
  deleteNode( mCoordinate );
  }




bool VrmlNodePointSet::parse(SdScanerVrml *scaner, const QString &fieldType)
  {
  if( fieldType == QStringLiteral("color") )
    mColor = parse2Declaration( scaner );

  else if( fieldType == QStringLiteral("coord") )
    mCoordinate = parse2Declaration( scaner );

  else return false;
  return true;
  }




VrmlNode *VrmlNodePointSet::copy() const
  {
  return new VrmlNodePointSet( this );
  }
