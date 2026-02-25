#include "SdPropDef.h"
#include "SdEnvir.h"

SdPropDef::SdPropDef()
  {
  //Сформировать свойства по умолчанию
  //Symbol
  mSymLineProp.mLayer.set( QString( LID0_COMPONENT ) );       //Свойства линейных объектов
  mSymLineProp.mType  = dltSolid;
  mSymLineProp.mWidth = 0;

  mSymTextProp.mLayer.set( QString( LID0_COMPONENT ) );       //Свойства текстовых объектов
  mSymTextProp.mFont   = 0;
  mSymTextProp.mSize   = 350;
  mSymTextProp.mDir    = da0;
  mSymTextProp.mHorz   = dhjCenter;
  mSymTextProp.mVert   = dvjMiddle;
  mSymTextProp.mMirror = 0;

  //Sheet
  mSchLineProp.mLayer.set( QString( LID0_BUS ) );       //Свойства линейных объектов
  mSchLineProp.mType  = dltSolid;
  mSchLineProp.mWidth = 0;

  mSchTextProp.mLayer.set( QString( LID0_FORM ) );       //Свойства текстовых объектов
  mSchTextProp.mFont   = 0;
  mSchTextProp.mSize   = 350;
  mSchTextProp.mDir    = da0;
  mSchTextProp.mHorz   = dhjLeft;
  mSchTextProp.mVert   = dvjMiddle;
  mSchTextProp.mMirror = 0;

  //Part
  mPartLineProp.mLayer.set( QString( LID0_COMPONENT LID1_TOP ) );       //Свойства линейных объектов
  mPartLineProp.mType  = dltSolid;
  mPartLineProp.mWidth = 0;

  mPartTextProp.mLayer.set( QString( LID0_COMPONENT LID1_TOP ) );       //Свойства текстовых объектов
  mPartTextProp.mFont   = 0;
  mPartTextProp.mSize   = 2000;
  mPartTextProp.mDir    = da0;
  mPartTextProp.mHorz   = dhjLeft;
  mPartTextProp.mVert   = dvjMiddle;
  mPartTextProp.mMirror = 0;

  //Pcb
  mPcbLineProp.mLayer.set( QString( LID0_REMARK ) );       //Свойства линейных объектов
  mPcbLineProp.mType  = dltSolid;
  mPcbLineProp.mWidth = 0;

  mPcbTextProp.mLayer.set( QString( LID0_REMARK ) );       //Свойства текстовых объектов
  mPcbTextProp.mFont   = 0;
  mPcbTextProp.mSize   = 350;
  mPcbTextProp.mDir    = da0;
  mPcbTextProp.mHorz   = dhjLeft;
  mPcbTextProp.mVert   = dvjMiddle;
  mPcbTextProp.mMirror = 0;


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
  mPartIdentProp.mSize   = 2000;
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
  mPartValueProp.mSize   = 2000;
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
  mViaProp.mPadType      = SdEnvir::instance()->mViaType;
  mViaProp.mNetName      = "";
  }

