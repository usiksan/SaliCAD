#include "VrmlNodeNormal.h"
#include "SdScanerVrml.h"



void VrmlNodeNormal::parse(SdScanerVrml *scaner)
  {
  mVectorList.clear();
  if( !scaner->tokenNeed( '{', QStringLiteral("Need Normal block") ) )
    return;
  if( scaner->matchTokenValue( 'n', QStringLiteral("vector") ) )
    scaner->parseVectorTable( mVectorList, QStringLiteral("Waiting vector list") );
  scaner->tokenNeed( '}', QStringLiteral("Need Normal close block") );
  }

