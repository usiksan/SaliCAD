/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDPARTIMPPROP_H
#define SDPARTIMPPROP_H

#include "SdPropInt.h"
#include "SdAngle.h"

struct SdPropPartImp
  {
    SdAngle   mAngle;  //Угол поворота компонента
    SdPropInt mMirror; //Зеркальность компонента
    SdPropInt mSide;   //Сторона расположения на плате

    void operator = ( SdPropPartImp const &sour ); //Назначить свойства
    void append( SdPropPartImp const &sour );      //Установить свойства
    void clear();                                  //Установить в неопределенное состояние
    bool match( SdPropPartImp const &prop );       //Сравнить на совпадение с эталоном

    void write( QJsonObject &obj );
    void read( const QJsonObject obj );
  };

#endif // SDPARTIMPPROP_H