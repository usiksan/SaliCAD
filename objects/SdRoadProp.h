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

#include "SdIntProp.h"

struct SdRoadProp
  {
    SdIntProp mWidth; //Ширина проводника
    SdIntProp mLayer; //Слой проводника (сторона)

    SdRoadProp() : mWidth(0), mLayer(0) { }
    void operator = ( SdRoadProp const &prop );  //Назначить свойства в соответствии с шаблоном
    void append( SdRoadProp const &prop );       //Добавить свойства
    bool operator == ( SdRoadProp const &prop ); //Сравнить свойства
    void clear();                                //Установить в неопределенное состояние
    bool match( SdRoadProp const &prop );        //Сравнить на совпадение с эталоном

    void write( QJsonObject &obj ) const;
    void read( const QJsonObject obj );
  };

#endif // SDROADPROP_H
