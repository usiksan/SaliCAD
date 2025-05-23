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
#include "SdEnvir.h"
#include "SdJsonIO.h"

#include <QSettings>

SdProp *sdGlobalProp;

SdProp::SdProp()
  {
  //Сформировать свойства по умолчанию
  mLineProp.mLayer.set( QString( LID0_REMARK ) );       //Свойства линейных объектов
  mLineProp.mType  = dltSolid;
  mLineProp.mWidth = 0;

  mTextProp.mLayer.set( QString( LID0_REMARK ) );       //Свойства текстовых объектов
  mTextProp.mFont   = 0;
  mTextProp.mSize   = 350;
  mTextProp.mDir    = da0;
  mTextProp.mHorz   = dhjLeft;
  mTextProp.mVert   = dvjMiddle;
  mTextProp.mMirror = 0;

  //Props of wire
  mWireProp.mLayer.set( QString( LID0_NET ) );       //Свойства сегментов цепей схемы
  mWireProp.mType  = dltSolid;
  mWireProp.mWidth = 0;

  //Props of wire name
  mWireNameProp.mLayer.set( QString( LID0_NET_NAME ) );       //Свойства текстовых объектов
  mWireNameProp.mFont   = 0;
  mWireNameProp.mSize   = 350;
  mWireNameProp.mDir    = da0;
  mWireNameProp.mHorz   = dhjLeft;
  mWireNameProp.mVert   = dvjMiddle;
  mWireNameProp.mMirror = 0;

  //Symbol pin properties
  mSymPinProp.mLayer.set( QString(LID0_PIN) );     //Свойства вывода символа
  mSymPinProp.mPinType = 0;

  //Symbol pin name properties
  mSymPinNameProp.mLayer.set( QString(LID0_PIN_NAME) );
  mSymPinNameProp.mFont   = 0;
  mSymPinNameProp.mSize   = 350;
  mSymPinNameProp.mDir    = da0;
  mSymPinNameProp.mHorz   = dhjLeft;
  mSymPinNameProp.mVert   = dvjMiddle;
  mSymPinNameProp.mMirror = 0;

  //Symbol pin number properties
  mSymPinNumberProp.mLayer.set( QString(LID0_PIN_NUMBER) );
  mSymPinNumberProp.mFont   = 0;
  mSymPinNumberProp.mSize   = 350;
  mSymPinNumberProp.mDir    = da0;
  mSymPinNumberProp.mHorz   = dhjLeft;
  mSymPinNumberProp.mVert   = dvjMiddle;
  mSymPinNumberProp.mMirror = 0;

  //Part pin properties
  mPartPinProp.mLayer.set( QString(LID0_PIN LID1_TOP) );    //Свойства вывода корпуса
  mPartPinProp.mPinType = QString("h1");
  mPartPinProp.mSide    = stmThrough;

  //Part pin name properties
  mPartPinNameProp.mLayer.set( QString(LID0_PIN_NAME LID1_TOP) );    //Свойства имени вывода
  mPartPinNameProp.mFont   = 0;
  mPartPinNameProp.mSize   = 1000;
  mPartPinNameProp.mDir    = da0;
  mPartPinNameProp.mHorz   = dhjLeft;
  mPartPinNameProp.mVert   = dvjMiddle;
  mPartPinNameProp.mMirror = 0;

  //Part pin number properties
  mPartPinNumberProp.mLayer.set( QString(LID0_PIN_NUMBER LID1_TOP) );  //Свойства номера вывода
  mPartPinNumberProp.mFont   = 0;
  mPartPinNumberProp.mSize   = 1000;
  mPartPinNumberProp.mDir    = da0;
  mPartPinNumberProp.mHorz   = dhjLeft;
  mPartPinNumberProp.mVert   = dvjMiddle;
  mPartPinNumberProp.mMirror = 0;

  //Props of symbol implement
  mSymImpProp.mAngle = 0;
  mSymImpProp.mMirror = 0;

  //Props of part implement
  mPartImpProp.mAngle  = 0;
  mPartImpProp.mSide   = stmTop;

  //Symbol identifier (reference) aka D4.5
  mSymIdentProp.mLayer.set( QString(LID0_IDENT) );
  mSymIdentProp.mFont   = 0;
  mSymIdentProp.mSize   = 350;
  mSymIdentProp.mDir    = da0;
  mSymIdentProp.mHorz   = dhjCenter;
  mSymIdentProp.mVert   = dvjMiddle;
  mSymIdentProp.mMirror = 0;

  //Part identifier (reference) aka D4
  mPartIdentProp.mLayer.set( QString(LID0_IDENT LID1_TOP) );    //Layer of ident
  mPartIdentProp.mFont   = 0;
  mPartIdentProp.mSize   = 1000;
  mPartIdentProp.mDir    = da0;
  mPartIdentProp.mHorz   = dhjCenter;
  mPartIdentProp.mVert   = dvjMiddle;
  mPartIdentProp.mMirror = 0;

  //Value properties for symbol
  mSymValueProp.mLayer.set( QString(LID0_VALUE) );
  mSymValueProp.mFont   = 0;
  mSymValueProp.mSize   = 350;
  mSymValueProp.mDir    = da0;
  mSymValueProp.mHorz   = dhjCenter;
  mSymValueProp.mVert   = dvjMiddle;
  mSymValueProp.mMirror = 0;

  //Value properties for part
  mPartValueProp.mLayer.set( QString(LID0_VALUE LID1_TOP) );
  mPartValueProp.mFont   = 0;
  mPartValueProp.mSize   = 1000;
  mPartValueProp.mDir    = da0;
  mPartValueProp.mHorz   = dhjCenter;
  mPartValueProp.mVert   = dvjMiddle;
  mPartValueProp.mMirror = 0;


  //Road properties [Свойства дорожки]
  mRoadProp.mWidth       = 400;
  mRoadProp.mStratum     = stmTop;
  mRoadProp.mNetName     = "";

  //Polygon properties
  mPolygonProp.mGap      = 500;
  mPolygonProp.mStratum  = stmTop;
  mPolygonProp.mNetName  = "";

  //TODO D011 setup all prop defaults
  //Via properties [Свойства переходных отверстий]
  mViaProp.mStratum      = stmThrough;
  mViaProp.mPadType      = sdEnvir->mViaType;
  mViaProp.mNetName      = "";
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

    mLineProp.json( "line", js );       //Свойства линейных объектов
    mTextProp.json( "text", js );       //Свойства текстовых объектов
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

  mLineProp.json( "line", js );       //Свойства линейных объектов
  mTextProp.json( "text", js );       //Свойства текстовых объектов
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

