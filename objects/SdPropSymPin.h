/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDSYMPINPROP_H
#define SDSYMPINPROP_H

#include "SdPropInt.h"

struct SdPropSymPin
  {
    SdPropInt mLayer;
    SdPropInt mPinType;

    SdPropSymPin() : mLayer(0), mPinType(0) {}

    void operator = ( SdPropSymPin const &sour ); //Назначить свойства
    void append( SdPropSymPin const &sour );      //Установить свойства
    void clear();                                //Установить в неопределенное состояние
    bool match( SdPropSymPin const &prop );       //Сравнить на совпадение с эталоном

    void write( QJsonObject &obj ) const;
    void read( const QJsonObject obj );
  };

#endif // SDSYMPINPROP_H
