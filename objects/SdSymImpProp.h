/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDSYMIMPPROP_H
#define SDSYMIMPPROP_H

#include "SdAngle.h"

struct SdSymImpProp
  {
    SdAngle   mAngle;  //Угол поворота компонента
    SdIntProp mMirror; //Зеркальность компонента

    SdSymImpProp() : mAngle(0), mMirror(0) {}

    void operator = ( SdSymImpProp const &sour ); //Назначить свойства
    void append( SdSymImpProp const &sour );      //Установить свойства
    void clear();                                 //Установить в неопределенное состояние
    bool match( SdSymImpProp const &prop );       //Сравнить на совпадение с эталоном

    void write( QJsonObject &obj ) const;
    void read( const QJsonObject obj );
  };

#endif // SDSYMIMPPROP_H
