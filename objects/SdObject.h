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

class SdObject
  {
  public:
    SdObject();

    //Information
    int               getId() const { return (int)(this); }

    virtual int       getType() = 0;


    //Copy
    //Exact copy object
    virtual SdObject* copy() = 0;
    //Copy logic next object
    virtual SdObject* copyNext() { return copy(); }

    //Write and read object
    virtual void      writeObject( QJsonObject &obj ) = 0;
    QJsonObject       write();
    QJsonObject       writePtr();

    virtual void      readObject( SdObjectMap *map, const QJsonObject obj ) = 0;

    static  SdObject* read( SdObjectMap *map, const QJsonObject obj );
    static  SdObject* readPtr( SdObjectMap *map, const QJsonObject obj );
    static  SdObject* build( int type );
  };

#endif // SDOBJECT_H
