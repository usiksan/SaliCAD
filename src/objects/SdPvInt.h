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

#ifndef SDPVINT_H
#define SDPVINT_H

#include "objects/SdPv.h"
#include "SvLib/SvJsonIO.h"

//!
//! \brief The SdPvInt struct - Integer property with conversion between logical and physical values
//!        Provides JSON serialization and physical value conversion (e.g., for distances, sizes)
//!
class SdPvInt : public SdPv<int,SdPvInt>
  {
  public:
    SdPvInt( int v = 0 ) : SdPv<int,SdPvInt>(v) {}
    SdPvInt( const QString &src, double ppm ) : SdPv<int,SdPvInt>(0) { setFromPhis( src, ppm ); }

    bool       asBool() const { return mValue != 0; }

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
    void       setFromPhis( const QString &src, double ppm );

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

  };


#endif // SDPVINT_H
