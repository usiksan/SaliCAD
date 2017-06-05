/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part implementation properties
*/

#ifndef SDPARTIMPPROP_H
#define SDPARTIMPPROP_H

#include "SdPropInt.h"
#include "SdAngle.h"

#define dsideTop 0
#define dsideBot 1

struct SdPropPartImpState {
    int mAngle;
    int mMirror;
    int mSide;
  };

struct SdPropPartImp
  {
    SdAngle   mAngle;  //Угол поворота компонента
    SdPropInt mMirror; //Зеркальность компонента
    SdPropInt mSide;   //Сторона расположения на плате

    void operator = ( SdPropPartImp const &sour ); //Назначить свойства
    void append( SdPropPartImp const &sour );      //Установить свойства
    void clear();                                  //Установить в неопределенное состояние
    bool match( SdPropPartImp const &prop );       //Сравнить на совпадение с эталоном

    void write( QJsonObject &obj ) const;
    void read( const QJsonObject obj );

    void saveState( SdPropPartImpState *dst );
    void swapState( SdPropPartImpState *src );
  };

#endif // SDPARTIMPPROP_H
