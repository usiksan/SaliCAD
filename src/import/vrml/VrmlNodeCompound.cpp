#include "VrmlNodeCompound.h"
#include "SdScanerVrml.h"

VrmlNodeCompound::VrmlNodeCompound() :
  VrmlNode()
  {

  }

VrmlNodeCompound::VrmlNodeCompound(const VrmlNodeCompound *compound) :
  VrmlNode( compound )
  {
  for( auto ptr : compound->mChildren )
    mChildren.append( makeCopy(ptr) );
  }

VrmlNodeCompound::~VrmlNodeCompound()
  {
  qDeleteAll(mChildren);
  }

void VrmlNodeCompound::parseChildren(SdScanerVrml *scaner)
  {
  if( scaner->matchToken('[') ) {
    //Multiple children
    while( !scaner->isEndOfScan() ) {
      VrmlNode *node = parse2Declaration( scaner );
      if( node != nullptr )
        mChildren.append( node );
      if( scaner->isError() || scaner->matchToken(']') ) return;
      if( !scaner->tokenNeed( ',', QStringLiteral("Need to be , beatween children") ) )
        return;
      }
    }
  else {
    //Single child
    VrmlNode *node = parse2Declaration( scaner );
    if( node != nullptr )
      mChildren.append( node );
    }
  }






void VrmlNodeCompound::generateFaces(Sd3drModel *model, Sd3drInstance &instance, Sd3drBody &body) const
  {
  for( auto ptr : mChildren )
    ptr->generateFaces( model, instance, body );
  }
