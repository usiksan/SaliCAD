/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDLINEPROP_H
#define SDLINEPROP_H

#include "SdIntProp.h"

struct SdLineProp {
  SdIntProp mWidth; //Толщина линии
  SdIntProp mType;  //Тип линии
  SdIntProp mLayer; //Слой

  SdLineProp() : mWidth(0), mType(0), mLayer(0) { }
  void operator = ( SdLineProp const &prop ); //Назначить свойства в соответствии с шаблоном
  void append( SdLineProp const &prop );      //Добавить свойства
  bool operator == ( SdLineProp const &prop );//Сравнить свойства
  void clear();
  bool match( SdLineProp const &prop );       //Сравнить на совпадение с эталоном

  void write( QJsonObject &obj ) const;
  void read( const QJsonObject obj );
  };

#endif // SDLINEPROP_H
