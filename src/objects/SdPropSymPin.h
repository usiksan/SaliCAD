/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Symbol pin properties
*/

#ifndef SDSYMPINPROP_H
#define SDSYMPINPROP_H

#include "SdPropLayer.h"
#include "SdPvInt.h"



//!
//! \brief The SdPropSymPin struct - Symbol pin properties for schematic components
//!        Stores layer assignment and pin type for electrical modeling and simulation
//!
struct SdPropSymPin
  {
    SdPropLayer mLayer;   //!< Layer where pin is placed [Слой размещения вывода]
    SdPvInt   mPinType; //!< Pin type for electrical modeling [Тип вывода для моделирования]

    //!
    //! \brief json Function to write object content into json writer
    //! \param js   Json writer
    //!
    void json( SvJsonWriter &js ) const;

    //!
    //! \brief json Function to read object content from json reader
    //! \param js   Json reader
    //!
    void json( const SdJsonReader &js);

    //!
    //! \brief swap Swap all symbol pin properties with another instance
    //! \param other Other SdPropSymPin object to swap with
    //!
    void swap( SdPropSymPin &other );
  };

#endif // SDSYMPINPROP_H
