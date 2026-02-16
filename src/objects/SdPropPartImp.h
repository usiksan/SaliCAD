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

#include "SdPropInt.h"
#include "SdPropAngle.h"
#include "SdPropStratum.h"

struct SdPropPartImpState {
    int mAngle;
    int mSide;
  };

struct SdPropPartImp
  {
    SdPropAngle   mAngle;  //Angle of rotation component [Угол поворота компонента]
    SdPropStratum     mSide;   //Side of component location [Сторона расположения на плате]

    void operator = ( SdPropPartImp const &sour ); //Назначить свойства
    void append( SdPropPartImp const &sour );      //Установить свойства
    void clear();                                  //Установить в неопределенное состояние
    bool match( SdPropPartImp const &prop );       //Сравнить на совпадение с эталоном


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

    void saveState( SdPropPartImpState *dst );
    void swapState( SdPropPartImpState *src );
  };

#endif // SDPARTIMPPROP_H
