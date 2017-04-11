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

struct SdPropText {
  SdPropInt  mSize;   //Размер текста
  SdAngle    mDir;    //Направление
  SdPropInt  mLayer;  //Слой
  SdPropInt  mFont;   //Идентификатор шрифта
  SdPropInt  mHorz;   //Выравнивание горизонтальное, вертикальное и зеркальность
  SdPropInt  mVert;
  SdPropInt  mMirror;

  SdPropText();

  void operator = ( SdPropText const &prop ); //Назначить свойства (кроме origin)
  void assign( SdPropText const &prop );      //Назначить свойства (включая origin)
  void append( SdPropText const &prop );      //Установить свойства
  void clear();                               //Установить в неопределенное состояние
  bool match( SdPropText const &prop );       //Сравнить на совпадение с эталоном
  };


#endif // SDTEXTPROP_H
