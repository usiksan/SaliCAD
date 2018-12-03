#ifndef SDRATNET_H
#define SDRATNET_H

#include "SdPoint.h"

#include <QVector>

//Rat net pair define point pair witch is unconnected link
struct SdRatNetPair {
    SdPoint a,b;

    SdRatNetPair() : a(), b() {}
    SdRatNetPair( SdPoint sa, SdPoint sb ) : a(sa), b(sb) {}
  };

typedef QVector<SdRatNetPair> SdRatNet;

#endif // SDRATNET_H
