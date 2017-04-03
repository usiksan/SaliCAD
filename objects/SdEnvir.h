/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDENVIR_H
#define SDENVIR_H

#include "SdConfig.h"
#include "SdLayer.h"
#include <QColor>
#include <QMap>

const int
scPureBlack     = -2, //Чисто черный цвет
scUnvisible     = -1,  //Всегда невидимые элементы
scLocked        = 0,  //Цвет заблокированных элементов
scSelected      = 1,  //Цвет выбранных элементов
scEnter         = 2,  //Цвет ввода
scSmart         = 3,  //Цвет разумного режима
scGraphBack     = 4,  //Цвет фона графического редактора
scRatNet        = 5,  //Цвет резинок
scCursor        = 6,  //Цвет курсора
scTraseNet      = 7,  //Цвет трассируемой цепи
scGrid          = 8,  //Цвет сетки
scLast          = 9;


//Вид перекрестья курсора
const int
  dcvNone    = 0,  //Курсор не виден
  dcvSmall   = 1,  //Малый курсор
  dcvFull    = 2,  //Полный курсор
  dcvSmall45 = 3,  //Малый под углом 45
  dcvFull45  = 4,  //Полный под углом 45
  dcvLast    = 5;

//Версия SdEnvir
const int SdEnvirVersion = 1;

typedef QMap<QString,SdLayerPtr> SdLayerPtrTable;


class SdEnvir
  {
    QColor          mSysColors[scLast];    //Системные цвета
  public:
    SdLayerPtrTable mLayerTable;           //Таблица слоев
    int             mDotSize;              //Размер точки соединений сегментов цепи
    int             mDotWidth;             //Толщина линии точки соединения сегментов цепи
    int             mSymPinSize;           //Размер перекрестья ножки символа
    int             mPrtPinSize;           //Размер окружности ножки корпуса
    int             mSmartSize;            //Размер разумной точки
    int             mSmartWidth;           //Толщина линии разумной точки
    bool            mIsSmart;              //Включение разумного режима
    bool            mIsWireSmart;          //Второй уровень разумного режима для цепей в схеме
    quint64         mSmartMask;            //Маска разрешенных для поиска разумных точек
    bool            mNetUnionOn;           //Выводить окно объединения каждый раз
    int             mNetUnionMode;         //Режим объединения цепей 0-нет, 1-XN, 2-все
    bool            mEnableComp;           //Разрешить операции с компонентами или заблокировать
    bool            mEnableNet;            //Разрешить операции с цепями
    bool            mEnablePic;            //Разрешить операции с картинкой
    int             mViaSize;              //Размер переходного отверстия по умолчанию
    bool            mShowRatNet;           //Показывать резинки
    bool            mShowRemark;           //Показывать всплывающие подсказки над компонентами
    bool            mShowMessageRemark;    //Показывать информацию в строке состояния
    int             mPlaceMode;            //Режим размещения компонентов
    int             mMinViewGrid;          //Минимальный видимый размер сетки
    int             mUndoSize;             //Размер буфера отмены
    int             mCursorView;           //Вид перекрестья курсора
    int             mSmallCursorSize;      //Размер малого курсора в пикселах
    bool            mShowCursor;           //Показывать курсор
    int             mTraseDotSize;         //Размер точки, показывающей цепь

    SdEnvir();
    ~SdEnvir();

    QColor   getSysColor( int colorId );
    void     setSysColor( int colorId, QColor color );

    void     loadEnvir();
    void     saveEnvir();
    void     defaultEnvir();

    //Get existing layer, if it is not exist - then it's created as default
    SdLayer *getLayer( QString id );

    //Set layer pair
    void     setPair( QString idTop, QString idBot );

  private:
    void deleteLayers();
    void addLayer( SdLayer *layer );
  };

extern SdEnvir *sdEnvir;

#endif // SDENVIR_H
