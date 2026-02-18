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
#include "SdLayer.h"
#include "SvLib/SvDir.h"

#include <QSettings>
#include <QByteArray>
#include <QDataStream>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>

SdEnvir::SdEnvir() :
  mPadStack(nullptr)
  {
  }

SdEnvir::~SdEnvir()
  {
  deleteLayers();
  if( mPadStack != nullptr )
    delete mPadStack;
  }



//!
//! \brief getSysColor Returns system color by its id
//! \param colorId     Id of color
//! \return            System color
//!
QColor SdEnvir::getSysColor(int colorId)
  {
  if( colorId == scUnvisible )
    return QColor(0,0,0,0);
  //If color is outside index bound then return true black color [Если вне диапазона, то вернуть черный цвет]
  if( colorId < 0 || colorId >= scLast )
    return QColor(0,0,0);
  return mSysColors[colorId];
  }





//!
//! \brief setSysColor Sets system color
//! \param colorId     Id of system color
//! \param color       New color value
//!
void SdEnvir::setSysColor(int colorId, QColor color)
  {
  if( colorId >= 0 && colorId < scLast )
    mSysColors[colorId] = color;
  }





//!
//! \brief getSysFont Returns name of system font by its id
//! \param fontId     System font id
//! \return           Name of system font
//!
QString SdEnvir::getSysFont(int fontId)
  {
  if( fontId >= 0 && fontId < FONT_COUNT )
    return mFonts[fontId];
  return mFonts[0];
  }





//!
//! \brief setSysFont Setup new name of system font
//! \param fontId     System font id whous name is need to setup
//! \param fontName   New font name
//!
void SdEnvir::setSysFont(int fontId, const QString fontName)
  {
  if( fontId >= 0 && fontId < FONT_COUNT )
    mFonts[fontId] = fontName;
  }





//!
//! \brief ppmGet  Returns ppm in accordance classId
//! \param classId Class of object for which editor return ppm
//! \return        ppm
//!
double SdEnvir::ppmGet(SdClass classId) const
  {
  if( classId & dctConstruct )
    return mPrtPPM;
  return mSchPPM;
  }




//!
//! \brief loadEnvir Load environment from user settings
//!
void SdEnvir::loadEnvir()
  {
  QSettings s;
  if( s.value(QString(SDK_ENVIR_VERSION),QVariant( static_cast<int>(0) )).toInt() == SdEnvirVersion ) {
    //Загружаем значения
    QByteArray ar = s.value(QString(SDK_ENVIR)).toByteArray();
    QDataStream is(ar);

    //Прочитали цвета
    for( int i = 0; i < scLast; i++ )
      is >> mSysColors[i];

    //Read fonts
    for( int i = 0; i < FONT_COUNT; i++ )
      is >> mFonts[i];

    //Прочитали слои
    deleteLayers();
    int c;
    is >> c;
    for( int i = 0; i < c; i++ ) {
      QString id;
      is >> id;
      SdLayer *layer = layerGet(id);
      layer->read( is );
      }

    //Read other values
    is >> mDotSize              //Размер точки соединений сегментов цепи
       >> mDotWidth             //Толщина линии точки соединения сегментов цепи
       >> mSymPinSize           //Размер перекрестья ножки символа
       >> mPartPinSize           //Размер окружности ножки корпуса
       >> mSmartSize            //Размер разумной точки
       >> mSmartWidth           //Толщина линии разумной точки
       >> mIsSmart              //Включение разумного режима
       >> mIsWireSmart          //Второй уровень разумного режима для цепей в схеме
       >> mSmartMask            //Маска разрешенных для поиска разумных точек
       >> mNetUnionOn           //Выводить окно объединения каждый раз
       >> mNetUnionMode         //Режим объединения цепей 0-нет, 1-XN, 2-все
       >> mEnableComp           //Разрешить операции с компонентами или заблокировать
       >> mEnableNet            //Разрешить операции с цепями
       >> mEnablePic            //Разрешить операции с картинкой
       >> mViaType              //Размер переходного отверстия по умолчанию
       >> mShowRatNet           //Показывать резинки
       >> mShowRemark           //Показывать всплывающие подсказки над компонентами
       >> mShowMessageRemark    //Показывать информацию в строке состояния
       >> mPlaceMode            //Режим размещения компонентов
       >> mMinViewGrid          //Минимальный видимый размер сетки
       >> mUndoSize             //Размер буфера отмены
       >> mCursorView           //Вид перекрестья курсора
       >> mSmallCursorSize      //Размер малого курсора в пикселах
       >> mCursorShow           //Показывать курсор
       >> mTraseDotSize         //Размер точки, показывающей цепь
       >> mSchPPM               //Коэффициент преобразования в физическую величину в схемном редакторе
       >> mPrtPPM               //Коэффициент преобразования в физическую величину в конструкциях
       >> mGridSyncXY           //Syncronisated edition x and grid steps
       >> mGridShow             //Включение сетки
       >> mCursorAlignGrid           //Включение движения курсора по сетке
       >> mCenterCursor         //Центровать курсор при увеличении и уменьшении
       >> mGridHistory          //Previous grid history table
       >> mDefaultRules         //Default rules for pcb
       >> mShowRuleErrors       //If true then over pcb shows rule error indicators as rectangles
       >> mShowFields           //If true then draw fields as fields names else draw fields as values
       >> mShowPads             //If true then draw pads in part editor
       >> mPolygonOpacity       //Polygons draws with this opacity
       >> mAutoRemoveRoadLoop;   //Automatic detect and remove loops when enter roads
    }
  else defaultEnvir();

  //Reset stratum layers cache
  resetForCache();
  }




//!
//! \brief saveEnvir Saves environment to user settings
//!
void SdEnvir::saveEnvir()
  {
  QByteArray ar;
  QDataStream os( &ar, QIODevice::WriteOnly );

  //Write colors [Записали цвета]
  for( int i = 0; i < scLast; i++ )
    os << mSysColors[i];

  //Write fonts
  for( int i = 0; i < FONT_COUNT; i++ )
    os << mFonts[i];

  //Write layers [Записали слои]
  int c = mLayerTable.count();
  os << c;
  for( SdLayer *layer : std::as_const(mLayerTable) ) {
    os << layer->id();
    layer->write( os );
    }


  //Записали остальные значения
  os << mDotSize              //Размер точки соединений сегментов цепи
     << mDotWidth             //Толщина линии точки соединения сегментов цепи
     << mSymPinSize           //Размер перекрестья ножки символа
     << mPartPinSize          //Размер окружности ножки корпуса
     << mSmartSize            //Размер разумной точки
     << mSmartWidth           //Толщина линии разумной точки
     << mIsSmart              //Включение разумного режима
     << mIsWireSmart          //Второй уровень разумного режима для цепей в схеме
     << mSmartMask            //Маска разрешенных для поиска разумных точек
     << mNetUnionOn           //Выводить окно объединения каждый раз
     << mNetUnionMode         //Режим объединения цепей 0-нет, 1-XN, 2-все
     << mEnableComp           //Разрешить операции с компонентами или заблокировать
     << mEnableNet            //Разрешить операции с цепями
     << mEnablePic            //Разрешить операции с картинкой
     << mViaType              //Размер переходного отверстия по умолчанию
     << mShowRatNet           //Показывать резинки
     << mShowRemark           //Показывать всплывающие подсказки над компонентами
     << mShowMessageRemark    //Показывать информацию в строке состояния
     << mPlaceMode            //Режим размещения компонентов
     << mMinViewGrid          //Минимальный видимый размер сетки
     << mUndoSize             //Размер буфера отмены
     << mCursorView           //Вид перекрестья курсора
     << mSmallCursorSize      //Размер малого курсора в пикселах
     << mCursorShow           //Показывать курсор
     << mTraseDotSize         //Размер точки, показывающей цепь
     << mSchPPM               //Коэффициент преобразования в физическую величину в схемном редакторе
     << mPrtPPM               //Коэффициент преобразования в физическую величину в конструкциях
     << mGridSyncXY           //Syncronisated edition x and grid steps
     << mGridShow             //Включение сетки
     << mCursorAlignGrid      //Включение движения курсора по сетке
     << mCenterCursor         //Центровать курсор при увеличении и уменьшении
     << mGridHistory          //Previous grid history table
     << mDefaultRules         //Default rules for pcb
     << mShowRuleErrors       //If true then over pcb shows rule error indicators as rectangles
     << mShowFields           //If true then draw fields as fields names else draw fields as values
     << mShowPads             //If true then draw pads in part editor
     << mPolygonOpacity       //Polygons draws with this opacity
     << mAutoRemoveRoadLoop;   //Automatic detect and remove loops when enter roads

  QSettings s;
  s.setValue( QString(SDK_ENVIR_VERSION), QVariant(SdEnvirVersion) );
  s.setValue( QString(SDK_ENVIR), QVariant(ar) );
  }




//!
//! \brief defaultEnvir Creates environment with default values
//!
void SdEnvir::defaultEnvir()
  {
  //Цвета по умолчанию
  mSysColors[scLocked]     = QColor(200,200,0);    //Цвет заблокированных элементов
  mSysColors[scSelected]   = QColor(150,150,150);  //Цвет выбранных элементов
  mSysColors[scEnter]      = QColor(65,65,65);     //Цвет ввода
  mSysColors[scSmart]      = QColor(36,27,99);     //Цвет разумного режима
  mSysColors[scGraphBack]  = QColor(255,255,255);  //Цвет фона графического редактора
  mSysColors[scRatNet]     = QColor(30,30,30);     //Цвет резинок
  mSysColors[scCursor]     = QColor(0,0,0);        //Цвет курсора
  mSysColors[scTraseNet]   = QColor(0,0,0);        //Цвет трассируемой цепи
  mSysColors[scGrid]       = QColor(100,100,100);  //Цвет сетки
  mSysColors[scRuleErrors] = QColor(0xff,0xd8,0);  //Color for rectangles indicator for rule errors
  mSysColors[scCatchPoint] = QColor(200,10,50);    //Catch point color in road enter mode. Catch point is point nearest current cursor postion for best next enter.
  mSysColors[sc3dPadTop]   = QColor(255,255,0);    //Color of top pad
  mSysColors[sc3dPadBot]   = QColor(255,255,0);    //Color of bottom pad
  mSysColors[sc3dPadHole]  = QColor(255,255,0);    //Color of pad holes
  mSysColors[sc3dPcb]      = QColor(40,100,0);     //Color of pcb

  //Default fonts
  mFonts[0] = QString("FreeSerif");
  mFonts[1] = QString("FreeSans");
  mFonts[2] = QString("FreeMono");
  mFonts[3] = QString("Source Code Pro");
  mFonts[4] = QString("Arial");

  //Значения глобальных переменных настройки по умолчанию
  mDotSize           = 30;             //Размер точки соединений сегментов цепи
  mDotWidth          = 60;             //Толщина линии точки соединения сегментов цепи
  mSymPinSize        = 100;            //Размер перекрестья ножки символа
  mPartPinSize       = 500;            //Размер окружности ножки корпуса
  mSmartSize         = 5;              //Размер разумной точки (в пикселях)
  mSmartWidth        = 3;              //Толщина линии разумной точки
  mIsSmart           = true;           //Включение разумного режима
  mIsWireSmart       = true;           //Второй уровень разумного режима для цепей в схеме
  mSmartMask         = snapFull;       //Маска разрешенных для поиска разумных точек
  mNetUnionOn        = true;           //Выводить окно объединения каждый раз
  mNetUnionMode      = 1;              //Режим объединения цепей 0-нет, 1-XN, 2-все
  mEnableComp        = true;           //Разрешить операции с компонентами или заблокировать
  mEnableNet         = true;           //Разрешить операции с цепями
  mEnablePic         = true;           //Разрешить операции с картинкой
  mViaType           = QStringLiteral("c1.0d0.4");    //Via default type [Тип переходного отверстия по умолчанию]
  mShowRatNet        = true;           //Показывать резинки
  mShowRemark        = true;           //Показывать всплывающие подсказки над компонентами
  mShowMessageRemark = true;           //Показывать информацию в строке состояния
  mPlaceMode         = 0;              //Режим размещения компонентов
  mMinViewGrid       = 5;              //Минимальный видимый размер сетки в пикселях
  mUndoSize          = 100000;         //Размер буфера отмены
  mCursorView        = dcvSmall;       //Вид перекрестья курсора
  mSmallCursorSize   = 30;             //Размер малого курсора в пикселах
  mCursorShow        = true;           //Показывать курсор
  mTraseDotSize      = 10;             //Размер точки, показывающей цепь

  //PPM показывает сколько физической величины приходится на одну логическую единицу
  mSchPPM            = 0.01;           //Коэффициент преобразования в физическую величину в схемном редакторе
  mPrtPPM            = 0.001;          //Коэффициент преобразования в физическую величину в конструкциях
  mGridSyncXY        = true;           //Syncronisated edition x and grid steps
  mGridShow          = true;           //Включение сетки
  mCursorAlignGrid   = true;           //Включение движения курсора по сетке
  mCenterCursor      = true;           //Центровать курсор при увеличении и уменьшении
  mGridHistory.clear();
  mShowRuleErrors    = true;           //If true then over pcb shows rule error indicators as rectangles
  mShowFields        = true;           //If true then draw fields as fields names else draw fields as values
  mShowPads          = true;           //If true then draw pads in part editor
  mPolygonOpacity    = 0.5;            //Polygons draws with this opacity
  mAutoRemoveRoadLoop= true;           //Automatic detect and remove loops when enter roads


  mGuiderEnabled     = true;           //Флаг разрешения/запрещения путеводителя
  mGuiderPosition    = 0;              //Положение разделителя путеводителя
  mProjectEnabled    = true;           //Флаг разрешения/запрещения проекта
  mProjectPosition   = 0;              //Позиция разделителя проекта
  mProjectRemoveEnabled = false;       //Разрешение автоматического запрещения проекта
  mProjectRemoveTime = 15000;          //Время удержания проекта


  //Перечень слоев по умолчанию
  //Default layer list
  deleteLayers();
  for( int i = 0; sdLayerDescrDefault[i].mId != nullptr; i++ )
    addLayerId( sdLayerDescrDefault[i] );


  //Assign paired layers
  layerSetPairFor( LID0_COMPONENT );
  layerSetPairFor( LID0_PIN );
  layerSetPairFor( LID0_PIN_NAME );
  layerSetPairFor( LID0_PIN_NUMBER );
  layerSetPairFor( LID0_IDENT );
  layerSetPairFor( LID0_VALUE );

  layerSetPairFor( LID0_PAD );
  layerSetPairFor( LID0_EXCLUSION );
  layerSetPairFor( LID0_SOLDER_MASK );
  layerSetPairFor( LID0_STENCIL );
  layerSetPairFor( LID0_STENCIL_REPER );
  layerSetPairFor( LID0_SILK );
  layerSetPairFor( LID0_ADHESIV );
  layerSetPairFor( LID0_DIM );
  layerSetPairFor( LID0_GUIDE );
  layerSetPairFor( LID0_COURTYARD );


  //Default pcb rules
  mDefaultRules.mRules[ruleRoadWidth] = 300;
  mDefaultRules.mRules[rulePadPad] = 300;
  mDefaultRules.mRules[ruleRoadPad] = 300;
  mDefaultRules.mRules[ruleRoadRoad] = 300;

  }




//!
//! \brief layerGet Get existing layer, if it is not exist - then it's created as default
//! \param id       Id of needed layer
//! \return         Layer pointer
//!
SdLayer *SdEnvir::layerGet(QString id)
  {
  if( id.isEmpty() )
    return layerGet( LID0_INVISIBLE );

  if( !mLayerTable.contains(id) ) {
    //Not exist. Create new one.

    //At first we try to find layer in default table
    for( int i = 0; sdLayerDescrDefault[i].mId != nullptr; i++ )
      if( QString( sdLayerDescrDefault[i].mId ) == id ) {
        //We found layer in default table
        //Layer was removed and now will be inserted again
        addLayerId( sdLayerDescrDefault[i] );
        return mLayerTable.value(id);
        }

    //Build layer name
    SdLayerDescr descr;
    int layerIndex;
    auto [englishName,name] = SdLayer::layerIdToName( id, layerIndex, &descr );
    addLayerId( id, name, englishName, descr, layerIndex );
    }

  return mLayerTable.value(id);
  }




//!
//! \brief layerForEach - Iterates through all registered layers matching the specified class
//! \param classMask   - Layer class filter (dctConstruct, dctSchematic, dctCommon or combination)
//! \param fun1        - Callback function for each layer object. Return true to continue iteration, false to stop
//!
void SdEnvir::layerForEach(quint64 classMask, std::function<bool (SdLayer *)> fun1)
  {
  for( auto *p : std::as_const(mLayerTable) )
    if( p->classGet() & classMask ) {
      if( !fun1(p) ) return;
      }
  }

void SdEnvir::layerForEachConst(quint64 classMask, std::function<bool (SdLayer *)> fun1) const
  {
  for( auto *p : std::as_const(mLayerTable) )
    if( p->classGet() & classMask ) {
      if( !fun1(p) ) return;
      }
  }






void SdEnvir::layerVisibleSet(const QStringList &layerIdTable)
  {
  //Switch off all layers
  for( auto layer : mLayerTable )
    layer->stateSet( layerStateOff );

  //Switch on only layers from list
  for( const QString &layerId : std::as_const(layerIdTable) )
    if( mLayerTable.contains(layerId) )
      mLayerTable.value( layerId )->stateSet( layerStateOn );
  }






//!
//! \brief gridForEach Executes function fun1 to index of grid table
//!                    If intex < 0 then for all table
//! \param index       Index of row for which need to be executed fun1, if < 0 then for all rows
//! \param fun1        Function to execute
//!
void SdEnvir::gridForEach(int index, std::function<bool (QPointF)> fun1) const
  {
  if( index >= 0 && index < mGridHistory.count() )
    fun1( mGridHistory.at(index) );
  else if( index < 0 )
    for( auto p : std::as_const(mGridHistory) )
      if( !fun1(p) ) return;
  }




//!
//! \brief gridAppend Appends new record to grid table
//! \param p          Record to append
//!
void SdEnvir::gridAppend(QPointF p)
  {
  int i;
  for( i = 0; i < mGridHistory.count(); i++ )
    if( mGridHistory.at(i) == p ) {
      if( i ) {
        //Move grid dimension on top of list
        mGridHistory.removeAt( i );
        mGridHistory.insert( 0, p );
        }
      break;
      }
  if( i >= mGridHistory.count() ) {
    //This grid dimension is new for list
    //Check list size. If enough space then simle append else last remove
    if( mGridHistory.count() >= GRID_HISTORY_SIZE )
      mGridHistory.removeLast();
    mGridHistory.insert( 0, p );
    }
  }







void SdEnvir::resetForCache()
  {
  mCacheForPad.rebuild( mLayerTable, layerTracePad );
  mCacheForMask.rebuild( mLayerTable, layerTraceMask );
  mCacheForStencil.rebuild( mLayerTable, layerTraceStencil );
  mCacheForHole.rebuild( mLayerTable, layerTraceHole );
  mCacheForRoad.rebuild( mLayerTable, layerTraceRoad );
  mCacheForPolygon.rebuild( mLayerTable, layerTracePolygon );
  mCacheForBoundary.rebuild( mLayerTable, layerTraceBoundary );
  mCacheForKeepout.rebuild( mLayerTable, layerTraceKeepout );
  }







//Set layer pair
void SdEnvir::layerSetPair(QString idTop, QString idBot)
  {
  layerGet(idTop)->pairSet( layerGet(idBot) );
  }





//Reset "usage" layer flag for all layers
void SdEnvir::resetLayerUsage()
  {
  for( SdLayer *layer : mLayerTable )
    layer->resetUsage();
  }





//Set "usage" layer flag for stratum layers from caches
void SdEnvir::setLayerUsage(int stratumCount)
  {
  mCacheForPad.setLayerUsage( stratumCount );
  mCacheForMask.setLayerUsage( stratumCount );
  mCacheForStencil.setLayerUsage( stratumCount );
  mCacheForHole.setLayerUsage( stratumCount );
  mCacheForRoad.setLayerUsage( stratumCount );
  mCacheForPolygon.setLayerUsage( stratumCount );
  mCacheForBoundary.setLayerUsage( stratumCount );
  mCacheForKeepout.setLayerUsage( stratumCount );
  }










SdPad SdEnvir::getPad(const QString pinType)
  {
  static SdPadMap padMap;
  if( !padMap.contains(pinType) ) {
    SdPad pad;
    pad.parse( pinType );
    padMap.insert( pinType, pad );
    }
  return padMap.value( pinType );
  }



void SdEnvir::resetPads()
  {
  if( mPadStack != nullptr )
    delete mPadStack;
  mPadStack = nullptr;
  }




//Return current language settings
QString SdEnvir::languageGet()
  {
  QSettings s;
  return s.value( SDK_LANGUAGE, QVariant(QString("en")) ).toString();
  }



void SdEnvir::deleteLayers()
  {
  qDeleteAll(mLayerTable);
  mLayerTable.clear();
  }








void SdEnvir::addLayerId( const SdLayerDescr &descr )
  {
  QString layerId(descr.mId);
  int layerIndex;
  auto [englishName,name] = SdLayer::layerIdToName( layerId, layerIndex );
  addLayerId( layerId, name, englishName, descr, layerIndex );
  }




void SdEnvir::addLayerId(const QString &layerId, const QString &name, const QString &englishName, const SdLayerDescr &descr, int layerIndex )
  {
  //Check if layer already in table
  if( !mLayerTable.contains(layerId) ) {
    //Build and insert layer
    SdLayer *layer = new SdLayer( layerId, name, englishName, descr.mTrace, descr.mClass, descr.mStratum, descr.mColor, layerIndex );
    mLayerTable.insert( layer->id(), layer );
    }
  }




void SdEnvir::layerSetPairFor(const QString &lid0)
  {
  layerSetPair( lid0 + QString(LID1_TOP), lid0 + QString(LID1_BOT) );
  }
