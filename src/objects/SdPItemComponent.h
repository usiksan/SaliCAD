/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Component is symbol without graph bun contains symbol sections with pin (name-number) assotiation and component part
*/
#ifndef SDPITEMCOMPONENT_H
#define SDPITEMCOMPONENT_H

#include "SdPItemVariant.h"
#include "SdPartVariant.h"
#include "SdSection.h"

#define SD_TYPE_COMPONENT "Comp"


class SdPItemComponent : public SdPItemVariant
  {
  public:
    SdPItemComponent();

    //Part
    //Part title for visual presentation
    QString          getPartTitle() const;
    //Part id
    QString          getPartId() const;
    //Setup new part id
    void             setPartId( const QString id, SdUndo *undo );
    //Return part descripted part variant
    SdPItemPart     *extractPartFromFactory( bool soft, QWidget *parent ) const;
    //Return default part
    SdPartVariant   *getPart() const;

    //Sections
    //Return section count
    int              getSectionCount() const;
    //Append section with symbol id. May be empty
    void             appendSection( const QString id, SdUndo *undo );
    //Section symbol title for visual presentation
    QString          getSectionSymbolTitle( int sectionIndex ) const;
    //Section symbol id
    QString          getSectionSymbolId( int sectionIndex ) const;
    //Setup new section symbol id
    void             setSectionSymbolId( const QString id, int sectionIndex, SdUndo *undo );
    //Return section by index
    SdSection       *getSection( int sectionIndex ) const;
    //Return symbol from section by index
    SdPItemSymbol   *extractSymbolFromFactory( int sectionIndex, bool soft, QWidget *parent ) const;
    //Remove section
    void             removeSection( int sectionIndex, SdUndo *undo );

    //Pins in sections
    //Return full section pin assotiation table
    SdPinAssociation getSectionPins( int sectionIndex ) const;
    //Return individual pin number for desired pin name for section
    QString          getSectionPinNumber( int sectionIndex, const QString pinName );
    //Setup new pin number for desired pin name for section
    void             setSectionPinNumber( int sectionIndex, const QString pinName, const QString pinNumber, SdUndo *undo );


    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual quint64 getClass() const override;

    // SdProjectItem interface
  public:
    virtual QString getIconName() const override;
    virtual quint64 getAcceptedObjectsMask() const override { return dctPartVariant | dctSection; }
  };


//Create default component with single section with given symbol
SdPItemComponent *sdCreateDefaultComponent( SdPItemSymbol *symbol, bool appendDefaultPart );

#endif // SDPITEMCOMPONENT_H
