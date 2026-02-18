/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Tracing via properties. It contains via pad type, net name which via and via stratum
*/
#ifndef SDPROPVIA_H
#define SDPROPVIA_H

#include "SdPvString.h"
#include "SdPvStratum.h"


//!
//! \brief The SdPropVia struct - Via properties for PCB interlayer connections
//!        Stores stratum (through, blind, buried), pad type association, and net assignment
//!
struct SdPropVia
  {
    SdPvStratum mStratum; //!< Via stratum type (through, blind, buried) [Тип переходного отверстия]
    SdPvString  mPadType; //!< Pad type for pad association [Тип контактной площадки для привязки]
    SdPvString  mNetName; //!< Net name which via belongs to [Имя цепи, которой принадлежит переходное отверстие]

    //!
    //! \brief SdPropVia Default constructor
    //!        Initializes stratum to through via, empty pad type and empty net name
    //!
    SdPropVia() : mStratum(stmThrough), mPadType(), mNetName() {}

    //Write and read
    //!
    //! \brief json Overloaded function to write object content into json writer
    //! \param js   Json writer
    //!
    void json( SvJsonWriter &js ) const;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //! \param js   Json reader
    //!
    void json( const SvJsonReader &js);

    //!
    //! \brief swap Swap all via properties with another instance
    //! \param other Other SdPropVia object to swap with
    //!
    void swap( SdPropVia &other );
  };

#endif // SDPROPVIA_H
