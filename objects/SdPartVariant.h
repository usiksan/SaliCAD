/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part variant contains one part variant for component. Variants may be
  different component placement with the same part or different parts of one component.
*/
#ifndef SDPARTVARIANT_H
#define SDPARTVARIANT_H

#include "SdObject.h"

#define SD_TYPE_PART_VARIANT "PartVariant"

class SdPItemPart;
class QWidget;

class SdPartVariant : public SdObject
  {
    QString           mPartName;       //Part for this variant
    QString           mPartAuthor;
    bool              mDefault;        //True if default part
  public:
    SdPartVariant();

    bool         isDefault() const { return mDefault; }
    void         setDefault( bool def ) { mDefault = def; }
    QString      getPartShortId() const { return mPartName + mPartAuthor; }
    SdPItemPart *extractFromFactory( bool soft, QWidget *parent ) const;


    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual quint64 getClass() const override;
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;
  };

#endif // SDPARTVARIANT_H
