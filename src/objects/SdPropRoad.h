/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Tracing road properties. It contains road width, net name which road and road stratum
*/

#ifndef SDROADPROP_H
#define SDROADPROP_H

#include "SdPropInt.h"
#include "SdPropString.h"
#include "SdStratum.h"

struct SdPropRoad
  {
    SdPropInt    mWidth;   //Road width [Ширина проводника]
    SdPropString mNetName; //Net name which road is
    SdStratum    mStratum; //Road stratum

    SdPropRoad() : mWidth(0), mNetName(), mStratum(1) { }
    void operator = ( SdPropRoad const &prop );  //Назначить свойства в соответствии с шаблоном
    void append( SdPropRoad const &prop );       //Добавить свойства
    bool operator == ( SdPropRoad const &prop ); //Сравнить свойства
    void clear();                                //Установить в неопределенное состояние
    bool match( SdPropRoad const &prop );        //Сравнить на совпадение с эталоном

    void write( QJsonObject &obj ) const;
    void read( const QJsonObject obj );
  };

#endif // SDROADPROP_H
