/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Class present schematic net wire part: wire segment or wire name
*/
#ifndef SDGRAPHWIRE_H
#define SDGRAPHWIRE_H

#include "SdGraph.h"
#include "SdPItemSheet.h"
#include "objects/SdPvString.h"


class SdGraphNet : public SdGraph
  {
  protected:
    SdPvString mNetName;
  public:
    SdGraphNet();
    SdGraphNet( const QString netName );

    SdPItemSheet *getSheet() const { return dynamic_cast<SdPItemSheet*>(getParent()); }

    SdPvString    netName() const { return mNetName; }

    QString       getNetName() const { return mNetName.string(); }

    virtual void  setNetName( const QString netName, SdUndo *undo );

    // SdObject interface
  public:
    //!
    //! \brief cloneFrom Overrided function. We copy object from source
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;

    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    virtual void json( SdJsonWriter &js ) const override;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    virtual void json( const SdJsonReader &js ) override;

    // SdGraph interface
  public:
    virtual void setProp(SdPropSelected &prop) override;
    virtual void getProp(SdPropSelected &prop) override;
    virtual void saveState(SdUndo *undo) override;
  };


#endif // SDGRAPHWIRE_H
