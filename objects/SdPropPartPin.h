/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDPARTPINPROP_H
#define SDPARTPINPROP_H

#include "SdPropLayer.h"
#include "SdPropInt.h"


struct SdPropPartPinState {
    SdLayer *mLayer;   //Layer is pin on
    int      mSide;    //Side of pin location
    QString  mPinType; //Pin type for pad assotiation
  };

struct SdPropPartPin
  {
    SdPropLayer mLayer;   //Pin layer
    SdPropInt   mSide;    //Side of pin location
    QString     mPinType; //Pin type for pad assotiation

    SdPropPartPin() : mLayer(0), mPinType(), mSide(0) {}

    void operator = ( SdPropPartPin const &sour ); //Назначить свойства
    void append( SdPropPartPin const &sour );      //Установить свойства
    void clear();                                  //Установить в неопределенное состояние
    bool match( SdPropPartPin const &prop );       //Сравнить на совпадение с эталоном

    void write( QJsonObject &obj ) const;
    void read( const QJsonObject obj );

    void saveState( SdPropPartPinState *dst );
    void swapState( SdPropPartPinState *src );
  };

#endif // SDPARTPINPROP_H
