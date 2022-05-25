/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Pin-to-pad association table map as object. It allow to save it in library
  and retrive from external library for share using.
*/
#ifndef SDPADASSOCIATION_H
#define SDPADASSOCIATION_H

#include "SdObject.h"
#include "SdPad.h"


class SdContext;


#define SD_TYPE_PAD_ASSOCIATION "PadAssociation"



class SdPadAssociation : public SdObject
  {
    SdPadMap mMap;    //!< Pin-to-pad association table map
    QString  mName;   //!< Name of association
    QString  mAuthor; //!< Association author
  public:
    SdPadAssociation();
    SdPadAssociation(const SdPadMap map, const QString name , const QString author);

    QString      getUid() const;

    SdPadMap     getMap() const { return mMap; }

    QString      getName() const { return mName; }

    void         setMap( const QString name, const SdPadMap &src, SdUndo *undo );

    bool         contains( const QString pinType ) const { return mMap.contains(pinType); }

    const SdPad  pin( const QString pinType ) const { return mMap[pinType]; }

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

#endif // SDPADASSOCIATION_H
