/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDTEXTPROP_H
#define SDTEXTPROP_H

#include "SdPropInt.h"
#include "SdAngle.h"

struct SdTextProp {
  SdPropInt  mSize;   //Размер текста
  SdAngle    mDir;    //Направление
  SdPropInt  mLayer;  //Слой
  SdPropInt  mFont;   //Идентификатор шрифта
  SdPropInt  mHorz;   //Выравнивание горизонтальное, вертикальное и зеркальность
  SdPropInt  mVert;
  SdPropInt  mMirror;

  SdTextProp();

  void operator = ( SdTextProp const &prop ); //Назначить свойства (кроме origin)
  void assign( SdTextProp const &prop );      //Назначить свойства (включая origin)
  void append( SdTextProp const &prop );      //Установить свойства
  void clear();                               //Установить в неопределенное состояние
  bool match( SdTextProp const &prop );       //Сравнить на совпадение с эталоном
  };


#endif // SDTEXTPROP_H
