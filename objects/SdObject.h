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
#include <QJsonObject>

//Классы объектов
const quint64
  dctLines         = 0x00000001l,
  dctText          = 0x00000002l,
  dctSymPin        = 0x00000004l,
  dctSymPinName    = 0x00000008l,
  dctSymPinNumber  = 0x00000010l,
  dctIdent         = 0x00000020l,
  dctSymbol        = 0x00000040l,
  dctSymImp        = 0x00000080l,
  dctPartPin       = 0x00000100l,
  dctPartPinName   = 0x00000200l,
  dctPartPinNumber = 0x00000400l,
  dctPart          = 0x00000800l,
  dctPartImp       = 0x00001000l,
  dctWire          = 0x00002000l,
  dctWireName      = 0x00004000l,
  dctSheet         = 0x00008000l,
  dctPlate         = 0x00010000l,
  dctList          = 0x00020000l,
  dctAlias         = 0x00040000l,
  dctPoligon       = 0x00080000l,
  dctVia           = 0x00100000l,
  dctComponent     = 0x00200000l,
  //dctUser          = 0x200000,
  dctProject       = 0x00400000l,
  dctSize          = 0x00800000l,
  dctSizeProp      = 0x01000000l,
  dctTextDoc       = 0x02000000l,
  dctSelector      = 0x04000000l,
  dctSheetNet      = 0x08000000l,
  dctPlateNet      = 0x10000000l,
  //dctChars    = 0x20000000,
  //dctPrjList  = 0x40000000,
  //dctData     = 0x80000000,
  dctPicture  = dctLines | dctText | dctSize,
  dctProjectItems = dctSymbol | dctPart | dctSheet | dctPlate | dctComponent;


#define SDKO_TYPE      "type"
#define SDKO_ID        "id"

class SdContainer;

typedef QString SdId;

class SdObject
  {
    SdContainer *mParent;  //Parent object
    bool         mDeleted; //Flag check if object deleted. Deleted objects not save
  public:
    SdObject();
    virtual ~SdObject() {}

    //Information
    virtual QString   getId() const;
    virtual QString   getType() const = 0;
    virtual quint64   getClass() const = 0;

    //Hierarhy
    SdContainer*      getParent() const { return mParent; }
    SdContainer*      getRoot() const;
    void              setParent( SdContainer *parent ) { mParent = parent; }
    SdUndo*           getUndo();

    //Attach and detach
    virtual void      attach() {}
    virtual void      undoAttach() {}
    virtual void      detach() {}
    virtual void      undoDetach() {}
    bool              isDeleted() const { return mDeleted; }
    void              markDeleted( bool deleted ) { mDeleted = deleted; }

    //Copy
    //Exact copy object
    SdObject*         copy();
    //Copy logic next object
    virtual SdObject* copyNext() { return copy(); }
    //Clone contens object
    virtual void      cloneFrom( const SdObject *src );

    //Write and read object
    virtual void      writeObject( QJsonObject &obj ) const = 0;
    QJsonObject       write() const;
    static  void      writePtr( const SdObject *ptr, const QString name, QJsonObject &obj );

    virtual void      readObject( SdObjectMap *map, const QJsonObject obj );

    static  SdObject* read( SdObjectMap *map, const QJsonObject obj );
    static  SdObject* readPtr( SdObjectMap *map, const QJsonObject obj );
    static  SdObject* readPtr( const QString name, SdObjectMap *map, const QJsonObject obj );
    static  SdObject* build( QString type );
  };

typedef SdObject *SdObjectPtr;


template <class T>
T* only( SdObject *obj ) {
  T *t = dynamic_cast<T*>( obj );
  if( t ) return t;
  delete obj;
  return 0;
  }

#endif // SDOBJECT_H
