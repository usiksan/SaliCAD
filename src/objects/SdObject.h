/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Root object of Database object hierarhy
*/

#ifndef SDOBJECT_H
#define SDOBJECT_H

#include "SdConfig.h"
#include "SdObjectMap.h"
#include "SdUndo.h"
#include "SdClass.h"

#include <QJsonObject>



#define SDKO_TYPE      "type"
#define SDKO_ID        "id"

class SdContainer;
class SdProjectItem;

typedef QString SdId;
typedef QString SdUid;

class SdObject
  {
    SdContainer *mParent;  //Parent object
    bool         mDeleted; //Flag check if object deleted. Deleted objects not save
  public:
    SdObject();
    virtual ~SdObject() {}

    //Information
            QString   getId() const;
    virtual QString   getType() const = 0;
    virtual SdClass   getClass() const = 0;
    virtual bool      isContainer() const { return false; }

    //Hierarhy
    SdContainer*      getParent() const { return mParent; }
    SdContainer*      getRoot() const;
    void              setParent( SdContainer *parent );
    void              setProjectDirtyFlag();

    //Attach and detach
    virtual void      attach( SdUndo *undo );
    virtual void      undoAttach() {}
    virtual void      detach( SdUndo *undo );
    virtual void      undoDetach() {}
    bool              isDeleted() const { return mDeleted; }
    void              markDeleted( bool deleted ) { mDeleted = deleted; }
    void              deleteObject( SdUndo *undo );
    virtual bool      isUsed( SdObject *obj ) const;

    //Upgrade project item on new one
    virtual bool      upgradeProjectItem( SdUndo *undo, QWidget *parent );

    //Copy
    //Exact copy object
    SdObject*         copy() const;
    //Copy logic next object
    virtual SdObject* copyNext() const { return copy(); }

    //Clone contents object except mParent field.
    //Cloned object has no parent
    virtual void      cloneFrom( const SdObject *src );

    //Write and read object
    virtual void      writeObject( QJsonObject &obj ) const;
    QJsonObject       write() const;
    static  void      writePtr( const SdObject *ptr, const QString name, QJsonObject &obj );

    virtual void      readObject( SdObjectMap *map, const QJsonObject obj );

    static  SdObject* read( SdObjectMap *map, const QJsonObject obj );
    static  SdObject* readPtr( SdObjectMap *map, const QJsonObject obj );
    static  SdObject* readPtr( const QString name, SdObjectMap *map, const QJsonObject obj );
    static  SdObject* build( QString type );
  };

typedef SdObject *SdObjectPtr;

//Convert object only to desired type, other way - deleted
template <class T>
T* sdObjectOnly( SdObject *obj ) {
  T *t = dynamic_cast<T*>( obj );
  if( t ) return t;
  delete obj;
  return nullptr;
  }


//Helper template for dynamic type casting
//Usage
// fun( SdObject *obj ) {
//   SdPtr<SdGraph> graph( obj );
//   ...
template <class objectType>
class SdPtr {
    objectType *mPtr;
  public:
    SdPtr( SdObject *obj ) : mPtr( dynamic_cast<objectType*>(obj) ) {}

    bool isValid() const { return mPtr != nullptr; }

    operator bool () const { return mPtr != nullptr; }

    objectType *operator -> () { return mPtr; }

    objectType *ptr() const { return mPtr; }
  };


//Helper template for dynamic type casting
//Usage
// fun( SdObject *obj ) {
//   SdPtr<SdGraph> graph( obj );
//   ...
template <class objectType>
class SdPtrConst {
    const objectType *mPtr;
  public:
    SdPtrConst( const SdObject *obj ) : mPtr( dynamic_cast<const objectType*>(obj) ) {}

    bool isValid() const { return mPtr != nullptr; }

    operator bool () const { return mPtr != nullptr; }

    const objectType *operator -> () { return mPtr; }

    const objectType *ptr() const { return mPtr; }
  };


#endif // SDOBJECT_H
