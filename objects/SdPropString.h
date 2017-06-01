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

#include <QString>
#include <QJsonObject>

class SdPropString
  {
    //Special codes for no value and many values
    enum Value { NoValue = 0, OneValue = 1, AllValue = 2 };

    QString  mString;
    Value    mValue;
  public:
    SdPropString();
    SdPropString( QString src );

    bool       operator == ( SdPropString p ) const { return mValue == OneValue && mString.compare( p.mString ) == 0; }
    void       operator = ( SdPropString p ) { if( p.mValue == OneValue ) mString = p.mString; }
    void       operator = ( const QString src ) { mString = src; mValue = OneValue; }
    QString    str() const { return mString; }
    void       append( SdPropString p );
    void       append( const QString str );
    void       clear() { mValue = NoValue; }   //Нет значения
    bool       match( SdPropString const &s ) {
      return s.mValue == OneValue && mValue == OneValue ? mString.compare( s.mString ) == 0 : true;
      }

    void       write( const QString name, QJsonObject &obj ) const { obj.insert( name, mString ); }
    void       read( const QString name, const QJsonObject obj );

    void       swap( QString *src );
  };

#endif // SDPROPSTRING_H
