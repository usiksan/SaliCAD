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

#include "SdPropString.h"
#include "SdStratum.h"


struct SdPropVia
  {
    SdStratum    mStratum; //Stratum of via
    SdPropString mPadType; //Pin type for pad assotiation
    SdPropString mNetName; //Net name which via is

    SdPropVia() : mStratum(stmThrough), mPadType(), mNetName() {}

    //Assign properties with pattern accordings [Назначить свойства в соответствии с шаблоном]
    void operator = ( SdPropVia const &prop );

    //Append properties [Добавить свойства]
    void append( SdPropVia const &prop );

    //Compare properties [Сравнить свойства]
    bool operator == ( SdPropVia const &prop );

    //Set to undefined state. When undefined state then appending equals coping [Установить в неопределенное состояние]
    void clear();

    //Compare with pattern [Сравнить на совпадение с эталоном]
    bool match( SdPropVia const &prop );

    //Write and read
    void write( QJsonObject &obj ) const;
    void read( const QJsonObject obj );
  };

#endif // SDPROPVIA_H
