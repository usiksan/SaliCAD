/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Tracing polygon properties. It contains windows gap, net name which polygon and polygon stratum
*/
#ifndef SDPROPPOLYGON_H
#define SDPROPPOLYGON_H

#include "SdPvInt.h"
#include "SdPvString.h"
#include "SdPvStratum.h"
#include "objects/SdPvMulty.h"

//!
//! \brief The SdPropPolygon struct - Polygon fill properties for PCB copper areas
//!        Stores gap clearance, net association, and layer stratum for polygon fills
//!
struct SdPropPolygon
  {
    SdPvInt     mGap;     //!< Polygon gap clearance [Зазоры окон полигона]
    SdPvString  mNetName; //!< Net name which polygon belongs to [Имя цепи, которой принадлежит полигон]
    SdPvStratum mStratum; //!< Polygon stratum (layer) [Слой размещения полигона]

    //!
    //! \brief SdPropPolygon Default constructor
    //!        Initializes gap to 100, empty net name, and top stratum
    //!
    SdPropPolygon() : mGap(100), mNetName(), mStratum(stmTop) { }

    //!
    //! \brief json Function to write object content into json writer
    //! \param js   Json writer
    //!
    void json( SvJsonWriter &js ) const;

    //!
    //! \brief json Function to read object content from json reader
    //! \param js   Json reader
    //!
    void json( const SvJsonReader &js);

    //!
    //! \brief swap Swap all polygon properties with another instance
    //! \param other Other SdPropPolygon object to swap with
    //!
    void swap( SdPropPolygon &other );
  };


using SdPropComposerPolygon = SdPropComposer<SdPropPolygon, &SdPropPolygon::mGap, &SdPropPolygon::mNetName, &SdPropPolygon::mStratum>;

#endif // SDPROPPOLYGON_H
