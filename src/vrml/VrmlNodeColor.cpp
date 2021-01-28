#include "VrmlNodeColor.h"
#include "SdScanerVrml.h"

VrmlNodeColor::VrmlNodeColor() :
  VrmlNode()
  {

  }




bool VrmlNodeColor::parse(SdScanerVrml *scaner, const QString &fieldType)
  {
  if( fieldType == QStringLiteral("color") ) {
    if( !scaner->tokenNeed( '[', QStringLiteral("Waiting color list")) )
      return true;
    while( !scaner->matchToken( ']') ) {
      if( scaner->isEndOfScan() ) {
        scaner->error( QStringLiteral("Uncompleted color list") );
        break;
        }
      if( scaner->isError() )
        break;

      VrmlColor color;
      color.parse( scaner );
      mColorList.append( color );
      if( !scaner->matchToken(',') ) {
        scaner->tokenNeed(']', QStringLiteral("Need ]") );
        break;
        }
      }
    return true;
    }
  return false;
  }


quint32 VrmlNodeColor::colorGet(int index, quint32 color)
  {
  Q_UNUSED(color)
  if( index >= 0 && index < mColorList.count() )
    return mColorList.at();
  }
