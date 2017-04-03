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
#include "SdTextProp.h"
#include "SdSymPinProp.h"
#include "SdPartPinProp.h"
#include "SdSymImpProp.h"
#include "SdPartImpProp.h"
#include "SdRoadProp.h"

struct SdProp
  {
    SdPropLine    mLineProp;       //Свойства линейных объектов
    SdTextProp    mTextProp;       //Свойства текстовых объектов
    SdPropLine    mWireProp;       //Свойства сегментов цепей схемы
    SdTextProp    mWireNameProp;   //Свойства имени цепи
    SdSymPinProp  mSymPinProp;     //Свойства вывода символа
    SdPartPinProp mPartPinProp;    //Свойства вывода корпуса
    SdTextProp    mPinNameProp;    //Свойства имени вывода
    SdTextProp    mPinNumberProp;  //Свойства номера вывода
    SdSymImpProp  mSymImpProp;     //Свойства вхождения символа
    SdPartImpProp mPartImpProp;    //Свойства вхождения корпуса
    SdTextProp    mSymIdentProp;   //Свойства идентификатора символа
    SdTextProp    mPartIdentProp;  //Свойства идентификатора корпуса
    SdRoadProp    mRoadProp;       //Свойства дорожки
//    SdViaProp    viaProp;        //Свойства переходных отверстий
    SdTextProp    mTextDocProp;    //Свойства текста в текстовых документах

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
