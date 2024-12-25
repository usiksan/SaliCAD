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
#include "SdSection.h"

#define SD_TYPE_COMPONENT "Comp"


class SdPItemComponent : public SdPItemVariant
  {
  public:
    SdPItemComponent();

    //Sections
    //!
    //! \brief sectionIsAvailable Return true if section with sectionIndex available
    //! \param sectionIndex       Section index
    //! \return                   true if section with sectionIndex available
    //!
    virtual bool           sectionIsAvailable( int sectionIndex ) const override;

    //!
    //! \brief sectionCount Returns section count
    //! \return                Section count
    //!
    virtual int            sectionCount() const override;

    //Append section with symbol id. May be empty
    void                   sectionAppend( const QString id, SdUndo *undo );

    //!
    //! \brief sectionSymbolTitleGet Returns section symbol title for visual presentation
    //! \param sectionIndex          Section index
    //! \return                      Section symbol title for visual presentation
    //!
    virtual QString        sectionSymbolTitleGet( int sectionIndex ) const override;

    //!
    //! \brief sectionSymbolIdGet Returns section symbol id
    //! \param sectionIndex       Section index
    //! \return                   Section symbol id
    //!
    virtual QString        sectionSymbolIdGet( int sectionIndex ) const override;

    //Setup new section symbol id
    void                   sectionSymbolIdSet( const QString id, int sectionIndex, SdUndo *undo );
    //Return section by index
    SdSection             *sectionGet( int sectionIndex ) const;

    //Return symbol from section by index
    virtual SdPItemSymbol *extractSymbolFromFactory(int sectionIndex) const override;

    //Remove section
    void                   sectionRemove( int sectionIndex, SdUndo *undo );


    //Pins in sections
    //Return full section pin assotiation table
    SdPinAssociation       sectionPinsGet( int sectionIndex ) const;

    //!
    //! \brief sectionPinNumberGet Return individual pin number for desired pin name for section
    //! \param sectionIndex        Section index
    //! \param pinName             Pin name which number must be retrived
    //! \return                    Pin number for pin name of section with index sectionIndex
    //!
    virtual QString        sectionPinNumberGet( int sectionIndex, const QString pinName ) const override;

    //Setup new pin number for desired pin name for section
    void                   sectionPinNumberSet( int sectionIndex, const QString pinName, const QString pinNumber, SdUndo *undo );


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
