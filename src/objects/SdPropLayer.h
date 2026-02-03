/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Layer property
*/
#ifndef SDPROPLAYER_H
#define SDPROPLAYER_H

#include "SdLayer.h"
#include <QString>

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
    SdPropLayer( const SdPropLayer &src ) = default;

    bool       isValid() const { return mValue == OneValue; }
    bool       isVisible( bool otherSide = false ) const;
    bool       isEdited() const;
    bool       operator == ( const SdPropLayer p ) const { return mValue == OneValue && mLayer == p.mLayer; }
    void       operator = ( const SdPropLayer p ) { if( p.mValue == OneValue ) { mLayer = p.mLayer; mValue = OneValue; } }
    void       operator = ( SdLayer *layer ) { mLayer = layer; mValue = layer ? OneValue : NoValue; }
    void       set( const QString id );
    SdLayer   *layer( bool otherSide = false ) const;
    void       append( SdPropLayer p );
    void       clear() { mValue = NoValue; }   //Нет значения
    void       assign( const SdPropLayer &p ) { mLayer = p.mLayer; mValue = p.mValue; }
    void       setLayerUsage() const;
    bool       match( SdPropLayer const &s ) {
      return s.mValue == OneValue && mValue == OneValue ? s.mLayer == mLayer : true;
      }

    void       json( const QString name, SvJsonWriter &js ) const { js.jsonString( name, mLayer->id() ); }
    void       json( const QString name, const SdJsonReader &js );

    SdLayer   *swap( SdLayer *src );
  };

#endif // SDPROPLAYER_H
