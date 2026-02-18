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
#ifndef SDPVSTRING_H
#define SDPVSTRING_H

#include "SvLib/SvJsonIO.h"

#include <QString>

//!
//! \brief The SdPvString struct - Simple string property wrapper with JSON serialization
//!        Provides basic string storage with swap and JSON I/O functionality
//!
class SdPvString
  {
    QString  mString;           //!< Stored string value
  public:
    //!
    //! \brief SdPvString Constructor with initial string value
    //! \param src Source string to initialize with
    //!
    SdPvString( const QString &src = QString{} ) : mString(src) {}

    SdPvString( const char *str ) : mString(str) {}

    QString    string() const { return mString; }

    bool       operator != ( const SdPvString &other ) const { return mString != other.mString; }

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
    void       swap( SdPvString &other ) { mString.swap( other.mString ); }
  };

#endif // SDPVSTRING_H
