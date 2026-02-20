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

using SdPropComposerPartPin = SdPropComposer<SdPropPartPin, &SdPropPartPin::mLayer, &SdPropPartPin::mPinType, &SdPropPartPin::mSide>;


#endif // SDPARTPINPROP_H
