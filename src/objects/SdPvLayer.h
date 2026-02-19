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
#ifndef SDPVLAYER_H
#define SDPVLAYER_H

#include "SdLayer.h"
#include "SdJsonIO.h"
#include "objects/SdPv.h"

#include <QString>

//!
//! \brief The SdPvLayer class - Property class for managing PCB layer references
//!        Handles layer visibility, editing state, and JSON serialization
//!
class SdPvLayer : public SdPv<SdLayerPtr,SdPvLayer>
  {
  public:
    //!
    //! \brief SdPvLayer Constructor with layer identifier
    //! \param id Layer identifier string to find and set the layer
    //!
    SdPvLayer( QString id = QString{} );

    SdPvLayer( SdLayer *layer );

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
    void       json( const QString name, SvJsonWriter &js ) const { js.jsonString( name, mValue->id() ); }

    //!
    //! \brief json Read layer property from JSON reader
    //! \param name JSON key name
    //! \param js   JSON reader object
    //!
    void       json( const QString name, const SdJsonReader &js );

  };


#endif // SDPVLAYER_H
