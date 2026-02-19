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

#ifndef SDPROP_H
#define SDPROP_H

#include "SdPropLine.h"
#include "SdPropText.h"
#include "SdPropSymPin.h"
#include "SdPropPartPin.h"
#include "SdPropSymImp.h"
#include "SdPropPartImp.h"
#include "SdPropRoad.h"
#include "SdPropPolygon.h"
#include "SdPropVia.h"

//
#define dleOrtho         0
#define dle45degree      1
#define dleAnyDegree     2

struct SdProp
  {
    //Symbol
    SdPropLine    mSymLineProp;       //Свойства линейных объектов
    SdPropText    mSymTextProp;       //Свойства текстовых объектов
    SdPropSymPin  mSymPinProp;     //Свойства вывода символа
    SdPropText    mSymPinNameProp;    //Свойства имени вывода
    SdPropText    mSymPinNumberProp;  //Свойства номера вывода
    SdPropText    mSymIdentProp;   //Свойства идентификатора символа
    SdPropText    mSymValueProp;   //Value properties for symbol

    //Schematic
    SdPropLine    mSchLineProp;       //Свойства линейных объектов
    SdPropText    mSchTextProp;       //Свойства текстовых объектов
    SdPropLine    mWireProp;       //Свойства сегментов цепей схемы
    SdPropText    mWireNameProp;   //Свойства имени цепи
    SdPropSymImp  mSymImpProp;     //Свойства вхождения символа

    //Part
    SdPropLine    mPartLineProp;       //Свойства линейных объектов
    SdPropText    mPartTextProp;       //Свойства текстовых объектов
    SdPropPartPin mPartPinProp;    //Свойства вывода корпуса
    SdPropText    mPartPinNameProp;    //Свойства имени вывода
    SdPropText    mPartPinNumberProp;  //Свойства номера вывода
    SdPropText    mPartIdentProp;  //Свойства идентификатора корпуса
    SdPropText    mPartValueProp;  //Value properties for part

    //PCB
    SdPropLine    mPcbLineProp;       //Свойства линейных объектов
    SdPropText    mPcbTextProp;       //Свойства текстовых объектов
    SdPropPartImp mPartImpProp;    //Свойства вхождения корпуса
    SdPropRoad    mRoadProp;       //Свойства дорожки
    SdPropPolygon mPolygonProp;        //Tracing polygon properties
    SdPropVia     mViaProp;        //Свойства переходных отверстий

    SdPropText    mTextDocProp;    //Свойства текста в текстовых документах

    int           mLineEnterType;
    int           mWireEnterType;
    SdPvString    mWireName;       //Имя цепи
    bool          mTextBold;       //Жирный шрифт
    bool          mTextItalic;     //Наклонный текст
    bool          mTextUderline;   //Подчеркивание текста

    int           mWireNumber;     //Порядковый номер цепи (для формирования имени по умолчанию)
    int           mMaskSide;       //Маска, определяющая возможность выбора компонентов
    int           mViaRule;        //!< Via creation rule (throught, hidden, blind)

//    DTraseRules traseRules;     //Правила трассировки

    const int     mPropVersion = 2;

    SdProp();

    void loadProp();
    void saveProp();

    SdPropLine *propLine( SdClass theClass );
    SdPropText *propText( SdClass theClass );
  };

extern SdProp *sdGlobalProp;

#endif // SDPROP_H
