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

//
#define dleOrtho         0
#define dle45degree      1
#define dleAnyDegree     2

//Side constant
#define dsNone           0
#define dsComp           0x1
#define dsSold           0x2
#define dsBoth           0x3
#define dsInner          0x4
#define dsThrow          0x7

struct SdProp
  {
    SdPropLine    mLineProp;       //Свойства линейных объектов
    SdPropText    mTextProp;       //Свойства текстовых объектов
    SdPropLine    mWireProp;       //Свойства сегментов цепей схемы
    SdPropText    mWireNameProp;   //Свойства имени цепи
    SdPropSymPin  mSymPinProp;     //Свойства вывода символа
    SdPropText    mSymPinNameProp;    //Свойства имени вывода
    SdPropText    mSymPinNumberProp;  //Свойства номера вывода
    SdPropPartPin mPartPinProp;    //Свойства вывода корпуса
    SdPropText    mPartPinNameProp;    //Свойства имени вывода
    SdPropText    mPartPinNumberProp;  //Свойства номера вывода
    SdPropSymImp  mSymImpProp;     //Свойства вхождения символа
    SdPropPartImp mPartImpProp;    //Свойства вхождения корпуса
    SdPropText    mSymIdentProp;   //Свойства идентификатора символа
    SdPropText    mPartIdentProp;  //Свойства идентификатора корпуса
    SdPropRoad    mRoadProp;       //Свойства дорожки
//    SdViaProp    viaProp;        //Свойства переходных отверстий
    SdPropText    mTextDocProp;    //Свойства текста в текстовых документах

    int           mLineEnterType;
    int           mWireEnterType;
    QString       mWireName;       //Имя цепи
    bool          mTextBold;       //Жирный шрифт
    bool          mTextItalic;     //Наклонный текст
    bool          mTextUderline;   //Подчеркивание текста

    int           mWireNumber;     //Порядковый номер цепи (для формирования имени по умолчанию)
    int           mMaskSide;       //Маска, определяющая возможность выбора компонентов

//    DTraseRules traseRules;     //Правила трассировки


    SdProp();

    void Clear();
  };

extern SdProp *sdGlobalProp;

#endif // SDPROP_H
