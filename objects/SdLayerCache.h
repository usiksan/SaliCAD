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
#ifndef SDLAYERCACHE_H
#define SDLAYERCACHE_H

#include "SdLayer.h"

#include <QMap>

typedef QMap<int,SdLayerPtr> SdLayerPtrMap;

class SdLayerCache
  {
    SdLayerPtr     mMap[stmCountMax]; //Layer map for all stratum
    //SdLayerPtrMap  mMap;     //Layer map for all stratum
    int            mStratum;          //Cached stratum
    SdLayer       *mCache;            //Cached layer for stratum
  public:
    SdLayerCache();

    //Build new stratum association
    void     rebuild(const SdLayerPtrTable &tab, SdLayerTrace tr);

    //Return layer mapped to stratum
    SdLayer *getVisibleLayer( int stratum );

    //Set layer usage. Set usage flag for stratumCount layers of cache
    void     setLayerUsage(int stratumCount);
  };

#endif // SDLAYERCACHE_H
