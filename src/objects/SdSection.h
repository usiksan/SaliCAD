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
    //Set new symbol id or update previously setuped
    void             setSymbolId( const QString id, SdUndo *undo );
    SdPItemSymbol   *extractFromFactory() const;
    void             setPinNumber( const QString name, const QString number, SdUndo *undo );

    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual quint64 getClass() const override;
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
  };

typedef SdSection *SdSectionPtr;

#endif // SDSECTION_H
