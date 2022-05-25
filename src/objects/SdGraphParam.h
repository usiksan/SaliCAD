/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base class for child objects with param map
*/
#ifndef SDGRAPHPARAM_H
#define SDGRAPHPARAM_H

#include "SdGraph.h"
#include "library/SdStringMap.h"


class SdGraphParam : public SdGraph
  {
  protected:
    SdStringMap       mParamTable;   //!< Parameters
  public:
    SdGraphParam();
    SdGraphParam( const SdStringMap &param );

    //Params with local param table
    //

    //!
    //! \brief paramContains Test if param present in local table
    //! \param key           Key of param
    //! \return              true if param present in local table
    //!
    bool              paramContains( const QString key ) const { return mParamTable.contains(key); }

    //!
    //! \brief paramGet Retrive param from local table
    //! \param key      Key of param
    //! \return         Value of param with key
    //!
    QString           paramGet( const QString key ) const { return mParamTable.value(key); }

    //!
    //! \brief paramSet Set single param value in local table
    //! \param key      Key of param
    //! \param val      Value of param
    //!
    void              paramSet( const QString key, const QString val ) { mParamTable.insert( key, val ); }

    //!
    //! \brief paramTable Return full local param table
    //! \return           Full local param table
    //!
    SdStringMap       paramTable() const { return mParamTable; }

    //!
    //! \brief paramTableSet Setup full param table
    //! \param map           New param table
    //! \param undo          Undo archive
    //!
    void              paramTableSet( const SdStringMap map, SdUndo *undo );

    // SdObject interface
  public:
    //!
    //! \brief cloneFrom Overrided function. We copy object from source
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void      cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;

    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    virtual void      json( SdJsonWriter &js ) const override;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    virtual void      json( const SdJsonReader &js ) override;
  };

#endif // SDGRAPHPARAM_H
