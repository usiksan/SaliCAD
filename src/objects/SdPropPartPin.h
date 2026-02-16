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

#include "SdPropLayer.h"
#include "SdPropInt.h"
#include "SdPropString.h"
#include "SdPropStratum.h"


//!
//! \brief The SdPropPartPin struct - Part pin properties
//!        Stores layer, board side, and pin type for component pin/pad associations
//!
struct SdPropPartPin
  {
    SdPropLayer   mLayer;   //!< Pin layer [Слой вывода]
    SdPropStratum mSide;    //!< Side of pin location [Сторона расположения вывода]
    SdPropString  mPinType; //!< Pin type for pad association [Тип вывода для привязки контактной площадки]

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

#endif // SDPARTPINPROP_H
