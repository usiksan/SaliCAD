/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Int property
*/

#ifndef SDINTPROP_H
#define SDINTPROP_H

#include "SvJsonIO.h"

class SdPropInt
  {
  protected:
    int mValue;
  public:
    //Special codes for no value and many values
    enum { NoValue = -1, AllValue = -2 };

    SdPropInt() : mValue(0) { }
    SdPropInt( int i ) : mValue( i ) { }
    SdPropInt( const SdPropInt &src ) = default;

    bool       operator == ( int i ) const { return mValue == i; }
               operator int () const { return mValue; }
    void       operator = ( int i ) { if( i >= 0 || i < AllValue ) mValue = i; }
    SdPropInt& operator = ( const SdPropInt& s ) { if( s.mValue != NoValue && s.mValue != AllValue ) mValue = s.mValue; return *this; }

    //Value as int
    int        getValue() const { return mValue; }

    //Value as double
    double     getDouble() const { return static_cast<double>(mValue); }

    //Visual representation
    QString    log2Phis( double ppm ) const;
    void       setFromPhis( const QString src, double ppm );

    void       append( int i ) { if( mValue != i && i >= 0 ) mValue = mValue == NoValue ? i : AllValue; }   //Добавить значение
    void       clear() { mValue = NoValue; }   //Нет значения
    bool       match( SdPropInt const &s ) {
      return s.mValue != NoValue && s.mValue != AllValue && mValue != NoValue && mValue != AllValue ? s.mValue == mValue : true;
      }

    void       json( const QString key, SvJsonWriter &js ) const { js.jsonInt( key, mValue ); }
    void       json( const QString key, const SvJsonReader &js ) { js.jsonInt( key, mValue ); }

    int        swap( int v ) { int t = mValue; mValue = v; return t; }

    bool       isValid() const { return mValue >= 0; }
  };

#endif // SDINTPROP_H
