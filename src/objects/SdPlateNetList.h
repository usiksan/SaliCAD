/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Plate net list. Contains list of all named nets.
  On list base we build ratnet.
*/
#ifndef SDPLATENETLIST_H
#define SDPLATENETLIST_H

#include "SdRatNet.h"
#include "SdPlateNet.h"
#include "SdPlateNetContainer.h"

#include <QMap>
#include <QString>

//List of plate nets
class SdPlateNetList : public SdPlateNetContainer {
    QMap<QString,SdPlateNet*> mNetList; //Net list
  public:
    SdPlateNetList();
    ~SdPlateNetList();

    //Add net segment to appropriate net
    void    addNetSegment( SdGraphTraced *traced, const QString netName, SdPropStratum s, SdPoint p1, SdPoint p2);

    //For each net build ratnet
    void    buildRatNet( SdRatNet *ratNet );

    //Clear net list
    void    clear();

    //Find nearest point for netName in net list
    SdPoint nearestPoint( const QString netName, SdPoint p );
  };


#endif // SDPLATENETLIST_H
