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
#include "objects/SdPv.h"

#include <QString>


//!
//! \brief The SdPvString struct - Simple string property wrapper with JSON serialization
//!        Provides basic string storage with swap and JSON I/O functionality
//!
class SdPvString : public SdPv<QString,SdPvString>
  {
  public:
    //!
    //! \brief SdPvString Constructor with initial string value
    //! \param src Source string to initialize with
    //!
    SdPvString( const QString &src = QString{} ) : SdPv<QString,SdPvString>(src) {}

    SdPvString( const char *str ) : SdPv<QString,SdPvString>( QString(str) ) {}

    QString    string() const { return mValue; }

    //!
    //! \brief json Write string property to JSON writer
    //! \param name JSON key name
    //! \param js   JSON writer object
    //!
    void       json( const QString name, SvJsonWriter &js ) const { js.jsonString( name, mValue ); }

    //!
    //! \brief json Read string property from JSON reader
    //! \param name JSON key name
    //! \param js   JSON reader object
    //!
    void       json( const QString name, const SvJsonReader &js ) { js.jsonString( name, mValue ); }

  };

#endif // SDPVSTRING_H
