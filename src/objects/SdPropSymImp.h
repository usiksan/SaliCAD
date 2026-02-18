/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Symbol implementation properties:
   angle of implementation rotation
   mirror of implementation
*/

#ifndef SDSYMIMPPROP_H
#define SDSYMIMPPROP_H

#include "SdPvAngle.h"


//!
//! \brief The SdPropSymImp struct - Symbol implementation properties
//!        Stores rotation angle and mirror state for component/symbol instances
//!
struct SdPropSymImp
  {
    SdPvAngle   mAngle;  //!< Angle of symbol implementation rotation [Угол поворота компонента]
    SdPvInt     mMirror; //!< Mirror of implementation [Зеркальность компонента]

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
    //! \brief swap Swap all symbol implementation properties with another instance
    //! \param other Other SdPropSymImp object to swap with
    //!
    void swap( SdPropSymImp &other );
  };
#endif // SDSYMIMPPROP_H
