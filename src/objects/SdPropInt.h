/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Int property
*/

#ifndef SDINTPROP_H
#define SDINTPROP_H

#include "SvLib/SvJsonIO.h"

//!
//! \brief The SdPropInt struct - Integer property with conversion between logical and physical values
//!        Provides JSON serialization and physical value conversion (e.g., for distances, sizes)
//!
struct SdPropInt
  {
    int mValue; //!< Stored integer value in logical units

    SdPropInt() : mValue(0) {}

    //!
    //! \brief log2Phis Convert logical value to physical string representation
    //! \param ppm      Pixels per millimeter conversion factor
    //! \return         Physical value as formatted string (e.g., "10.5 mm")
    //!
    QString    log2Phis( double ppm ) const;


    //!
    //! \brief setFromPhis Set logical value from physical string representation
    //! \param src        Source physical string (e.g., "10.5 mm")
    //! \param ppm        Pixels per millimeter conversion factor
    //!
    void       setFromPhis( const QString src, double ppm );

    //!
    //! \brief json Write integer property to JSON writer
    //! \param key JSON key name
    //! \param js  JSON writer object
    //!
    void       json( const QString key, SvJsonWriter &js ) const { js.jsonInt( key, mValue ); }

    //!
    //! \brief json Read integer property from JSON reader
    //! \param key JSON key name
    //! \param js  JSON reader object
    //!
    void       json( const QString key, const SvJsonReader &js ) { js.jsonInt( key, mValue ); }

    //!
    //! \brief swap Swap values with another SdPropInt instance
    //! \param other Other integer property to swap with
    //!
    void       swap( SdPropInt &other ) { qSwap( mValue, other.mValue ); }
  };


#endif // SDINTPROP_H
