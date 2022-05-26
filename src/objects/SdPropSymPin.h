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
#include "SdPropInt.h"


struct SdPropSymPinState {
    SdLayer *mLayer;   //Layer is pin on
    int      mPinType; //Pin type for modelling
  };


struct SdPropSymPin
  {
    SdPropLayer mLayer;   //Layer is pin on
    SdPropInt   mPinType; //Pin type for modelling

    SdPropSymPin() : mLayer(), mPinType(0) {}

    void operator = ( SdPropSymPin const &sour ); //Назначить свойства
    void append( SdPropSymPin const &sour );      //Установить свойства
    void clear();                                 //Установить в неопределенное состояние
    bool match( SdPropSymPin const &prop );       //Сравнить на совпадение с эталоном

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

    void saveState( SdPropSymPinState *dst );
    void swapState( SdPropSymPinState *src );
  };

#endif // SDSYMPINPROP_H
