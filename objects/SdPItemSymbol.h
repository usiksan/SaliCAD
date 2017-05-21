/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical symbol presentation
  Symbol also may contains symbol sections with pin (name-number) assotiation and component part
*/

#ifndef SDPITEMSYMBOL_H
#define SDPITEMSYMBOL_H

#include "SdProjectItem.h"

class SdPItemSymbol;
class SdPItemPart;
class QWidget;
class SdSection;
class SdPartVariant;


#define SD_TYPE_SYMBOL "Symbol"

class SdPItemSymbol : public SdProjectItem
  {
  public:
    SdPItemSymbol();

    //Return section count
    int            getSectionCount() const;
    //Return section by index
    SdSection     *getSection( int sectionIndex ) const;
    //Return part variant by index
    SdPartVariant *getPart( int partIndex ) const;
    //Return default part
    SdPartVariant *getDefaultPart() const;

    //Return symbol from section by index
    SdPItemSymbol *extractSymbolFromFactory( int sectionIndex, bool soft, QWidget *parent ) const;
    //Return part from part variant by index
    SdPItemPart   *extractPartFromFactory(int partIndex, bool soft, QWidget *parent ) const;
    //Return default part from default part variant
    SdPItemPart   *extractDefaultPartFromFacory( bool soft, QWidget *parent ) const;


    virtual QString   getType() const override;
    virtual quint64   getClass() const override;
    virtual QString   getIconName() const override;
    virtual void      cloneFrom( const SdObject *src ) override;

    // SdProjectItem interface
  public:
    virtual SdGraphIdent *createIdent() override;
  };

#endif // SDPITEMSYMBOL_H
