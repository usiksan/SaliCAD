/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdConfig.h"
#include "SdEnvir.h"
#include <QSettings>
#include <QByteArray>
#include <QDataStream>

SdEnvir *sdEnvir;

SdEnvir::SdEnvir()
  {
  loadEnvir();
  }


QColor SdEnvir::getSysColor(int colorId)
  {
  if( colorId == scUnvisible )
    return QColor(0,0,0,0);
  //Если вне диапазона, то вернуть черный цвет
  if( colorId < 0 || colorId >= scLast )
    return QColor(0,0,0);
  return mSysColors[colorId];
  }




void SdEnvir::setSysColor(int colorId, QColor color)
  {
  if( colorId >= 0 && colorId < scLast )
    mSysColors[colorId] = color;
  }




void SdEnvir::loadEnvir()
  {
  QSettings s;
  if( s.value(QString(SDK_ENVIR_VERSION),QVariant((int)0)).toInt() == SdEnvirVersion ) {
    //Загружаем значения
    QByteArray ar = s.value(QString(SDK_ENVIR)).toByteArray();
    QDataStream in(ar);
    in.readRawData( (char*)this, sizeof(SdEnvir) );
    }
  else defaultEnvir();
  }




void SdEnvir::saveEnvir()
  {
  QByteArray ar;
  QDataStream os( &ar, QIODevice::WriteOnly );
  os.writeRawData( (const char*)this, sizeof(SdEnvir) );

  QSettings s;
  s.setValue( QString(SDK_ENVIR_VERSION), QVariant(SdEnvirVersion) );
  s.setValue( QString(SDK_ENVIR), QVariant(ar) );
  }





void SdEnvir::defaultEnvir()
  {
  //Цвета по умолчанию
  mSysColors[scLocked]    = QColor(200,200,0);    //Цвет заблокированных элементов
  mSysColors[scSelected]  = QColor(150,150,150);  //Цвет выбранных элементов
  mSysColors[scEnter]     = QColor(65,65,65);     //Цвет ввода
  mSysColors[scSmart]     = QColor(36,27,99);     //Цвет разумного режима
  mSysColors[scGraphBack] = QColor(255,255,255);  //Цвет фона графического редактора
  mSysColors[scRatNet]    = QColor(30,30,30);     //Цвет резинок
  mSysColors[scCursor]    = QColor(0,0,0);        //Цвет курсора
  mSysColors[scTraseNet]  = QColor(0,0,0);        //Цвет трассируемой цепи
  mSysColors[scGrid]      = QColor(100,100,100);  //Цвет сетки

  //Значения глобальных переменных настройки по умолчанию
  mDotSize           = 30;             //Размер точки соединений сегментов цепи
  mDotWidth          = 60;             //Толщина линии точки соединения сегментов цепи
  mSymPinSize        = 100;            //Размер перекрестья ножки символа
  mPrtPinSize        = 500;            //Размер окружности ножки корпуса
  mSmartSize         = 5;              //Размер разумной точки (в пикселях)
  mSmartWidth        = 3;              //Толщина линии разумной точки
  mIsSmart           = true;           //Включение разумного режима
  mIsWireSmart       = true;           //Второй уровень разумного режима для цепей в схеме
  mSmartMask         = -1L;            //Маска разрешенных для поиска разумных точек
  mNetUnionOn        = true;           //Выводить окно объединения каждый раз
  mNetUnionMode      = 1;              //Режим объединения цепей 0-нет, 1-XN, 2-все
  mEnableComp        = true;           //Разрешить операции с компонентами или заблокировать
  mEnableNet         = true;           //Разрешить операции с цепями
  mEnablePic         = true;           //Разрешить операции с картинкой
  mViaSize           = 100;            //Размер переходного отверстия по умолчанию
  mShowRatNet        = true;           //Показывать резинки
  mShowRemark        = true;           //Показывать всплывающие подсказки над компонентами
  mShowMessageRemark = true;           //Показывать информацию в строке состояния
  mPlaceMode         = 0;              //Режим размещения компонентов
  mMinViewGrid       = 5;              //Минимальный видимый размер сетки в пикселях
  mUndoSize          = 100000;         //Размер буфера отмены
  mCursorView        = dcvSmall;       //Вид перекрестья курсора
  mSmallCursorSize   = 30;             //Размер малого курсора в пикселах
  mShowCursor        = true;           //Показывать курсор
  mTraseDotSize      = 10;             //Размер точки, показывающей цепь
  }
