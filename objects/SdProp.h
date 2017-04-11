/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
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

struct SdProp
  {
    SdPropLine    mLineProp;       //Свойства линейных объектов
    SdPropText    mTextProp;       //Свойства текстовых объектов
    SdPropLine    mWireProp;       //Свойства сегментов цепей схемы
    SdPropText    mWireNameProp;   //Свойства имени цепи
    SdPropSymPin  mSymPinProp;     //Свойства вывода символа
    SdPropPartPin mPartPinProp;    //Свойства вывода корпуса
    SdPropText    mPinNameProp;    //Свойства имени вывода
    SdPropText    mPinNumberProp;  //Свойства номера вывода
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

#endif // SDPROP_H
