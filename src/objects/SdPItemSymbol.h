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

#include "SdPItemVariant.h"

class SdPItemSymbol;


#define SD_TYPE_SYMBOL "Symbol"

class SdPItemSymbol : public SdPItemVariant
  {
  public:
    SdPItemSymbol();


    virtual QString   getType() const override;
    virtual quint64   getClass() const override;
    virtual QString   getIconName() const override;
    virtual quint64   getAcceptedObjectsMask() const override;
    //virtual SdGraph  *insertCopyObject( const SdGraph *obj, SdPoint offset, SdUndo *undo, SdWEditorGraph *editor, bool next ) override;
    //virtual void      insertObjects( SdPoint offset, SdSelector *sel, SdUndo *undo, SdWEditorGraph *editor, SdSelector *dest, bool next ) override;

    // SdPItemVariant interface
  public:
    //!
    //! \brief sectionIsAvailable Return true if section with sectionIndex available
    //! \param sectionIndex       Section index
    //! \return                   true if section with sectionIndex available
    //!
    virtual bool          sectionIsAvailable(int sectionIndex) const override;

    //!
    //! \brief sectionCount Returns section count
    //! \return                Section count
    //!
    virtual int           sectionCount() const override;

    //!
    //! \brief sectionPinNumberGet Return individual pin number for desired pin name for section
    //! \param sectionIndex        Section index
    //! \param pinName             Pin name which number must be retrived
    //! \return                    Pin number for pin name of section with index sectionIndex
    //!
    virtual QString        sectionPinNumberGet(int sectionIndex, const QString pinName) const override;

    //!
    //! \brief extractSymbolFromFactory Return symbol from section by index
    //! \param sectionIndex             Section index
    //! \return                         Symbol extracted from factory
    //!
    virtual SdPItemSymbol *extractSymbolFromFactory(int sectionIndex) const override;
  };

#endif // SDPITEMSYMBOL_H
