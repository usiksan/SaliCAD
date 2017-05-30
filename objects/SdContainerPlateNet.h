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

#define SD_TYPE_PLATE_NET "PlateNet"

class SdContainerPlateNet : public SdContainer
  {
  public:
    SdContainerPlateNet();
  };

#endif // SDCONTAINERPLATENET_H
