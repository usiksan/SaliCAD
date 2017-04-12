/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdProp.h"

SdProp *sdGlobalProp;

SdProp::SdProp()
  {
  //Сформировать свойства по умолчанию
  mLineProp.mLayer.set( QString( LID0_COMMON LID1_ELEM) );       //Свойства линейных объектов
  mLineProp.mType = dltSolid;
  mLineProp.mWidth = 0;

  //TODO setup all prop defaults
  //mTextProp.;       //Свойства текстовых объектов
//  SdPropLine    mWireProp;       //Свойства сегментов цепей схемы
//  SdPropText    mWireNameProp;   //Свойства имени цепи
//  SdPropSymPin  mSymPinProp;     //Свойства вывода символа
//  SdPropPartPin mPartPinProp;    //Свойства вывода корпуса
//  SdPropText    mPinNameProp;    //Свойства имени вывода
//  SdPropText    mPinNumberProp;  //Свойства номера вывода
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

