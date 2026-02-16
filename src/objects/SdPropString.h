/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  String property
*/
#ifndef SDPROPSTRING_H
#define SDPROPSTRING_H

#include "SvLib/SvJsonIO.h"

#include <QString>

//!
//! \brief The SdPropString struct - Simple string property wrapper with JSON serialization
//!        Provides basic string storage with swap and JSON I/O functionality
//!
struct SdPropString
  {
    QString  mString;           //!< Stored string value

    //!
    //! \brief SdPropString Constructor with initial string value
    //! \param src Source string to initialize with
    //!
    SdPropString( const QString &src ) : mString(src) {}

    //!
    //! \brief json Write string property to JSON writer
    //! \param name JSON key name
    //! \param js   JSON writer object
    //!
    void       json( const QString name, SvJsonWriter &js ) const { js.jsonString( name, mString ); }

    //!
    //! \brief json Read string property from JSON reader
    //! \param name JSON key name
    //! \param js   JSON reader object
    //!
    void       json( const QString name, const SvJsonReader &js ) { js.jsonString( name, mString ); }

    //!
    //! \brief swap Swap string values with another SdPropString instance
    //! \param other Other string property to swap with
    //!
    void       swap( SdPropString &other ) { mString.swap( other.mString ); }
  };

#endif // SDPROPSTRING_H
