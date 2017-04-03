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

#include "SdPropInt.h"
#include "SdPropLayer.h"

struct SdPropLine {
  SdPropInt   mWidth; //Толщина линии
  SdPropInt   mType;  //Тип линии
  SdPropLayer mLayer; //Слой

  SdPropLine() : mWidth(0), mType(0), mLayer() { }
  void operator = ( SdPropLine const &prop ); //Назначить свойства в соответствии с шаблоном
  void append( SdPropLine const &prop );      //Добавить свойства
  bool operator == ( SdPropLine const &prop );//Сравнить свойства
  void clear();
  bool match( SdPropLine const &prop );       //Сравнить на совпадение с эталоном

  void write( QJsonObject &obj ) const;
  void read( const QJsonObject obj );
  };

#endif // SDLINEPROP_H
