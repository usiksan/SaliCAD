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
#include "SdPropLayer.h"

//Horizontal alignment
#define dhjLeft    0
#define dhjCenter  1
#define dhjRight   2

//Vertical alignment
#define dvjTop     0
#define dvjMiddle  1
#define dvjBottom  2

class SdUndo;

struct SdPropTextState {
    SdLayer *mLayer;
    int      mSize;
    int      mDir;
    int      mFont;
    int      mHorz;
    int      mVert;
    int      mMirror;
  };

struct SdPropText {
  SdPropLayer mLayer; //Слой
  SdPropInt   mSize;   //Размер текста
  SdAngle     mDir;    //Направление
  SdPropInt   mFont;   //Идентификатор шрифта
  SdPropInt   mHorz;   //Выравнивание горизонтальное, вертикальное и зеркальность
  SdPropInt   mVert;
  SdPropInt   mMirror;

  SdPropText();

  void operator = ( SdPropText const &prop ); //Назначить свойства (кроме origin)
  void assign( SdPropText const &prop );      //Назначить свойства (включая origin)
  void append( SdPropText const &prop );      //Установить свойства
  void clear();                               //Установить в неопределенное состояние
  bool match( SdPropText const &prop );       //Сравнить на совпадение с эталоном

  void write( const QString prefix, QJsonObject &obj ) const;
  void read( const QString prefix, const QJsonObject obj );

  void saveState(SdPropTextState *dst );
  void swapState(SdPropTextState *src );
  };


#endif // SDTEXTPROP_H