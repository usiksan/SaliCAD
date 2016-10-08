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

class SdIterator {
  protected:
    int classMask;
  public:
    SdIterator( int mask ) : classMask(mask) {}

    virtual bool operation( SdObject *obj ) = 0;

    virtual bool isAccept( int classId ) { return classId & classMask; }
  };


class SdOwner : public SdObject
  {
  public:
    SdOwner();

    virtual void readObject(SdObjectMap *map, const QJsonObject obj) override;

    virtual void forEach( SdIterator &i ) = 0;

    virtual void writeObject(QJsonObject &obj) const override;
  };


#endif // SDOWNER_H
