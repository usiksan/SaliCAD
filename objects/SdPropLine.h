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

//Line types
#define dltSolid  0 //Solid line _______
#define dltDotted 1 //Dotted line . . . . .
#define dltDashed 2 //Dashed line __  __  __

class SdUndo;

struct SdPropLineState {
    int      mWidth;
    int      mType;
    SdLayer *mLayer;
  };

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
  void saveState( SdPropLineState *dst );
  void swapState( SdPropLineState *src );
  };

#endif // SDLINEPROP_H
