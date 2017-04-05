/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  SdContainer - container object contains other objects
*/

#ifndef SDCONTAINER_H
#define SDCONTAINER_H

#include "SdObject.h"
#include <QList>
#include <QMap>
#include <functional>

class SdContext;

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

    virtual void forEach( quint64 classMask, std::function<bool(SdObject*)> fun1 );

    virtual void insertChild( SdObject *child );
    virtual void undoInsertChild( SdObject *child );

    virtual void deleteChild( SdObject *child );
    virtual void undoDeleteChild( SdObject *child );


    virtual void cloneFrom( const SdObject *src) override;

            void draw( SdContext *context );
  private:
    void         clearChildList();
  };


#endif // SDCONTAINER_H
