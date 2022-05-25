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
    void         setPartId( const QString id, SdUndo *undo );


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

typedef SdPartVariant *SdPartVariantPtr;

#endif // SDPARTVARIANT_H
