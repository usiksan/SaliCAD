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
#define dctLines          0x00000001ul
#define dctText           0x00000002ul
#define dctSymPin         0x00000004ul
#define dctPartPin        0x00000008ul
#define dctIdent          0x00000010ul
#define dctSymbol         0x00000020ul
#define dctSymImp         0x00000040ul
#define dctPart           0x00000080ul
#define dctPartImp        0x00000100ul
#define dctNetWire        0x00000200ul
#define dctNetName        0x00000400ul
#define dctSheet          0x00000800ul
#define dctPlate          0x00001000ul
#define dctComponent      0x00002000ul
#define dctProject        0x00004000ul
#define dctSection        0x00008000ul
#define dctPartVariant    0x00010000ul
#define dctArea           0x00020000ul
#define dctSelector       0x00040000ul
#define dctPadAssociation 0x00080000ul
#define dctTraceRoad      0x00100000ul
#define dctTraceVia       0x00200000ul
#define dctTextDoc        0x00400000ul

#define dctLocal          0x80000000ul //This flag setup for objects which must not be send to global storage


#define dctPicture       (dctLines | dctText)
#define dctProjectItems  (dctSymbol | dctPart | dctSheet | dctPlate | dctComponent)
#define dctAll           MAX64_MASK
#define dctSymbolObjects (dctPicture | dctSymPin )
#define dctPartObjects   (dctPicture | dctPartPin)
#define dctSheetObjects  (dctPicture | dctSymImp | dctNetWire | dctNetName)
#define dctPlateObjects  (dctPicture | dctPartImp | dctTraceVia | dctTraceRoad )
#define dctTraced        (dctPartImp | dctTraceVia | dctTraceRoad )


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
            QString   getId() const;
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
