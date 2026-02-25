/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part pin properties
*/

#ifndef SDPARTPINPROP_H
#define SDPARTPINPROP_H

#include "SdPvLayer.h"
#include "SdPvInt.h"
#include "SdPvString.h"
#include "SdPvStratum.h"
#include "objects/SdPvMulty.h"

//!
//! \brief The SdPropPartPin struct - Part pin properties
//!        Stores layer, board side, and pin type for component pin/pad associations
//!
struct SdPropPartPin
  {
    SdPvLayer   mLayer;   //!< Pin layer [Слой вывода]
    SdPvStratum mSide;    //!< Side of pin location [Сторона расположения вывода]
    SdPvString  mPinType; //!< Pin type for pad association [Тип вывода для привязки контактной площадки]

    //!
    //! \brief json Overloaded function to write object content into json writer
    //! \param js   Json writer
    //!
    void json( SvJsonWriter &js ) const;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //! \param js   Json reader
    //!
    void json( const SdJsonReader &js);

    //!
    //! \brief swap Swap all part pin properties with another instance
    //! \param other Other SdPropPartPin object to swap with
    //!
    void swap( SdPropPartPin &other );
  };

// using SdPropComposerPartPin = SdPropComposer<SdPropPartPin, &SdPropPartPin::mLayer, &SdPropPartPin::mPinType, &SdPropPartPin::mSide>;

class SdPropComposerPartPin : public SdPropComposer<SdPropPartPin, &SdPropPartPin::mLayer, &SdPropPartPin::mPinType, &SdPropPartPin::mSide>
  {
  public:
    // SdLayer *getSingleLayer( bool otherSide = false ) const
    //   {
    //   auto &propLayer = get<&SdPropPartPin::mLayer>();
    //   return propLayer.isSingle() ? propLayer.value().layer(otherSide) : nullptr;
    //   }

    auto& layer() { return get<&SdPropPartPin::mLayer>(); }
    const auto& layer() const { return get<&SdPropPartPin::mLayer>(); }

    auto& side() { return get<&SdPropPartPin::mSide>(); }
    const auto& side() const { return get<&SdPropPartPin::mSide>(); }

    auto& pinType() { return get<&SdPropPartPin::mPinType>(); }
    const auto& pinType() const { return get<&SdPropPartPin::mPinType>(); }
  };

#endif // SDPARTPINPROP_H
