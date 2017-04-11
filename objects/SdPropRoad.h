/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDROADPROP_H
#define SDROADPROP_H

#include "SdPropInt.h"

struct SdPropRoad
  {
    SdPropInt mWidth; //Ширина проводника
    SdPropInt mLayer; //Слой проводника (сторона)

    SdPropRoad() : mWidth(0), mLayer(0) { }
    void operator = ( SdPropRoad const &prop );  //Назначить свойства в соответствии с шаблоном
    void append( SdPropRoad const &prop );       //Добавить свойства
    bool operator == ( SdPropRoad const &prop ); //Сравнить свойства
    void clear();                                //Установить в неопределенное состояние
    bool match( SdPropRoad const &prop );        //Сравнить на совпадение с эталоном

    void write( QJsonObject &obj ) const;
    void read( const QJsonObject obj );
  };

#endif // SDROADPROP_H
