/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  String property
*/
#ifndef SDPROPSTRING_H
#define SDPROPSTRING_H

#include "SvJsonIO.h"

#include <QString>

class SdPropString
  {
    //Special codes for no value and many values
    enum Value { NoValue = 0, OneValue = 1, AllValue = 2 };

    QString  mString;
    Value    mValue;
  public:
    SdPropString();
    SdPropString( QString src );
    SdPropString( const SdPropString &src ) = default;

    bool       operator == ( SdPropString p ) const { return mValue == OneValue && mString.compare( p.mString ) == 0; }
    void       operator = ( SdPropString p ) { if( p.mValue == OneValue ) { mString = p.mString; mValue = OneValue; } }
    void       operator = ( const QString src ) { mString = src; mValue = OneValue; }
    QString    str() const { return mValue == OneValue ? mString : QString(); }
    void       append( SdPropString p );
    void       append( const QString str );
    void       clear() { mValue = NoValue; mString.clear(); }   //Нет значения
    bool       match( SdPropString const &s ) {
      return s.mValue == OneValue && mValue == OneValue ? mString.compare( s.mString ) == 0 : true;
      }

    void       json( const QString name, SvJsonWriter &js ) const { js.jsonString( name, mString ); }
    void       json( const QString name, const SvJsonReader &js );

    void       swap( QString *src );

    void       assignTo( QString &dst );

    bool       isValid() const { return mValue == OneValue; }
  };

#endif // SDPROPSTRING_H
