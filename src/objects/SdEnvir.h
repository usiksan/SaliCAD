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
#include "SvLib/SvSingleton.h"

#include <QColor>
#include <QMap>
#include <QPointF>
#include <QList>
#include <functional>


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
#define sc3dPadTop      11 //Color of top pad
#define sc3dPadBot      12 //Color of bottom pad
#define sc3dPadHole     13 //Color of pad holes
#define sc3dPcb         14 //Color of pcb
#define scLast          15


//Вид перекрестья курсора
#define dcvNone          0  //Cursor is invisible [Курсор не виден]
#define dcvSmall         1  //Small cursor crosshair [Малый курсор]
#define dcvFull          2  //Crosshair to fill view [Полный курсор]
#define dcvSmall45       3  //Small cursor crosshair with angle 45 [Малый под углом 45]
#define dcvFull45        4  //Crosshair to fill view with angle 45 [Полный под углом 45]
#define dcvLast          5

//Версия SdEnvir
#define SdEnvirVersion  (41 + FONT_COUNT)


class SdEnvir
  {
    QColor          mSysColors[scLast];    //System color table
    QString         mFonts[FONT_COUNT];    //System font table
    SdLayerPtrMap   mLayerTable;           //Layers table [Таблица слоев]
    double          mSchPPM;               //Коэффициент преобразования в физическую величину в схемном редакторе
    double          mPrtPPM;               //Коэффициент преобразования в физическую величину в конструкциях

    QList<QPointF>  mGridHistory;          //Previous grid history
    SdRuleBlock     mDefaultRules;         //Default rules for pcb

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
  public:
    ~SdEnvir();

    SV_SINGLETON( SdEnvir )


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
    bool            mGridSyncXY;           //Syncronisated edition x and grid steps
    bool            mGridShow;             //Включение сетки
    bool            mCursorAlignGrid;      //Включение движения курсора по сетке
    bool            mCenterCursor;         //Центровать курсор при увеличении и уменьшении

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

    //This path depresate
    QString mPatternPath;

    //!
    //! \brief getSysColor Returns system color by its id
    //! \param colorId     Id of color
    //! \return            System color
    //!
    QColor   getSysColor( int colorId );

    //!
    //! \brief setSysColor Sets system color
    //! \param colorId     Id of system color
    //! \param color       New color value
    //!
    void     setSysColor( int colorId, QColor color );



    //!
    //! \brief getSysFont Returns name of system font by its id
    //! \param fontId     System font id
    //! \return           Name of system font
    //!
    QString  getSysFont( int fontId );

    //!
    //! \brief setSysFont Setup new name of system font
    //! \param fontId     System font id whous name is need to setup
    //! \param fontName   New font name
    //!
    void     setSysFont( int fontId, const QString fontName );

    //!
    //! \brief ppmGet  Returns ppm in accordance classId
    //! \param classId Class of object for which editor return ppm
    //! \return        ppm
    //!
    double   ppmGet( SdClass classId ) const;



    //!
    //! \brief loadEnvir Loads environment from user settings
    //!
    void     loadEnvir();

    //!
    //! \brief saveEnvir Saves environment to user settings
    //!
    void     saveEnvir();

    //!
    //! \brief defaultEnvir Creates environment with default values
    //!
    void     defaultEnvir();



    //!
    //! \brief layerGet Get existing layer, if it is not exist - then it's created as default
    //! \param id       Id of needed layer
    //! \return         Layer pointer
    //!
    SdLayer *layerGet( QString id );

    /*[] (SdLayer *layer) -> bool {

       }
       */
    //!
    //! \brief layerForEach - Iterates through all registered layers matching the specified class
    //! \param classMask   - Layer class filter (dctConstruct, dctSchematic, dctCommon or combination)
    //! \param fun1        - Callback function for each layer object. Return true to continue iteration, false to stop
    //!
    void     layerForEach(quint64 classMask, std::function<bool (SdLayer *)> fun1 );
    void     layerForEachConst( quint64 classMask, std::function<bool(SdLayer*)> fun1 ) const;

    void     layerRemove( const QString &layerId ) { if( mLayerTable.contains(layerId) ) { delete mLayerTable[layerId]; mLayerTable.remove( layerId ); } }

    int      layerCount() const { return mLayerTable.count(); }

    void     layerVisibleSet( const QStringList &layerIdTable );

    //!
    //! \brief gridForEach Executes function fun1 to index of grid table
    //!                    If intex < 0 then for all table
    //! \param index       Index of row for which need to be executed fun1, if < 0 then for all rows
    //! \param fun1        Function to execute
    //!
    void     gridForEach( int index, std::function<bool (QPointF)> fun1 ) const;

    //!
    //! \brief gridAppend Appends new record to grid table
    //! \param p          Record to append
    //!
    void     gridAppend( QPointF p );

    //Clear stratum layer association cashe
    void     resetForCache();

    SdLayer *layerVisibleForPad( int stratum )      { return mCacheForPad.getVisibleLayer( stratum ); }
    SdLayer *layerVisibleForMask( int stratum )     { return mCacheForMask.getVisibleLayer( stratum ); }
    SdLayer *layerVisibleForPolygon( int stratum )  { return mCacheForPolygon.getVisibleLayer( stratum ); }
    SdLayer *layerVisibleForStencil( int stratum )  { return mCacheForStencil.getVisibleLayer( stratum ); }
    SdLayer *layerVisibleForHole( int stratum )     { return mCacheForHole.getVisibleLayer( stratum ); }
    SdLayer *layerVisibleForRoad( int stratum )     { return mCacheForRoad.getVisibleLayer( stratum ); }
    SdLayer *layerVisibleForBoundary( int stratum ) { return mCacheForBoundary.getVisibleLayer( stratum ); }
    SdLayer *layerVisibleForKeepout( int stratum )  { return mCacheForKeepout.getVisibleLayer( stratum ); }

    //Set layer pair
    void     layerSetPair( QString idTop, QString idBot );

    //Reset "usage" layer flag for all layers
    void     resetLayerUsage();
    //Set "usage" layer flag for stratum layers from caches
    void     setLayerUsage( int stratumCount );

    QString  toPhisSchematic( int val ) const { return SdUtil::log2physStr(val,mSchPPM); }
    int      fromPhisSchematic( const QString str ) const { return SdUtil::phys2log(str,mSchPPM); }

    QString  toPhisPcb( int val ) const { return SdUtil::log2physStr(val,mPrtPPM); }
    int      fromPhisPcb( const QString str ) const { return SdUtil::phys2log(str,mPrtPPM); }

    SdPad    getPad( const QString pinType );
    void     resetPads();

    SdRuleBlock defaultRules() const { return mDefaultRules; }

    //Return current language settings
    static   QString languageGet();
  private:
    void deleteLayers();
    void addLayer( SdLayer *layer );
    void addLayerId(const SdLayerDescr &descr);
    void addLayerId(const QString &layerId, const QString &name, const QString &englishName, const SdLayerDescr &descr , int layerIndex);
    void layerSetPairFor( const QString &lid0 );
  };


#endif // SDENVIR_H
