/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDOWNER_H
#define SDOWNER_H

#include "SdObject.h"

class SdOwner : public SdObject
  {
  public:
    SdOwner();

    // SdObject interface
  public:
    virtual void readObject(SdObjectMap *map, const QJsonObject obj) override;
  };


#endif // SDOWNER_H
