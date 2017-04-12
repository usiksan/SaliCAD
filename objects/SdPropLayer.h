/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDPROPLAYER_H
#define SDPROPLAYER_H

#include "SdLayer.h"
#include <QString>
#include <QJsonObject>
#include <QDataStream>

class SdPropLayer
  {
    //Special codes for no value and many values
    enum Value { NoValue = 0, OneValue = 1, AllValue = 2 };
  protected:
    SdLayer *mLayer;
    Value    mValue;
  public:
    SdPropLayer();
    SdPropLayer( QString id );

    bool       operator == ( SdPropLayer p ) const { return mValue == OneValue && mLayer == p.mLayer; }
    void       operator = ( SdPropLayer p ) { if( p.mValue == OneValue ) mLayer = p.mLayer; }
    void       operator = ( SdLayer *layer ) { mLayer = layer; mValue = layer ? OneValue : NoValue; }
    SdLayer   *layer( bool otherSide = false ) const;
    void       append( SdPropLayer p );
    void       clear() { mValue = NoValue; }   //Нет значения
    bool       match( SdPropLayer const &s ) {
      return s.mValue == OneValue && mValue == OneValue ? s.mLayer == mLayer : true;
      }

    void       write( const QString name, QJsonObject &obj ) const { obj.insert( name, mLayer->id() ); }
    void       read( const QString name, const QJsonObject obj );

    friend QDataStream& operator >> ( QDataStream &is, SdPropLayer &p );
    friend QDataStream& operator << ( QDataStream &os, const SdPropLayer &p );
  };

#endif // SDPROPLAYER_H
