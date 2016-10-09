/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  SdOwner - container object contains other objects
*/

#ifndef SDCONTAINER_H
#define SDCONTAINER_H

#include "SdObject.h"
#include <QList>
#include <functional>

typedef QList<SdObjectPtr> SdObjectPtrList;

class SdContainer : public SdObject
  {
  protected:
    SdObjectPtrList mChildList; //List of child objects
  public:
    SdContainer();
    ~SdContainer();

    virtual void readObject(SdObjectMap *map, const QJsonObject obj) override;

    virtual void writeObject(QJsonObject &obj) const override;

    virtual void forEach( int classMask, std::function<bool(SdObject*)> fun1 );

  };


#endif // SDCONTAINER_H
