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
class SdCopyMap;
class SdProjectItem;
class QWidget;
class SdObject;
class SdJsonWriter;
class SdJsonReader;

typedef QString SdId;
typedef QString SdUid;

using SdObjectPtr = SdObject*;

class SdObject
  {
    SdContainer *mParent;     //!< Parent object
    int          mCpid;       //!< id for crossproject association
    bool         mDeleted;    //!< Flag check if object deleted. Deleted objects not save

    static int   mCpidCount;  //!< Count for generation session id
  public:
    SdObject();
    virtual ~SdObject() {}

    //Information
            QString   getId() const;
    virtual QString   getType() const = 0;
    virtual SdClass   getClass() const = 0;
    virtual bool      isContainer() const { return false; }

    //Session id for crossproject association
    int               cpid() const { return mCpid; }
    void              cpidInit() { mCpid = mCpidCount++; }

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



    //=================================================
    //                  Copy
    //!
    //! \brief copy    Returns copy of this object. Copy is not exact but modified with copyMap
    //! \param copyMap Structure for mapping copying substitutes
    //! \param next    Make simple or next copy. Next copy available not for all objects.
    //!                For example: pin name A23 with next copy return A24
    //! \return        Copy of this object
    //!
    SdObjectPtr       copy( SdCopyMap &copyMap, bool next ) const;



    //!
    //! \brief copyClass Template which is exactly as copy but on finish make conversion to target class
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //! \return          Copy of this object
    //!
    template <typename SdObjectClass>
    SdObjectClass    *copyClass( SdCopyMap &copyMap, bool next ) const
      {
      SdObjectClass *obj = dynamic_cast<SdObjectClass*>( copy( copyMap, next ) );
      Q_ASSERT( obj != nullptr );
      return obj;
      }


    //!
    //! \brief cloneFrom Clone contents object except mParent field. This must be overrided in all subclasses to
    //!                  generate right copy of object
    //!                  Cloned object has no parent
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void      cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next );



    //Write and read object

    //!
    //! \brief json Overloaded function to write object content into json writer
    //! \param js   Json writer
    //!
    virtual void      json( SdJsonWriter &js ) const;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //! \param js   Json reader
    //!
    virtual void      json( const SdJsonReader &js );

    //Convert object to-from json representation
    QJsonObject       jsonObjectTo() const;
    static  SdObject* jsonObjectFrom( const QJsonObject obj );

    //Convert object to-from json text representation
    QByteArray        jsonTextTo() const;
    static SdObject*  jsonTextFrom( const QByteArray &ar );

    //Convert object to-from binary cbor format
    QByteArray        jsonCborTo() const;
    static SdObject*  jsonCborFrom( const QByteArray &ar );

    //Convert object to-from binary cbor compressed format
    QByteArray        jsonCborCompressedTo() const;
    static SdObject*  jsonCborCompressedFrom( const QByteArray &ar );

    //Read-write to-from file
    bool              fileJsonSave( const QString fname ) const;
    static SdObject*  fileJsonLoad( const QString fname );

    //Write and read object pointer which enable save and restore interobject referencing
    static  void      writePtr( const SdObject *ptr, SdJsonWriter &js );
    static  SdObject* readPtr( const SdJsonReader &js );

    static  SdObject* build( QString type );
    static  SdObject* buildFromJson( const SdJsonReader &js );


  };


//Convert object only to desired type, other way - deleted
template <class T>
T* sdObjectOnly( SdObject *obj ) {
  T *t = dynamic_cast<T*>( obj );
  if( t ) return t;
  if( obj != nullptr )
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





//!
//! \brief The SdPtrUnique class Helper template for exclusive owning of object
//!
//! Usage
//!   SdPtrUnique<int> ptrToInt;
template <class objectType>
class SdPtrUnique {
    objectType *mPtr;
  public:
    SdPtrUnique( objectType *p = nullptr ) noexcept : mPtr( p ) {  }

    SdPtrUnique( SdPtrUnique<objectType> &&pu ) noexcept : mPtr( pu.release() ) {}

    template <class baseType>
    SdPtrUnique( baseType *obj = nullptr ) noexcept : mPtr( dynamic_cast<objectType*>(obj) ) { if( mPtr == nullptr && obj != nullptr ) delete obj; }

    template <class baseType>
    SdPtrUnique( SdPtrUnique<baseType> &&pu ) noexcept : mPtr( nullptr ) { baseType *bp = pu.release(); mPtr = dynamic_cast<objectType*>(bp); if( mPtr == nullptr && bp != nullptr ) delete bp; }

    ~SdPtrUnique() { if( mPtr != nullptr ) delete mPtr; }


    void        reset( objectType *p ) { clear(); mPtr = p; }

    template<class baseObject>
    void        reset( baseObject *obj ) { clear(); mPtr = dynamic_cast<const objectType*>(obj); if( mPtr == nullptr && obj != nullptr ) delete obj; }

    void        clear() { if( mPtr != nullptr ) delete mPtr; mPtr = nullptr; }

    objectType *release() { objectType *p = mPtr; mPtr = nullptr; return p; }

    bool        isValid() const { return mPtr != nullptr; }

    void        swap( SdPtrUnique &pu ) { objectType *p = mPtr; mPtr = pu.mPtr; pu.mPtr = p; }

    operator bool () const { return mPtr != nullptr; }

    SdPtrUnique &operator = ( SdPtrUnique &&pu ) { reset( pu.release() ); return *this; }

    template<class baseType>
    SdPtrUnique &operator = ( SdPtrUnique<baseType> &&pu ) { reset( pu.release() ); return *this; }

    const objectType *operator -> () { return mPtr; }

    const objectType *ptr() const { return mPtr; }

    // Disable copy from lvalue.
    SdPtrUnique(const SdPtrUnique&) = delete;
    SdPtrUnique& operator=(const SdPtrUnique&) = delete;

  };


template <class objectType>
class SdPtrShared {
    objectType *mPtr;
    int        *mRefCount;
  public:
    SdPtrShared( objectType *p ) noexcept : mPtr(p), mRefCount(nullptr) { initRefCount(); }

    SdPtrShared( SdPtrShared &p ) noexcept : mPtr(p.mPtr), mRefCount(nullptr) { incRefCount(p.mRefCount); }

    SdPtrShared( SdPtrUnique<objectType> &&p ) noexcept : mPtr( p.release() ), mRefCount(nullptr) { initRefCount(); }

    template<class baseType>
    SdPtrShared( baseType *p ) noexcept : mPtr( dynamic_cast<objectType*>(p) ), mRefCount(nullptr) { initRefCount(); }

    template<class baseType>
    SdPtrShared( SdPtrShared<baseType> &p ) noexcept : mPtr( dynamic_cast<objectType*>(p.mPtr) ), mRefCount(nullptr) { incRefCount(p.mRefCount); }

    template<class baseType>
    SdPtrShared( SdPtrUnique<baseType> &p ) noexcept : mPtr( nullptr ), mRefCount(nullptr) { reset<baseType>( p.release() ); }

    ~SdPtrShared() { clear(); }

    void        reset( objectType *p ) { clear(); mPtr = p; initRefCount(); }

    template<class baseObject>
    void        reset( baseObject *obj ) { clear(); mPtr = dynamic_cast<const objectType*>(obj); if( mPtr == nullptr && obj != nullptr ) delete obj; initRefCount(); }

    void        reset( SdPtrShared &p ) { clear(); mPtr = p.mPtr; incRefCount( p.mRefCount ); }

    template<class baseObject>
    void        reset( SdPtrShared<baseObject> &p ) { clear(); mPtr = dynamic_cast<const objectType*>(p.mPtr); incRefCount( p.mRefCount ); }

    void        clear() { if( mRefCount != nullptr ) { (*mRefCount)--; if( *mRefCount == 0 ) { delete mPtr; delete mRefCount;} } mRefCount = nullptr; mPtr = nullptr;  }

    //objectType *release() { objectType *p = mPtr; mPtr = nullptr; return p; }

    bool        isValid() const { return mPtr != nullptr; }

    //void        swap( SdPtrUnique &pu ) { objectType *p = mPtr; mPtr = pu.mPtr; pu.mPtr = p; }

    operator bool () const { return mPtr != nullptr; }

    SdPtrShared &operator = ( SdPtrShared &&pu ) { reset( pu ); return *this; }

    SdPtrShared &operator = ( SdPtrShared &pu ) { reset( pu ); return *this; }

    template<class baseType>
    SdPtrShared &operator = ( SdPtrShared<baseType> &&pu ) { reset( pu ); return *this; }

    template<class baseType>
    SdPtrShared &operator = ( SdPtrShared<baseType> &pu ) { reset( pu ); return *this; }

    objectType *operator -> () const { return mPtr; }

    objectType *ptr() const { return mPtr; }

  private:
    void  initRefCount() { if( mPtr != nullptr ) { mRefCount = new int; *mRefCount = 1; }  }

    void  incRefCount( int *refCount ) { if( mPtr != nullptr ) { mRefCount = refCount; if( mRefCount != nullptr ) (*mRefCount)++; } }
  };


template <class objectType>
class SdPtrUniqueOnly {
    objectType *mPtr;
  public:
    template <class baseType>
    SdPtrUniqueOnly( baseType *obj = nullptr ) noexcept : mPtr( dynamic_cast<objectType*>(obj) ) { if( mPtr == nullptr && obj != nullptr ) delete obj; }

    template <class baseType>
    SdPtrUniqueOnly( SdPtrUnique<baseType> &&pu ) noexcept : mPtr( nullptr ) { baseType *bp = pu.release(); mPtr = dynamic_cast<objectType*>(bp); if( mPtr == nullptr && bp != nullptr ) delete bp; }

    ~SdPtrUniqueOnly() { if( mPtr != nullptr ) delete mPtr; }



    void        reset( const SdObject *obj ) { clear(); mPtr = dynamic_cast<const objectType*>(obj); if( mPtr == nullptr && obj != nullptr ) delete obj; }

    void        clear() { if( mPtr != nullptr ) delete mPtr; mPtr = nullptr; }

    objectType *release() { objectType *p = mPtr; mPtr = nullptr; return p; }

    bool        isValid() const { return mPtr != nullptr; }

    operator bool () const { return mPtr != nullptr; }

    const objectType *operator -> () { return mPtr; }

    const objectType *ptr() const { return mPtr; }

  };


#endif // SDOBJECT_H
