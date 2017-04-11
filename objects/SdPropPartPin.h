/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDPARTPINPROP_H
#define SDPARTPINPROP_H

#include "SdPropInt.h"

struct SdPropPartPin
  {
    SdPropInt mLayer;
    SdPropInt mPinType;
    SdPropInt mSide;

    SdPropPartPin() : mLayer(0), mPinType(0), mSide(0) {}

    void operator = ( SdPropPartPin const &sour ); //Назначить свойства
    void append( SdPropPartPin const &sour );      //Установить свойства
    void clear();                                  //Установить в неопределенное состояние
    bool match( SdPropPartPin const &prop );       //Сравнить на совпадение с эталоном

    void write( QJsonObject &obj ) const;
    void read( const QJsonObject obj );
  };

#endif // SDPARTPINPROP_H
