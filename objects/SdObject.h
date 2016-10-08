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
#include <QJsonObject>

//Классы объектов
const int
  dctLines    = 0x0001,
  dctText     = 0x0002,
  dctSymPin   = 0x0004,
  dctSPName   = 0x0008,
  dctSPNumb   = 0x0010,
  dctIdent    = 0x0020,
  dctSymbol   = 0x0040,
  dctSymImp   = 0x0080,
  dctPrtPin   = 0x0100,
  dctPPName   = 0x0200,
  dctPPNumb   = 0x0400,
  dctPart     = 0x0800,
  dctPartImp  = 0x1000,
  dctWire     = 0x2000,
  dctWName    = 0x4000,
  dctSheet    = 0x8000,
  dctPlate    = 0x10000,
  dctList     = 0x20000,
  dctAlias    = 0x40000,
  dctPoligon  = 0x80000,
  dctVia      = 0x100000,
  dctUser     = 0x200000,
  dctProject  = 0x400000,
  dctSize     = 0x800000,
  dctSizePrp  = 0x1000000,
  dctTextDoc  = 0x2000000,
  dctSelector = 0x4000000,
  dctSheetNet = 0x8000000,
  dctPlateNet = 0x10000000,
  dctChars    = 0x20000000,
  dctPrjList  = 0x40000000,
  dctData     = 0x80000000,
  dctPicture  = dctLines | dctText | dctSize;


#define SDKO_TYPE      "type"
#define SDKO_ID        "id"

class SdOwner;

class SdObject
  {
    SdOwner *mParent;
  public:
    SdObject();
    virtual ~SdObject() {}

    //Information
    int               getId() const { return (int)(this); }
    virtual QString   getType() const = 0;
    virtual int       getClass() const = 0;

    //Hierarhy
    SdOwner*          getParent() const { return mParent; }
    SdOwner*          getRoot() const;
    void              setParent( SdOwner *parent ) { mParent = parent; }


    //Copy
    //Exact copy object
    virtual SdObject* copy() = 0;
    //Copy logic next object
    virtual SdObject* copyNext() { return copy(); }

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


template <class T>
T* only( SdObject *obj ) {
  T *t = dynamic_cast<T*>( obj );
  if( t ) return t;
  delete obj;
  return 0;
  }

#endif // SDOBJECT_H
