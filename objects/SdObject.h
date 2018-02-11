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
#define dctLines         0x00000001l
#define dctText          0x00000002l
#define dctSymPin        0x00000004l
//#define dctSymPinName    0x00000008l
//#define dctSymPinNumber  0x00000010l
#define dctIdent         0x00000020l
#define dctSymbol        0x00000040l
#define dctSymImp        0x00000080l
#define dctPartPin       0x00000100l
#define dctRoadPin       0x00000200l
//#define dctPartPinNumber 0x00000400l
#define dctPart          0x00000800l
#define dctPartImp       0x00001000l
#define dctWire          0x00002000l
#define dctWireName      0x00004000l
#define dctSheet         0x00008000l
#define dctPlate         0x00010000l
#define dctList          0x00020000l
#define dctAlias         0x00040000l
#define dctPoligon       0x00080000l
#define dctVia           0x00100000l
#define dctComponent     0x00200000l
  //dctUser          = 0x200000,
#define dctProject       0x00400000l
#define dctSize          0x00800000l
#define dctSizeProp      0x01000000l
#define dctTextDoc       0x02000000l
#define dctSelector      0x04000000l
#define dctSheetNet      0x08000000l
#define dctPlateNet      0x10000000l
#define dctSection       0x20000000l
#define dctPartVariant   0x40000000l
#define dctArea          0x80000000l
  //dctChars    = 0x20000000,
  //dctPrjList  = 0x40000000,
  //dctData     = 0x80000000,
#define dctPicture       (dctLines | dctText | dctSize)
#define dctProjectItems  (dctSymbol | dctPart | dctSheet | dctPlate | dctComponent)
#define dctAll           MAX64_MASK
#define dctSymbolObjects (dctPicture | dctSymPin | dctSection | dctPartVariant)
#define dctPartObjects   (dctPicture | dctPartPin)
#define dctSheetObjects  (dctPicture | dctSymImp | dctSheetNet | dctWire | dctWireName)
#define dctPlateObjects  (dctPicture | dctPartImp | dctVia )


#define SDKO_TYPE      "type"
#define SDKO_ID        "id"

class SdContainer;
class SdProjectItem;

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
    virtual bool      isContainer() const { return false; }

    //Hierarhy
    SdContainer*      getParent() const { return mParent; }
    SdContainer*      getRoot() const;
    void              setParent( SdContainer *parent );

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
    virtual void      upgradeProjectItem( SdProjectItem *newItem, SdUndo *undo );

    //Copy
    //Exact copy object
    SdObject*         copy() const;
    //Copy logic next object
    virtual SdObject* copyNext() const { return copy(); }

    //Clone contens object except mParent field.
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
T* only( SdObject *obj ) {
  T *t = dynamic_cast<T*>( obj );
  if( t ) return t;
  delete obj;
  return 0;
  }

#endif // SDOBJECT_H
