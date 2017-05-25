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

struct SdPropSymImpState {
    int mAngle;
    int mMirror;
  };

struct SdPropSymImp
  {
    SdAngle   mAngle;  //Угол поворота компонента
    SdPropInt mMirror; //Зеркальность компонента

    SdPropSymImp() : mAngle(0), mMirror(0) {}

    void operator = ( SdPropSymImp const &sour ); //Назначить свойства
    void append( SdPropSymImp const &sour );      //Установить свойства
    void clear();                                 //Установить в неопределенное состояние
    bool match( SdPropSymImp const &prop );       //Сравнить на совпадение с эталоном

    void write( QJsonObject &obj ) const;
    void read( const QJsonObject obj );

    void saveState( SdPropSymImpState *dst );
    void swapState( SdPropSymImpState *src );
  };

#endif // SDSYMIMPPROP_H
