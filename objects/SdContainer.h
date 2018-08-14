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
    ~SdContainer() override;

    //Information
    virtual bool isContainer() const override { return true; }

    virtual void readObject(SdObjectMap *map, const QJsonObject obj) override;

    virtual void writeObject(QJsonObject &obj) const override;

    virtual void insertChild( SdObject *child, SdUndo *undo );
    virtual void undoInsertChild( SdObject *child );
    virtual void redoInsertChild( SdObject *child );

    virtual void deleteChild( SdObject *child, SdUndo *undo );
    virtual void undoDeleteChild( SdObject *child );
    virtual void redoDeleteChild( SdObject *child );


    virtual void cloneFrom( const SdObject *src) override;

            void draw( SdContext *context );
/*[] (SdObject *obj) -> bool {

   }
   */
            void forEach( quint64 classMask, std::function<bool(SdObject*)> fun1 );
            void forEachConst( quint64 classMask, std::function<bool(SdObject*)> fun1 ) const;

            void sort( std::function<bool(SdObject *obj1, SdObject *obj2)> fun1 );

    //Return object and its state behind point
    SdObject    *behindPoint( quint64 classMask, SdPoint p, int *state );

  private:
    void         clearChildList();

    // SdObject interface
  public:
    //Return true if used any of objects in container but exclude itself objects in container
    virtual bool isUsed(SdObject *test) const override;
  };


#endif // SDCONTAINER_H
