/*
  Проект "SaliBrick"
    Визуальное программирование микроконтроллеров
  Автор
    Сибилев А.С.
  Описание
*/
#include "SdPropString.h"

SdPropString::SdPropString() :
  mString(),
  mValue( NoValue )
  {

  }



SdPropString::SdPropString(QString src) :
  mString( src ),
  mValue( OneValue )
  {

  }



void SdPropString::append(SdPropString p)
  {
  if( mValue == NoValue ) {
    mValue  = p.mValue;
    mString = p.mString;
    }
  else if( mValue == OneValue ) {
    if( p.mValue != OneValue || mString.compare( p.mString ) != 0 )
      mValue = AllValue;
    }
  }



void SdPropString::append(const QString str)
  {
  if( mValue == NoValue ) {
    mValue  = OneValue;
    mString = str;
    }
  else if( mValue == OneValue ) {
    if( mString.compare( str ) != 0 )
      mValue = AllValue;
    }
  }




void SdPropString::read(const QString name, const QJsonObject obj)
  {
  mValue = OneValue;
  mString = obj.value(name).toString();
  }




void SdPropString::swap(QString *src)
  {
  src->swap( mString );
  mValue = OneValue;
  }
