/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Symbol section is one of more schematic sections where all section
  declare full component.
*/
#ifndef SDSECTION_H
#define SDSECTION_H

#include "SdObject.h"
#include "library/SdStringMap.h"

#include <QMap>
#include <QList>

//Pin association where pin name assotiated with pin number
//Pin assotiation table where symbol pin name is key and part pin number is value
typedef SdStringMap SdPinAssociation;

class SdPItemSymbol;
class QWidget;

#define SD_TYPE_SECTION "Section"

class SdSection : public SdObject
  {
    QString           mSymbolId;          //Symbolic presentation of this section
    QString           mSymbolTitle;       //Symbolic title
    SdPinAssociation  mAssociationTable;  //Pin assotiation table
  public:
    SdSection();

    QString          getPinNumber( const QString name ) const { return mAssociationTable.value(name); }
    QString          getSymbolTitle() const { return mSymbolTitle; }
    QString          getSymbolId() const { return mSymbolId; }
    SdPinAssociation getPins() const { return mAssociationTable; }
    void             setSymbolId( const QString id, SdUndo *undo );
    SdPItemSymbol   *extractFromFactory(bool soft, QWidget *parent ) const;
    void             setPinNumber( const QString name, const QString number, SdUndo *undo );

    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual quint64 getClass() const override;
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;
  };

typedef SdSection *SdSectionPtr;

#endif // SDSECTION_H
