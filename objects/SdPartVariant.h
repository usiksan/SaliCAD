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
    QString           mPartTitle;      //Part for this variant
    QString           mPartId;
    bool              mDefault;        //True if default part
  public:
    int               mVisualIndex;    //Index in visual list. This is temporary element and not saved
    SdPartVariant();

    bool         isDefault() const { return mDefault; }
    void         setDefault( bool def ) { mDefault = def; }
    SdPItemPart *extractFromFactory( bool soft, QWidget *parent ) const;
    QString      getPartTitle() const { return mPartTitle; }
    QString      getPartId() const { return mPartId; }
    QString      getTitle() const;
    void         setPartId( const QString id, SdUndo *undo );


    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual quint64 getClass() const override;
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;
  };

#endif // SDPARTVARIANT_H
