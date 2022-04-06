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
#include "SdPropAngle.h"
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
  SdPropLayer mLayer;  //Layer of text [Слой]
  SdPropInt   mSize;   //Text size [Размер текста]
  SdPropAngle mDir;    //Direction [Направление]
  SdPropInt   mFont;   //Font ident [Идентификатор шрифта]
  SdPropInt   mHorz;   //Alignment [Выравнивание горизонтальное, вертикальное и зеркальность]
  SdPropInt   mVert;
  SdPropInt   mMirror;

  SdPropText();

  void operator = ( SdPropText const &prop ); //Назначить свойства
  void assign( SdPropText const &prop );      //Назначить свойства
  void append( SdPropText const &prop );      //Установить свойства
  void clear();                               //Установить в неопределенное состояние
  bool match( SdPropText const &prop );       //Сравнить на совпадение с эталоном

  void write( const QString prefix, QJsonObject &obj ) const;
  void read( const QString prefix, const QJsonObject obj );

  void jsonWrite( const char *prefix, SdJsonWriter &js ) const { write( QString(prefix), js.ref() ); }
  void jsonRead( const char *prefix, const SdJsonReader &js ) { read( QString(prefix), js.object() ); }

  void saveState(SdPropTextState *dst );
  void swapState(SdPropTextState *src );
  };

//Parse src string and find numeric substring. Numeric substring converted into int, int incremented
//and convert to substring. Substring, at end, inserted into src string and return result
//Example: src=PORT18CDF will result PORT19CDF
QString nextText( const QString src );

#endif // SDTEXTPROP_H
