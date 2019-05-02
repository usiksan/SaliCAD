/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Work environment for individual user
*/

#ifndef SDENVIR_H
#define SDENVIR_H

#include "SdConfig.h"
#include "SdLayer.h"
#include "SdLayerCache.h"
#include "SdSnapInfo.h"
#include "SdUtil.h"
#include "SdPadAssociation.h"
#include "SdRuleBlock.h"
#include <QColor>
#include <QMap>
#include <QPointF>
#include <QList>


#define scPureBlack     -2 //Pure black color [Чисто черный цвет]
#define scUnvisible     -1 //Always invisible elements [Всегда невидимые элементы]
#define scLocked         0 //Color for locked elements [Цвет заблокированных элементов]
#define scSelected       1 //Color for selected elements [Цвет выбранных элементов]
#define scEnter          2 //Enter color [Цвет ввода]
#define scSmart          3 //Smart mark color [Цвет разумного режима]
#define scGraphBack      4 //Graphics editor background color [Цвет фона графического редактора]
#define scRatNet         5 //Rat net color [Цвет резинок]
#define scCursor         6 //Cursor color [Цвет курсора]
#define scTraseNet       7 //Color for currently traced net pads [Цвет трассируемой цепи]
#define scGrid           8 //Grid color [Цвет сетки]
#define scRuleErrors     9 //Rule errors color
#define scCatchPoint    10 //Catch point color in road enter mode. Catch point is point nearest current cursor postion for best next enter.
#define scLast          11


//Вид перекрестья курсора
#define dcvNone          0  //Cursor is invisible [Курсор не виден]
#define dcvSmall         1  //Small cursor crosshair [Малый курсор]
#define dcvFull          2  //Crosshair to fill view [Полный курсор]
#define dcvSmall45       3  //Small cursor crosshair with angle 45 [Малый под углом 45]
#define dcvFull45        4  //Crosshair to fill view with angle 45 [Полный под углом 45]
#define dcvLast          5

//Версия SdEnvir
#define SdEnvirVersion  (36 + FONT_COUNT)


class SdEnvir
  {
    QColor          mSysColors[scLast];    //System color table
    QString         mFonts[FONT_COUNT];    //System font table
  public:
    SdLayerPtrTable mLayerTable;           //Layers table [Таблица слоев]
    int             mDotSize;              //Размер точки соединений сегментов цепи
    int             mDotWidth;             //Толщина линии точки соединения сегментов цепи
    int             mSymPinSize;           //Размер перекрестья ножки символа
    int             mPartPinSize;          //Размер окружности ножки корпуса
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
    QString         mViaType;              //Default via type [Тип переходного отверстия по умолчанию]
    bool            mShowRatNet;           //Показывать резинки
    bool            mShowRemark;           //Показывать всплывающие подсказки над компонентами
    bool            mShowMessageRemark;    //Показывать информацию в строке состояния
    int             mPlaceMode;            //Режим размещения компонентов
    int             mMinViewGrid;          //Минимальный видимый размер сетки
    int             mUndoSize;             //Undo buffer size [Размер буфера отмены]
    int             mCursorView;           //Cursor crosshair type [Вид перекрестья курсора]
    int             mSmallCursorSize;      //Small cursor size in pixels [Размер малого курсора в пикселах]
    bool            mCursorShow;           //Show cursor [Показывать курсор]
    int             mTraseDotSize;         //Размер точки, показывающей цепь
                                           //PPM показывает сколько физической величины приходится на одну логическую единицу
    double          mSchPPM;               //Коэффициент преобразования в физическую величину в схемном редакторе
    double          mPrtPPM;               //Коэффициент преобразования в физическую величину в конструкциях
    bool            mGridSyncXY;           //Syncronisated edition x and grid steps
    bool            mGridShow;             //Включение сетки
    bool            mCursorAlignGrid;      //Включение движения курсора по сетке
    bool            mCenterCursor;         //Центровать курсор при увеличении и уменьшении
    QString         mHomePath;             //Current user home path [Каталог пользователя]
    QString         mLibraryPath;          //Library path [Каталог библиотек]
    QString         mPatternPath;          //Каталог шаблонов
    QString         mCategoryPath;         //Category hierarchy file
    QList<QPointF>  mGridHistory;          //Previous grid history
    SdRuleBlock     mDefaultRules;         //Default rules for pcb
    bool            mShowRuleErrors;       //If true then over pcb shows rule error indicators as rectangles
    bool            mShowFields;           //If true then draw fields as fields names else draw fields as values
    bool            mShowPads;             //If true then draw pads in part editor
    double          mPolygonOpacity;       //Polygons draws with this opacity
    bool            mAutoRemoveRoadLoop;   //Automatic detect and remove loops when enter roads


    bool            mGuiderEnabled;        //Флаг разрешения/запрещения путеводителя
    int             mGuiderPosition;       //Положение разделителя путеводителя
    bool            mProjectEnabled;       //Флаг разрешения/запрещения проекта
    int             mProjectPosition;      //Позиция разделителя проекта
    bool            mProjectRemoveEnabled; //Разрешение автоматического запрещения проекта
    int             mProjectRemoveTime;    //Время удержания проекта
    double          mWidthStep;            //Шаг изменения толщины
    double          mTextSizeStep;         //Шаг изменения высоты текста
    bool            mCreateBack;           //Создавать BAK файл при сохранении

    //Not saved
    //Cashed layers for stratum
    SdLayerCache      mCacheForPad;
    SdLayerCache      mCacheForMask;
    SdLayerCache      mCacheForStencil;
    SdLayerCache      mCacheForHole;
    SdLayerCache      mCacheForRoad;
    SdLayerCache      mCacheForPolygon;
    SdLayerCache      mCacheForBoundary;
    SdLayerCache      mCacheForKeepout;
    SdPadAssociation *mPadStack;


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

    //Clear stratum layer association cashe
    void     resetForCache();

    //Set layer pair
    void     setPair( QString idTop, QString idBot );

    //Reset "usage" layer flag for all layers
    void     resetLayerUsage();
    //Set "usage" layer flag for stratum layers from caches
    void     setLayerUsage( int stratumCount );

    //Layer id to name translation service
    QString  layerId2NameLevel0(QString lid0 );
    QString  layerId2NameLevel1( QString lid1 );

    QString  toPhisSchematic( int val ) const { return SdUtil::log2physStr(val,mSchPPM); }
    int      fromPhisSchematic( const QString str ) const { return SdUtil::phys2log(str,mSchPPM); }

    QString  toPhisPcb( int val ) const { return SdUtil::log2physStr(val,mPrtPPM); }
    int      fromPhisPcb( const QString str ) const { return SdUtil::phys2log(str,mPrtPPM); }

    SdPad    getPad( const QString pinType );
    void     resetPads();

    //Return current language settings
    static   QString languageGet();
  private:
    void deleteLayers();
    void addLayer( SdLayer *layer );
    void addLayerId(const QString layerId, unsigned ccolor , SdLayerTrace st, int stratum);
  };

extern SdEnvir *sdEnvir;

#endif // SDENVIR_H
