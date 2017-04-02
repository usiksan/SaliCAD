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

#include "SdIntProp.h"
#include "SdAngle.h"

struct SdPartImpProp
  {
    SdAngle   mAngle;  //Угол поворота компонента
    SdIntProp mMirror; //Зеркальность компонента
    SdIntProp mSide;   //Сторона расположения на плате

    void operator = ( SdPartImpProp const &sour ); //Назначить свойства
    void append( SdPartImpProp const &sour );      //Установить свойства
    void clear();                                  //Установить в неопределенное состояние
    bool match( SdPartImpProp const &prop );       //Сравнить на совпадение с эталоном

    void write( QJsonObject &obj );
    void read( const QJsonObject obj );
  };

#endif // SDPARTIMPPROP_H
