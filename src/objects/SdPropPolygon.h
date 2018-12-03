/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Tracing polygon properties. It contains windows gap, net name which polygon and polygon stratum
*/
#ifndef SDPROPPOLYGON_H
#define SDPROPPOLYGON_H

#include "SdPropInt.h"
#include "SdPropString.h"
#include "SdStratum.h"


struct SdPropPolygon
  {
    SdPropInt    mGap;     //Polygon gap [Зазоры окон полигона]
    SdPropString mNetName; //Net name which polygon is
    SdStratum    mStratum; //Polygon stratum

    SdPropPolygon() : mGap(100), mNetName(), mStratum(1) { }
    void operator = ( SdPropPolygon const &prop );  //Назначить свойства в соответствии с шаблоном
    void append( SdPropPolygon const &prop );       //Добавить свойства
    bool operator == ( SdPropPolygon const &prop ); //Сравнить свойства
    void clear();                                   //Установить в неопределенное состояние
    bool match( SdPropPolygon const &prop );        //Сравнить на совпадение с эталоном

    void write( QJsonObject &obj ) const;
    void read( const QJsonObject obj );
  };

#endif // SDPROPPOLYGON_H
