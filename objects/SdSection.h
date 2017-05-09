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
#include <QList>

//Pin assotiation where pin name assotiated with pin number
//struct SdPinAssotiation
//  {
//    QString mPinName;
//    QString mPinNumber;
//  };

//Pin assotiation table where symbol pin name is key and part pin number is value
typedef QMap<QString,QString> SdPinAssotiation;

class SdPItemSymbol;
class QWidget;

#define SD_TYPE_SECTION "Section"

class SdSection : public SdObject
  {
    QString           mSymbolTitle;       //Symbolic presentation of this section
    QString           mSymbolAuthor;
    SdPinAssotiation  mAssotiationTable; //Pin assotiation table
  public:
    SdSection();

    QString          getPinNumber( const QString name ) const { return mAssotiationTable.value(name); }
    QString          getSymbolShortId() const { return mSymbolTitle + mSymbolAuthor; }
    QString          getSymbolTitle() const { return mSymbolTitle; }
    QString          getSymbolAuthor() const { return mSymbolAuthor; }
    QString          getTitle() const;
    SdPinAssotiation getPins() const { return mAssotiationTable; }
    void             updateFromSymbol( SdPItemSymbol *symbol );
    SdPItemSymbol   *extractFromFactory(bool soft, QWidget *parent ) const;

    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual quint64 getClass() const override;
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;
  };

#endif // SDSECTION_H
