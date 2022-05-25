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




void SdPropString::json(const QString name, const SvJsonReader &js)
  {
  mValue = OneValue;
  js.jsonString( name, mString );
  }




void SdPropString::swap(QString *src)
  {
  src->swap( mString );
  mValue = OneValue;
  }




void SdPropString::assignTo(QString &dst)
  {
  if( mValue == OneValue )
    dst = mString;
  }
