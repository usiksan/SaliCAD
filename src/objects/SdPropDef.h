#ifndef SDPROPDEF_H
#define SDPROPDEF_H

#include "SdPropLine.h"
#include "SdPropText.h"
#include "SdPropSymPin.h"
#include "SdPropPartPin.h"
#include "SdPropSymImp.h"
#include "SdPropPartImp.h"
#include "SdPropRoad.h"
#include "SdPropPolygon.h"
#include "SdPropVia.h"

#include "SvLib/SvSingleton.h"

struct SdPropDef
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

    inline static const SdPropDef &get() { static SdPropDef obj; return obj; }

  private:
    SdPropDef();
  };

#endif // SDPROPDEF_H
