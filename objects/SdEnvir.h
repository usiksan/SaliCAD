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

#include <QColor>

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
scGrid          = 8, //Цвет сетки
scLast          = 9;

class SdEnvir
  {
    static QColor mSysColors[scLast];
  public:
    static int      mDotSize;     //Размер точки соединений сегментов цепи
    static int      mDotWidth;    //Толщина линии точки соединения сегментов цепи
    static int      mSymPinSize;  //Размер перекрестья ножки символа
    static int      mPrtPinSize;  //Размер окружности ножки корпуса
    static int      mSmartSize;   //Размер разумной точки
    static int      mSmartWidth;  //Толщина линии разумной точки
    static bool     mIsSmart;     //Включение разумного режима
    static bool     mIsWireSmart; //Второй уровень разумного режима для цепей в схеме
    static quint64  mSmartMask;   //Маска разрешенных для поиска разумных точек
    static bool     mNetUnionOn;  //Выводить окно объединения каждый раз
    static int      mNetUnionMode;//Режим объединения цепей 0-нет, 1-XN, 2-все
    static bool     mEnableComp;  //Разрешить операции с компонентами или заблокировать
    DBool       enableNet;   //Разрешить операции с цепями
    DBool       enablePic;   //Разрешить операции с картинкой
    DInt32      viaSize;     //Размер переходного отверстия по умолчанию
    DBool       showRatNet;  //Показывать резинки
    DBool       bShowRemark;           //Показывать всплывающие подсказки над компонентами
    DBool       bShowMessageRemark;    //Показывать информацию в строке состояния
    DInt32      placeMode;             //Режим размещения компонентов
    DInt32      minViewGrid;           //Минимальный видимый размер сетки
    DInt32      undoSize;              //Размер буфера отмены
    DInt32      cursorView;            //Вид перекрестья курсора
    DInt32      smallCursorSize;       //Размер малого курсора в пикселах
    DBool       bShowCursor;           //Показывать курсор
    DInt32      traseDotSize;          //Размер точки, показывающей цепь

    static QColor getSysColor( int colorId );
    static void   setSysColor( int colorId, QColor color );

    static void   loadEnvir();
    static void   saveEnvir();
  };

#endif // SDENVIR_H
