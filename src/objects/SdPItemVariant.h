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
#include "SdPartVariant.h"
#include <QStringList>

#define SD_TYPE_VARIANT "Variant"

class SdPItemSymbol;

class SdPItemVariant : public SdProjectItem
  {
    qint32      mVariantFieldCount;
    QStringList mVariantTable;
  public:
    SdPItemVariant();

    //To variant table access
    void variantTableGet( qint32 &fieldCount, QStringList &list ) const;
    void variantTableSet( qint32 fieldCount, QStringList list, SdUndo *undo );

    //!
    //! \brief isFieldPresent Test if field name present in variant table
    //! \param fieldName      Field name for test
    //! \return               true if field with this name presents in variant table
    //!
    bool             isFieldPresent( const QString fieldName ) const;




    //Part
    //!
    //! \brief partIsPresent Returns true if contains one or more part variants
    //! \return              true if contains one or more part variants
    //!
    bool             partIsPresent() const;

    //!
    //! \brief partTitleGet Returns part title for visual presentation
    //! \param id           Id of part variant which title need to be returned. If empty then return primary part title
    //! \return             Part title for visual presentation
    //!
    QString          partTitleGet() const;

    //!
    //! \brief partIdGet Returns part id for primary part variant
    //! \return          Part id for primary part variant
    //!
    QString          partIdGet() const;

    //!
    //! \brief partIdSet Setup new part id for primary part variant
    //! \param id        New id for primary part
    //! \param undo
    //!
    void             partIdSet( const QString id, SdUndo *undo );

    //!
    //! \brief partExtractFromFactory Return part descripted part variant
    //! \return                       Part descripted part variant
    //!
    SdPItemPart     *partExtractFromFactory() const;

    //!
    //! \brief partGet Returns part variant for id. If id is empty then return primary part variant
    //! \return
    //!
    SdPartVariant   *partGet() const;

    //!
    //! \brief partRemove Removes part variant from part list
    //! \param undo
    //!
    void             partRemove( SdUndo *undo );


    //Symbol sections
    //!
    //! \brief sectionIsAvailable Return true if section with sectionIndex available
    //! \param sectionIndex       Section index
    //! \return                   true if section with sectionIndex available
    //!
    virtual bool           sectionIsAvailable( int sectionIndex ) const = 0;

    //!
    //! \brief sectionCount Returns section count
    //! \return                Section count
    //!
    virtual int            sectionCount() const = 0;

    //!
    //! \brief sectionSymbolTitleGet Returns section symbol title for visual presentation
    //! \param sectionIndex          Section index
    //! \return                      Section symbol title for visual presentation
    //!
    virtual QString        sectionSymbolTitleGet( int sectionIndex ) const = 0;

    //!
    //! \brief sectionSymbolIdGet Returns section symbol id
    //! \param sectionIndex       Section index
    //! \return                   Section symbol id
    //!
    virtual QString        sectionSymbolIdGet( int sectionIndex ) const = 0;


    //!
    //! \brief sectionPinNumberGet Return individual pin number for desired pin name for section
    //! \param sectionIndex        Section index
    //! \param pinName             Pin name which number must be retrived
    //! \return                    Pin number for pin name of section with index sectionIndex
    //!
    virtual QString        sectionPinNumberGet( int sectionIndex, const QString pinName ) const = 0;

    //!
    //! \brief extractSymbolFromFactory Return symbol from section by index
    //! \param sectionIndex             Section index
    //! \return                         Symbol extracted from factory
    //!
    virtual SdPItemSymbol *extractSymbolFromFactory(int sectionIndex) const = 0;


    // SdObject interface
  public:
    //!
    //! \brief cloneFrom Overrided function. We copy object from source
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void    cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;

    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    virtual void    json( SdJsonWriter &js ) const override;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    virtual void    json( const SdJsonReader &js ) override;

    // SdProjectItem interface
  public:
    virtual void    getHeader(SdLibraryHeader &hdr) const override;
  };

#endif // SDPITEMVARIANT_H
