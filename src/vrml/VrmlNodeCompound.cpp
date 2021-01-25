#include "VrmlNodeCompound.h"
#include "SdScanerVrml.h"

VrmlNodeCompound::VrmlNodeCompound()
  {

  }

void VrmlNodeCompound::parseChildren(SdScanerVrml *scaner)
  {
  if( scaner->matchToken('[') ) {
    while( !scaner->matchToken(']') ) {
      if( scaner->isEndOfScan() ) return;
      parse2Declaration( scaner, &mChildren );
      }
    }
  }

void VrmlNodeCompound::cloneNodeCompound(VrmlNodeCompound *destNode) const
  {
  cloneNode( destNode );
  for( auto ptr : mChildren )
    destNode->mChildren.append( ptr->copy() );
  }
