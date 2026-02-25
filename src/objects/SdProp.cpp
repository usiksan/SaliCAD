/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Full properties list set
*/
#include "SdConfig.h"
#include "SdProp.h"
#include "SdPropDef.h"
#include "SdEnvir.h"
#include "SdJsonIO.h"

#include <QSettings>

SdProp *sdGlobalProp;

SdProp::SdProp()
  {
  //Сформировать свойства по умолчанию
  //Symbol
  mSymLineProp = SdPropDef::get().mSymLineProp;
  mSymTextProp = SdPropDef::get().mSymTextProp;

  //Sheet
  mSchLineProp = SdPropDef::get().mSchLineProp;
  mSchTextProp = SdPropDef::get().mSchTextProp;

  //Part
  mPartLineProp = SdPropDef::get().mPartLineProp;
  mPartTextProp = SdPropDef::get().mPartTextProp;

  //Pcb
  mPcbLineProp = SdPropDef::get().mPcbLineProp;
  mPcbTextProp = SdPropDef::get().mPcbTextProp;


  //Props of wire
  mWireProp = SdPropDef::get().mWireProp;

  //Props of wire name
  mWireNameProp = SdPropDef::get().mWireNameProp;

  //Symbol pin properties
  mSymPinProp = SdPropDef::get().mSymPinProp;

  //Symbol pin name properties
  mSymPinNameProp = SdPropDef::get().mSymPinNameProp;

  //Symbol pin number properties
  mSymPinNumberProp = SdPropDef::get().mSymPinNumberProp;

  //Part pin properties
  mPartPinProp = SdPropDef::get().mPartPinProp;

  //Part pin name properties
  mPartPinNameProp = SdPropDef::get().mPartPinNameProp;

  //Part pin number properties
  mPartPinNumberProp = SdPropDef::get().mPartPinNumberProp;

  //Props of symbol implement
  mSymImpProp = SdPropDef::get().mSymImpProp;

  //Props of part implement
  mPartImpProp = SdPropDef::get().mPartImpProp;

  //Symbol identifier (reference) aka D4.5
  mSymIdentProp = SdPropDef::get().mSymIdentProp;

  //Part identifier (reference) aka D4
  mPartIdentProp = SdPropDef::get().mPartIdentProp;

  //Value properties for symbol
  mSymValueProp = SdPropDef::get().mSymValueProp;

  //Value properties for part
  mPartValueProp = SdPropDef::get().mPartValueProp;


  //Road properties [Свойства дорожки]
  mRoadProp = SdPropDef::get().mRoadProp;

  //Polygon properties
  mPolygonProp = SdPropDef::get().mPolygonProp;

  //TODO D011 setup all prop defaults
  //Via properties [Свойства переходных отверстий]
  mViaProp = SdPropDef::get().mViaProp;

  mViaRule               = 0;
//  SdPropText    mTextDocProp;    //Свойства текста в текстовых документах

  mLineEnterType     = dleOrtho;       //By default orthogonal vertex line
  mWireEnterType     = dleOrtho;
//  QString       mWireName;       //Имя цепи
//  bool          mTextBold;       //Жирный шрифт
//  bool          mTextItalic;     //Наклонный текст
//  bool          mTextUderline;   //Подчеркивание текста

//  int           mWireNumber;     //Порядковый номер цепи (для формирования имени по умолчанию)
//  int           mMaskSide;       //Маска, определяющая возможность выбора компонентов

  }






void SdProp::loadProp()
  {
  QSettings s;
  if( s.value(QString(SDK_PROP_VERSION),QVariant( static_cast<int>(0) )).toInt() == mPropVersion ) {
    //Загружаем значения
    QByteArray ar = s.value(QString(SDK_PROP)).toByteArray();
    QJsonObject obj( svJsonObjectFromByteArray(ar) );
    SdJsonReaderProperty rdProp;
    SdJsonReader js( obj, &rdProp );

    mSymLineProp.json( "symLine", js );       //Свойства линейных объектов
    mSymTextProp.json( "symText", js );       //Свойства текстовых объектов

    mSchLineProp.json( "schLine", js );       //Свойства линейных объектов
    mSchTextProp.json( "schText", js );       //Свойства текстовых объектов

    mPartLineProp.json( "partLine", js );       //Свойства линейных объектов
    mPartTextProp.json( "partText", js );       //Свойства текстовых объектов

    mPcbLineProp.json( "pcbLine", js );       //Свойства линейных объектов
    mPcbTextProp.json( "pcbText", js );       //Свойства текстовых объектов

    mWireProp.json( "wire", js );       //Свойства сегментов цепей схемы
    mWireNameProp.json( "wireName", js );   //Свойства имени цепи
    mSymPinProp.json( js );     //Свойства вывода символа
    mSymPinNameProp.json( "symPinName", js );    //Свойства имени вывода
    mSymPinNumberProp.json( "symPinNumber", js );  //Свойства номера вывода
    mPartPinProp.json( js );    //Свойства вывода корпуса
    mPartPinNameProp.json( "partPinName", js );    //Свойства имени вывода
    mPartPinNumberProp.json( "partPinNumber", js );  //Свойства номера вывода
    mSymImpProp.json( js );     //Свойства вхождения символа
    mPartImpProp.json( js );    //Свойства вхождения корпуса
    mSymIdentProp.json( "symIdent", js );   //Свойства идентификатора символа
    mPartIdentProp.json( "partIdent", js );  //Свойства идентификатора корпуса
    mSymValueProp.json( "symValue", js );   //Value properties for symbol
    mPartValueProp.json( "partValue", js );  //Value properties for part
    mRoadProp.json( js );       //Свойства дорожки
    mPolygonProp.json( js );        //Tracing polygon properties
    mViaProp.json( js );        //Свойства переходных отверстий
    mTextDocProp.json( "textDoc", js );    //Свойства текста в текстовых документах

    js.jsonInt( "lineEnterType", mLineEnterType );
    js.jsonInt( "wireEnterType", mWireEnterType );
    js.jsonBool( "textBold", mTextBold );      //Жирный шрифт
    js.jsonBool( "textItalic", mTextItalic );     //Наклонный текст
    js.jsonBool( "textUnderline", mTextUderline );   //Подчеркивание текста

    js.jsonInt( "maskSide", mMaskSide );       //Маска, определяющая возможность выбора компонентов
    js.jsonInt( "viaRule", mViaRule );        //!< Via creation rule (throught, hidden, blind)

    }

  }





void SdProp::saveProp()
  {
  SdJsonWriter js;

  mSymLineProp.json( "symLine", js );       //Свойства линейных объектов
  mSymTextProp.json( "symText", js );       //Свойства текстовых объектов

  mSchLineProp.json( "schLine", js );       //Свойства линейных объектов
  mSchTextProp.json( "schText", js );       //Свойства текстовых объектов

  mPartLineProp.json( "partLine", js );       //Свойства линейных объектов
  mPartTextProp.json( "partText", js );       //Свойства текстовых объектов

  mPcbLineProp.json( "pcbLine", js );       //Свойства линейных объектов
  mPcbTextProp.json( "pcbText", js );       //Свойства текстовых объектов

  mWireProp.json( "wire", js );       //Свойства сегментов цепей схемы
  mWireNameProp.json( "wireName", js );   //Свойства имени цепи
  mSymPinProp.json( js );     //Свойства вывода символа
  mSymPinNameProp.json( "symPinName", js );    //Свойства имени вывода
  mSymPinNumberProp.json( "symPinNumber", js );  //Свойства номера вывода
  mPartPinProp.json( js );    //Свойства вывода корпуса
  mPartPinNameProp.json( "partPinName", js );    //Свойства имени вывода
  mPartPinNumberProp.json( "partPinNumber", js );  //Свойства номера вывода
  mSymImpProp.json( js );     //Свойства вхождения символа
  mPartImpProp.json( js );    //Свойства вхождения корпуса
  mSymIdentProp.json( "symIdent", js );   //Свойства идентификатора символа
  mPartIdentProp.json( "partIdent", js );  //Свойства идентификатора корпуса
  mSymValueProp.json( "symValue", js );   //Value properties for symbol
  mPartValueProp.json( "partValue", js );  //Value properties for part
  mRoadProp.json( js );       //Свойства дорожки
  mPolygonProp.json( js );        //Tracing polygon properties
  mViaProp.json( js );        //Свойства переходных отверстий
  mTextDocProp.json( "textDoc", js );    //Свойства текста в текстовых документах

  js.jsonInt( "lineEnterType", mLineEnterType );
  js.jsonInt( "wireEnterType", mWireEnterType );
  js.jsonBool( "textBold", mTextBold );      //Жирный шрифт
  js.jsonBool( "textItalic", mTextItalic );     //Наклонный текст
  js.jsonBool( "textUnderline", mTextUderline );   //Подчеркивание текста

  js.jsonInt( "maskSide", mMaskSide );       //Маска, определяющая возможность выбора компонентов
  js.jsonInt( "viaRule", mViaRule );        //!< Via creation rule (throught, hidden, blind)

  QSettings s;
  s.setValue( QString(SDK_PROP_VERSION), mPropVersion );
  s.setValue( QString(SDK_PROP), svJsonObjectToByteArray( js.object() ) );
  }




SdPropLine *SdProp::propLine(SdClass theClass)
  {
  switch( theClass ) {
    case dctSymbol : return &mSymLineProp;
    case dctSheet  : return &mSchLineProp;
    case dctPart   : return &mPartLineProp;
    case dctPlate  : return &mPcbLineProp;
    }
  qFatal() << "Invalid line property required" << theClass;
  return nullptr;
  }



SdPropText *SdProp::propText( SdClass theClass )
  {
  switch( theClass ) {
    case dctSymbol : return &mSymTextProp;
    case dctSheet  : return &mSchTextProp;
    case dctPart   : return &mPartTextProp;
    case dctPlate  : return &mPcbTextProp;
    }
  qFatal() << "Invalid text property required" << theClass;
  return nullptr;
  }
