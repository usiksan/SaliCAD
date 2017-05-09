/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Component contains symbol sections with pin (name-number) assotiation and component part
*/
#ifndef SDPITEMCOMPONENT_H
#define SDPITEMCOMPONENT_H

#include "SdProjectItem.h"

#define SD_TYPE_COMPONENT "Comp"

class SdSection;
class SdPartVariant;
class SdPItemSymbol;
class SdPItemPart;
class QWidget;

class SdPItemComponent : public SdProjectItem
  {
  public:
    SdPItemComponent();

    QString getSectionShortId( int sectionIndex ) const;
    QString getDefaultPartShortId() const;
    QString getPartShortId( int partIndex ) const;

    SdSection *getSection( int sectionIndex ) const;
    SdPartVariant *getPart( int partIndex ) const;
    SdPartVariant *getDefaultPart() const;

    SdPItemSymbol *extractSymbolFromFactory( int sectionIndex, bool soft, QWidget *parent ) const;
    SdPItemPart   *extractPartFromFactory(int partIndex, bool soft, QWidget *parent ) const;
    SdPItemPart   *extractDefaultPartFromFacory( bool soft, QWidget *parent ) const;
    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual quint64 getClass() const override;

    // SdProjectItem interface
  public:
    virtual QString getIconName() const override;
  };

#endif // SDPITEMCOMPONENT_H
