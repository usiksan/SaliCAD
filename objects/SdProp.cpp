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

#include "SdProp.h"

SdProp *sdGlobalProp;

SdProp::SdProp()
  {
  //Сформировать свойства по умолчанию
  mLineProp.mLayer.set( QString( LID0_COMMON LID1_ELEM) );       //Свойства линейных объектов
  mLineProp.mType  = dltSolid;
  mLineProp.mWidth = 0;

  mTextProp.mLayer.set( QString( LID0_COMMON LID1_ELEM) );       //Свойства текстовых объектов
  mTextProp.mFont   = 0;
  mTextProp.mSize   = 35;
  mTextProp.mDir    = da0;
  mTextProp.mHorz   = dhjLeft;
  mTextProp.mVert   = dvjMiddle;
  mTextProp.mMirror = 0;
  //TODO setup all prop defaults
//  SdPropLine    mWireProp;       //Свойства сегментов цепей схемы
//  SdPropText    mWireNameProp;   //Свойства имени цепи
  //Symbol pin properties
  mSymPinProp.mLayer.set( QString(LID0_SCHEMATIC LID1_PIN) );     //Свойства вывода символа
  mSymPinProp.mPinType = 0;
  //Symbol pin name properties
  mSymPinNameProp.mLayer.set( QString(LID0_SCHEMATIC LID1_PIN_NAME) );
  mSymPinNameProp.mFont   = 0;
  mSymPinNameProp.mSize   = 35;
  mSymPinNameProp.mDir    = da0;
  mSymPinNameProp.mHorz   = dhjLeft;
  mSymPinNameProp.mVert   = dvjMiddle;
  mSymPinNameProp.mMirror = 0;
  //Symbol pin number properties
  mSymPinNumberProp.mLayer.set( QString(LID0_SCHEMATIC LID1_PIN_NUMBER) );
  mSymPinNumberProp.mFont   = 0;
  mSymPinNumberProp.mSize   = 35;
  mSymPinNumberProp.mDir    = da0;
  mSymPinNumberProp.mHorz   = dhjLeft;
  mSymPinNumberProp.mVert   = dvjMiddle;
  mSymPinNumberProp.mMirror = 0;

  //Part pin properties
  mPartPinProp.mLayer.set( QString(LID0_PCB_TOP LID1_PIN ) );    //Свойства вывода корпуса
  mPartPinProp.mPinType = QString("h1");
  mPartPinProp.mSide = dsThrow;
  //Part pin name properties
  mPartPinNameProp.mLayer.set( QString(LID0_PCB_TOP LID1_PIN_NAME) );    //Свойства имени вывода
  mPartPinNameProp.mFont   = 0;
  mPartPinNameProp.mSize   = 1000;
  mPartPinNameProp.mDir    = da0;
  mPartPinNameProp.mHorz   = dhjLeft;
  mPartPinNameProp.mVert   = dvjMiddle;
  mPartPinNameProp.mMirror = 0;
  //Part pin number properties
  mPartPinNumberProp.mLayer.set( QString(LID0_PCB_TOP LID1_PIN_NUMBER) );  //Свойства номера вывода
  mPartPinNumberProp.mFont   = 0;
  mPartPinNumberProp.mSize   = 1000;
  mPartPinNumberProp.mDir    = da0;
  mPartPinNumberProp.mHorz   = dhjLeft;
  mPartPinNumberProp.mVert   = dvjMiddle;
  mPartPinNumberProp.mMirror = 0;

//  SdPropSymImp  mSymImpProp;     //Свойства вхождения символа
//  SdPropPartImp mPartImpProp;    //Свойства вхождения корпуса
//  SdPropText    mSymIdentProp;   //Свойства идентификатора символа
//  SdPropText    mPartIdentProp;  //Свойства идентификатора корпуса
//  SdPropRoad    mRoadProp;       //Свойства дорожки
////    SdViaProp    viaProp;        //Свойства переходных отверстий
//  SdPropText    mTextDocProp;    //Свойства текста в текстовых документах

  mLineEnterType     = dleOrtho;       //By default orthogonal vertex line
//  int           mWireEnterType;
//  QString       mWireName;       //Имя цепи
//  bool          mTextBold;       //Жирный шрифт
//  bool          mTextItalic;     //Наклонный текст
//  bool          mTextUderline;   //Подчеркивание текста

//  int           mWireNumber;     //Порядковый номер цепи (для формирования имени по умолчанию)
//  int           mMaskSide;       //Маска, определяющая возможность выбора компонентов

  }

