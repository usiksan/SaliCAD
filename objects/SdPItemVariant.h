/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Variant contains table with parameters variant
  Table is square array with columns - parameters and with rows - values of parameters.
  This table contained in StringList, first element of it is fields names
*/
#ifndef SDPITEMVARIANT_H
#define SDPITEMVARIANT_H

#include "SdProjectItem.h"
#include <QStringList>

#define SD_TYPE_VARIANT "Variant"


class SdPItemVariant : public SdProjectItem
  {
    qint32      mVariantFieldCount;
    QStringList mVariantTable;
  public:
    SdPItemVariant();

    //To variant table access
    void variantTableGet( qint32 &fieldCount, QStringList &list ) const;
    void variantTableSet( qint32 fieldCount, QStringList list, SdUndo *undo );



    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual SdClass getClass() const override;
    virtual void cloneFrom(const SdObject *src) override;
    virtual void writeObject(QJsonObject &obj) const override;
    virtual void readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdProjectItem interface
  public:
    virtual void getHeader(SdLibraryHeader &hdr) const override;
    virtual QString getIconName() const override;
    virtual quint64 getAcceptedObjectsMask() const override;
  };

#endif // SDPITEMVARIANT_H
