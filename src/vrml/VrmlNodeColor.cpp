#include "VrmlNodeColor.h"
#include "SdScanerVrml.h"

VrmlNodeColor::VrmlNodeColor()
  {

  }


void VrmlNodeColor::parse(SdScanerVrml *scaner)
  {
  mColorList.clear();
  if( scaner->matchTokenValue( 'n', QStringLiteral("color") ) ) {
    if( !scaner->tokenNeed( '[', QStringLiteral("Waiting color list")) )
      return;
    while( !scaner->matchToken( ']') ) {
      if( scaner->isEndOfScan() ) {
        scaner->error( QStringLiteral("Uncompleted color list") );
        return;
        }
      if( scaner->isError() )
        return;
      VrmlColor color;
      color.parse( scaner );
      mColorList.append( color );
      if( !scaner->matchToken(',') ) {
        if( !scaner->tokenNeed(']', QStringLiteral("Need ]") ) )
          return;
        break;
        }
      }
    }
  }

