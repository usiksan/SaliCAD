/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Tracing road properties. It contains road width, net name which road and road stratum
*/

#ifndef SDROADPROP_H
#define SDROADPROP_H

#include "SdPvInt.h"
#include "SdPvString.h"
#include "SdPvStratum.h"
#include "objects/SdPvMulty.h"

//!
//! \brief The SdPropRoad struct - Routing track properties for PCB traces
//!        Stores track width, net association, and layer stratum for electrical connections
//!
struct SdPropRoad
  {
    SdPvInt     mWidth;   //!< Road width [Ширина проводника]
    SdPvString  mNetName; //!< Net name which road belongs to [Имя цепи, которой принадлежит проводник]
    SdPvStratum mStratum; //!< Road stratum (layer) [Слой размещения проводника]

    //!
    //! \brief SdPropRoad Default constructor
    //!        Initializes width to 200, empty net name, and top stratum
    //!
    SdPropRoad() : mWidth(200), mNetName(), mStratum(stmTop) { }

    bool operator == ( const SdPropRoad &road ) const { return mWidth == road.mWidth && mNetName == road.mNetName && mStratum == road.mStratum; }

    //!
    //! \brief json Overloaded function to write object content into json writer
    //! \param js   Json writer
    //!
    void json( SvJsonWriter &js ) const;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //! \param js   Json reader
    //!
    void json(const SdJsonReader &js);

    //!
    //! \brief swap Swap all road properties with another instance
    //! \param other Other SdPropRoad object to swap with
    //!
    void swap( SdPropRoad &other );
  };

using SdPropComposerRoad = SdPropComposer<SdPropRoad, &SdPropRoad::mNetName, &SdPropRoad::mStratum, &SdPropRoad::mWidth>;

#endif // SDROADPROP_H
