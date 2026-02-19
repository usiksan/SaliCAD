/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part implementation properties
*/

#ifndef SDPARTIMPPROP_H
#define SDPARTIMPPROP_H

#include "SdPvAngle.h"
#include "SdPvStratum.h"
#include "SdJsonIO.h"


//!
//! \brief The SdPropPartImp struct - Part implementation properties
//!        Stores rotation angle and board side placement for component parts
//!
struct SdPropPartImp
  {
    SdPvAngle   mAngle;  //!< Angle of rotation component [Угол поворота компонента]
    SdPvStratum mSide;   //!< Side of component location [Сторона расположения на плате]

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
    //! \brief swap Swap all part implementation properties with another instance
    //! \param other Other SdPropPartImp object to swap with
    //!
    void swap( SdPropPartImp &other );
  };

#endif // SDPARTIMPPROP_H
