/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDSECTION_H
#define SDSECTION_H

#include "SdObject.h"

#include <QMap>

//Pin assotiation table where symbol pin name is key and part pin number is value
typedef QMap<QString,QString> SdPinAssotiation;

class SdPItemSymbol;

#define SD_TYPE_SECTION "Section"

class SdSection : public SdObject
  {
    SdPItemSymbol    *mSymbol;           //Symbolic presentation of this section
    SdPinAssotiation  mAssotiationTable; //Pin assotiation table
  public:
    SdSection();

    QString getPinNumber( const QString name ) const { return mAssotiationTable.value(name); }

    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual quint64 getClass() const override;
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;
  };

#endif // SDSECTION_H
