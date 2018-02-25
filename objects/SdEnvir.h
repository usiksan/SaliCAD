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
#include "SdSnapInfo.h"
#include <QColor>
#include <QMap>
#include <QPointF>
#include <QList>


#define scPureBlack     -2 //Чисто черный цвет
#define scUnvisible     -1 //Всегда невидимые элементы
#define scLocked         0 //Цвет заблокированных элементов
#define scSelected       1 //Цвет выбранных элементов
#define scEnter          2 //Цвет ввода
#define scSmart          3 //Цвет разумного режима
#define scGraphBack      4 //Цвет фона графического редактора
#define scRatNet         5 //Цвет резинок
#define scCursor         6 //Цвет курсора
#define scTraseNet       7 //Цвет трассируемой цепи
#define scGrid           8 //Цвет сетки
#define scNotLinked      9 //Color for not linked objects
#define scLast          10


//Вид перекрестья курсора
#define dcvNone          0  //Курсор не виден
#define dcvSmall         1  //Малый курсор
#define dcvFull          2  //Полный курсор
#define dcvSmall45       3  //Малый под углом 45
#define dcvFull45        4  //Полный под углом 45
#define dcvLast          5

//Версия SdEnvir
#define SdEnvirVersion  (21 + FONT_COUNT)

typedef QMap<QString,SdLayerPtr> SdLayerPtrTable;


class SdEnvir
  {
    QColor          mSysColors[scLast];    //System color table
    QString         mFonts[FONT_COUNT];    //System font table
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
    SdSnapMask      mSmartMask;            //Маска разрешенных для поиска разумных точек
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
    bool            mCursorShow;           //Показывать курсор
    int             mTraseDotSize;         //Размер точки, показывающей цепь
                                           //PPM показывает сколько физической величины приходится на одну логическую единицу
    double          mSchPPM;               //Коэффициент преобразования в физическую величину в схемном редакторе
    double          mPrtPPM;               //Коэффициент преобразования в физическую величину в конструкциях
    bool            mGridSyncXY;           //Syncronisated edition x and grid steps
    bool            mGridShow;             //Включение сетки
    bool            mCursorAlignGrid;           //Включение движения курсора по сетке
    bool            mCenterCursor;         //Центровать курсор при увеличении и уменьшении
    QString         mHomePath;             //Каталог пользователя
    QString         mLibraryPath;          //Каталог библиотек
    QString         mPatternPath;          //Каталог шаблонов
    QString         mPadStackFile;         //Файл контактных площадок
    QString         mPadStackObject;       //Объект содержащий контактные площадки
    QList<QPointF>  mGridHistory;          //Previous grid history


    bool            mGuiderEnabled;        //Флаг разрешения/запрещения путеводителя
    int             mGuiderPosition;       //Положение разделителя путеводителя
    bool            mProjectEnabled;       //Флаг разрешения/запрещения проекта
    int             mProjectPosition;      //Позиция разделителя проекта
    bool            mProjectRemoveEnabled; //Разрешение автоматического запрещения проекта
    int             mProjectRemoveTime;    //Время удержания проекта
    QString         mLastFile;             //Последний файл пользователя
    double          mWidthStep;            //Шаг изменения толщины
    double          mTextSizeStep;         //Шаг изменения высоты текста
    bool            mMaxFrame;             //Истина, если главное окно на весь экран
    bool            mAutoPads;             //Автоматическая расстановка КП
    bool            mCreateBack;           //Создавать BAK файл при сохранении
    double          mPolyClear;             //Зазор между дорожками и полигоном
    bool            mShowConflict;         //Показывать конфликты трассировки

    SdEnvir();
    ~SdEnvir();

    QColor   getSysColor( int colorId );
    void     setSysColor( int colorId, QColor color );

    QString  getSysFont( int fontId );
    void     setSysFont( int fontId, const QString fontName );

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
