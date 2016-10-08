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

#define SdtObject      1
#define SdtSymbol      50
#define SdtPart        51
#define SdtSheet       52
#define SdtPlate       53
#define SdtProject     54


#define SDKO_TYPE      "type"
#define SDKO_ID        "id"

class SdOwner;

class SdObject
  {
    SdOwner *mParent;
  public:
    SdObject();

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

#endif // SDOBJECT_H
