/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDCONTAINERPLATENET_H
#define SDCONTAINERPLATENET_H

#include "SdContainer.h"

class SdSelector;
class SdPItemPlate;
class SdGraphRoad;

#define SD_TYPE_PLATE_NET "PlateNet"

class SdContainerPlateNet : public SdContainer
  {
    QString mNetName; //Name of net
  public:
    SdContainerPlateNet();
    SdContainerPlateNet( const QString netName );

    //Information
    //Return net name
    QString       getNetName() const { return mNetName; }
    //Return plate where in net recides
    SdPItemPlate *getPlate() const;
  };

#endif // SDCONTAINERPLATENET_H
