/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Symbol implementation properties:
   angle of implementation rotation
   mirror of implementation
*/

#ifndef SDSYMIMPPROP_H
#define SDSYMIMPPROP_H

#include "SdPropAngle.h"

struct SdPropSymImpState {
    int mAngle;
    int mMirror;
  };

struct SdPropSymImp
  {
    SdPropAngle   mAngle;  //Angle of symbol implementation rotation [Угол поворота компонента]
    SdPropInt     mMirror; //Mirror of implementation [Зеркальность компонента]

    SdPropSymImp() : mAngle(0), mMirror(0) {}

    void operator = ( SdPropSymImp const &sour ); //Assign properties [Назначить свойства]
    void append( SdPropSymImp const &sour );      //Append properties [Установить свойства]
    void clear();                                 //Set properties to undefined state [Установить в неопределенное состояние]
    bool match( SdPropSymImp const &prop );       //Compare properties with etalon [Сравнить на совпадение с эталоном]

    void write( QJsonObject &obj ) const;
    void read( const QJsonObject obj );

    void saveState( SdPropSymImpState *dst );
    void swapState( SdPropSymImpState *src );
  };

#endif // SDSYMIMPPROP_H