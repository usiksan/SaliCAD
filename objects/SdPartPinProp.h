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

struct SdPartPinProp
  {
    SdPropInt mLayer;
    SdPropInt mPinType;
    SdPropInt mSide;

    SdPartPinProp() : mLayer(0), mPinType(0), mSide(0) {}

    void operator = ( SdPartPinProp const &sour ); //Назначить свойства
    void append( SdPartPinProp const &sour );      //Установить свойства
    void clear();                                  //Установить в неопределенное состояние
    bool match( SdPartPinProp const &prop );       //Сравнить на совпадение с эталоном

    void write( QJsonObject &obj ) const;
    void read( const QJsonObject obj );
  };

#endif // SDPARTPINPROP_H
