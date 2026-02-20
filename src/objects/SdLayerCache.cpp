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
void SdLayerCache::rebuild(const SdLayerPtrMap &tab, SdLayerTrace tr)
  {
  mStratum = -1;
  mCache = nullptr;
  for( int i = 0; i < stmCountMax; i++ ) {
    //Find layer with match stratum and trace type
    SdPvStratum stratum( 1 << i );
    mMap[i] = nullptr; // tab.value(LID0_TRACE_DEFAULT);
    for( auto iter = tab.cbegin(); iter != tab.cend(); iter++ )
      if( iter.value()->trace() == tr && iter.value()->stratum().isIntersect(stratum) ) {
        mMap[i] = iter.value();
        break;
        }
    }
  }



//Return layer mapped to stratum
SdLayer *SdLayerCache::getVisibleLayer(SdPvStratum stratum)
  {
  //If no stratum then return no layer
  if( stratum.isEmpty() )
    return nullptr;

  //Check if cache hit then return cached layer
  if( mStratum == stratum && mCache && mCache->isVisible() )
    return mCache;

  //Look layer in table
  mStratum = stratum;
  mCache = nullptr;
  for( int i = 0; i < stmCountMax; i++ )
    if( mStratum.isIntersect(1 << i) && mMap[i] && mMap[i]->isVisible() ) {
      mCache = mMap[i];
      return mCache;
      }

  //Visible layer not found
  return nullptr;
  }




//Set layer usage. Set usage flag for stratumCount layers of cache
void SdLayerCache::setLayerUsage( int stratumCount )
  {
  stratumCount = qMin( stratumCount, stmCountMax );
  if( stratumCount == 1 ) {
    if( mMap[0] ) {
      mMap[0]->setUsage();
      mMap[0]->pair()->setUsage();
      }
    }
  else {
    if( mMap[stmCountMax-1] ) {
      mMap[stmCountMax-1]->setUsage();
      mMap[stmCountMax-1]->pair()->setUsage();
      }
    }
  for( int i = 0; i < stratumCount - 1; i++ ) {
    if( mMap[i] ) {
      mMap[i]->setUsage();
      mMap[i]->pair()->setUsage();
      }
    }
  }
