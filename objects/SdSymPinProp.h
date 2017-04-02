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

#include "SdIntProp.h"

struct SdSymPinProp
  {
    SdIntProp mLayer;
    SdIntProp mPinType;

    SdSymPinProp() : mLayer(0), mPinType(0) {}

    void operator = ( SdSymPinProp const &sour ); //Назначить свойства
    void append( SdSymPinProp const &sour );      //Установить свойства
    void clear();                                //Установить в неопределенное состояние
    bool match( SdSymPinProp const &prop );       //Сравнить на совпадение с эталоном

    void write( QJsonObject &obj ) const;
    void read( const QJsonObject obj );
  };

#endif // SDSYMPINPROP_H
