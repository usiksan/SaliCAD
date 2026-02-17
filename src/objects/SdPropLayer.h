/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Layer property
*/
#ifndef SDPROPLAYER_H
#define SDPROPLAYER_H

#include "SdLayer.h"
#include "SdJsonIO.h"
#include <QString>

//!
//! \brief The SdPropLayer class - Property class for managing PCB layer references
//!        Handles layer visibility, editing state, and JSON serialization
//!
class SdPropLayer
  {
    SdLayer *mLayer;            //!< Pointer to the associated SdLayer object

  public:
    //!
    //! \brief SdPropLayer Default constructor - initializes with default layer
    //!
    SdPropLayer();

    //!
    //! \brief SdPropLayer Constructor with layer identifier
    //! \param id Layer identifier string to find and set the layer
    //!
    SdPropLayer( QString id );

    bool       operator != ( const SdPropLayer &v ) const { return mLayer != v.mLayer; }

    //!
    //! \brief isVisible Check if layer is visible
    //! \param otherSide If true, check paired layer for flipped components
    //! \return True if layer (or its pair) is visible
    //!
    bool       isVisible( bool otherSide = false ) const;

    //!
    //! \brief isEdited Check if layer is in edit mode
    //! \return True if layer is currently editable
    //!
    bool       isEdited() const;

    //!
    //! \brief set Set layer by identifier
    //! \param id Layer identifier string to find and set
    //!
    void       set( const QString id );

    //!
    //! \brief layer Get layer pointer
    //! \param otherSide If true, return paired layer for flipped components
    //! \return Pointer to SdLayer object
    //!
    SdLayer   *layer( bool otherSide = false ) const;

    //!
    //! \brief setLayerUsage Mark layer as used (sets usage flag)
    //!
    void       setLayerUsage() const;

    //!
    //! \brief json Write layer property to JSON writer
    //! \param name JSON key name
    //! \param js   JSON writer object
    //!
    void       json( const QString name, SvJsonWriter &js ) const { js.jsonString( name, mLayer->id() ); }

    //!
    //! \brief json Read layer property from JSON reader
    //! \param name JSON key name
    //! \param js   JSON reader object
    //!
    void       json( const QString name, const SdJsonReader &js );

    //!
    //! \brief swap Swap layer references with another SdPropLayer instance
    //! \param other Other layer property to swap with
    //!
    void       swap( SdPropLayer &other ) { qSwap( mLayer, other.mLayer ); }
  };


#endif // SDPROPLAYER_H
