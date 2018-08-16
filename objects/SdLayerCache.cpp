/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Layer cache for stratum mapping
*/
#include "SdLayerCache.h"


SdLayerCache::SdLayerCache() :
  mStratum(-1)
  {

  }



//Build new stratum association
void SdLayerCache::rebuild(const SdLayerPtrTable &tab, SdLayerTrace tr)
  {
  mStratum = -1;
  mMap.clear();
  for( auto iter = tab.cbegin(); iter != tab.cend(); iter++ )
    if( iter.value()->trace() == tr )
      mMap.insert( iter.value()->stratum(), iter.value() );
  //Check all available 1' stratum
  for( int s = 1; s < stmEnd; s <<= 1 ) {
    if( !mMap.contains(s) ) {
      bool finded = false;
      for( auto iter = tab.cbegin(); iter != tab.cend() && !finded; iter++ )
        if( iter.value()->trace() == tr && (iter.value()->stratum() & s) != 0 ) {
          mMap.insert( s, iter.value() );
          finded = true;
          }
      if( !finded ) {
        //No layer defined for this stratum. Insert default.
        mMap.insert( s, tab.value(LID0_TRACE_DEFAULT) );
        }
      }
    }
  }



//Return layer mapped to stratum
SdLayer *SdLayerCache::getLayer(int stratum)
  {
  //If no stratum then return no layer
  if( stratum == 0 )
    return nullptr;

  //Check if cache hit then return cached layer
  if( mStratum == stratum )
    return mCache;

  //Look layer in table
  mStratum = stratum;
  //Check if mapping present
  if( mMap.contains(stratum) )
    //Then return existing association
    mCache = mMap.value(stratum);
  else {
    //Find new association
    for( int i = 0x1; i < stmEnd; i <<= 1 )
      if( stratum & i ) {
        mCache = mMap.value(i);
        break;
        }
    mMap.insert( stratum, mCache );
    }
  return mCache;
  }
