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

SdEnvir *sdEnvir;

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
      SdLayer *layer = getLayer(id);
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
       >> mHomePath             //Каталог пользователя
       >> mLibraryPath          //Каталог библиотек
       >> mPatternPath          //Каталог шаблонов
       >> mCategoryPath         //Base path for store category hierarchy
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
     << mHomePath             //Каталог пользователя
     << mLibraryPath          //Каталог библиотек
     << mPatternPath          //Каталог шаблонов
     << mCategoryPath         //Base path for store category hierarchy
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

  mHomePath = QDir::homePath();        //Каталог пользователя
  if( !mHomePath.endsWith( QChar('/') ) )
    mHomePath.append( QChar('/') );
  mHomePath.append( QStringLiteral("SaliLAB/SaliCAD/") );

  //mLibraryPath in SdEnvir is obsolete
  mLibraryPath = mHomePath;            //Каталог библиотек
  mLibraryPath.append( QStringLiteral("library/") );

  SvDir def( QCoreApplication::applicationDirPath() );
  mPatternPath = def.slashedPath() + QStringLiteral("pattern/");   //Каталог шаблонов

  //Category file path
  mCategoryPath = def.slashedPath() + QString("category/");        //Base path for store category hierarchy


  //Перечень слоев по умолчанию
  //Default layer list
  deleteLayers();
  for( int i = 0; sdLayerDescrActual[i].mId != nullptr; i++ )
    addLayerId( QString(sdLayerDescrActual[i].mId), sdLayerDescrActual[i].mColor, sdLayerDescrActual[i].mTrace, sdLayerDescrActual[i].mStratum );
  for( int i = 0; sdLayerDescrAddon[i].mId != nullptr; i++ )
    addLayerId( QString(sdLayerDescrAddon[i].mId), sdLayerDescrAddon[i].mColor, sdLayerDescrAddon[i].mTrace, sdLayerDescrAddon[i].mStratum );


  //Assign paired layers
  setPair( QString( LID0_COMPONENT ),  QString( LID0_COMPONENT LID1_BOT ) );
  setPair( QString( LID0_PIN ),        QString( LID0_PIN LID1_BOT ) );
  setPair( QString( LID0_PIN_NAME ),   QString( LID0_PIN_NAME LID1_BOT ) );
  setPair( QString( LID0_PIN_NUMBER ), QString( LID0_PIN_NUMBER LID1_BOT ) );
  setPair( QString( LID0_IDENT ),      QString( LID0_IDENT LID1_BOT ) );

  setPair( QString( LID0_PAD LID1_TOP ),  QString( LID0_PAD LID1_BOT ) );
  setPair( QString( LID0_CLEAR LID1_TOP ), QString( LID0_CLEAR LID1_BOT ) );
  setPair( QString( LID0_SOLDER_MASK LID1_TOP ),  QString( LID0_SOLDER_MASK LID1_BOT ) );


  //Default pcb rules
  mDefaultRules.mRules[ruleRoadWidth] = 300;
  mDefaultRules.mRules[rulePadPad] = 300;
  mDefaultRules.mRules[ruleRoadPad] = 300;
  mDefaultRules.mRules[ruleRoadRoad] = 300;

  }




//!
//! \brief getLayer Get existing layer, if it is not exist - then it's created as default
//! \param id       Id of needed layer
//! \return         Layer pointer
//!
SdLayer *SdEnvir::getLayer(QString id)
  {
  if( id.isEmpty() )
    return getLayer( LID0_COMMON );

  if( !mLayerTable.contains(id) ) {
    //Not exist. Create new one.

    //Build layer name
    QString name;
    QStringList list = id.split( QChar('.') );
    QString lid0 = list.at(0);
    name = layerId2NameLevel0( lid0 );

    if( list.count() > 1 ) {
      QString lid1clear = list.at(1);
      QString lid1 = QString(".") + lid1clear;
      name.append( " " ).append( layerId2NameLevel1(lid1) );
      }
    addLayer( new SdLayer(id, name, 0x3f803f) );
    }

  return mLayerTable.value(id);
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
void SdEnvir::setPair(QString idTop, QString idBot)
  {
  getLayer(idTop)->setPair( getLayer(idBot) );
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





//Layer id to name translation service
QString SdEnvir::layerId2NameLevel0(QString lid0 )
  {
  for( int i = 0; sdLayerLevel0[i].mLid != nullptr; i++ )
    if( lid0 == QString(sdLayerLevel0[i].mLid) )
      return QObject::tr( sdLayerLevel0[i].mTranslate );
  return lid0;
  }


//Layer id to name translation service
QString SdEnvir::layerId2NameLevel1(QString lid1)
  {
  for( int i = 1; sdLayerLevel1[i].mLid != nullptr; i++ )
    if( lid1 == QString(sdLayerLevel1[i].mLid) )
      return QObject::tr( sdLayerLevel1[i].mTranslate );
  return lid1;
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




void SdEnvir::addLayer(SdLayer *layer)
  {
  mLayerTable.insert( layer->id(), layer );
  }




void SdEnvir::addLayerId(const QString layerId, unsigned ccolor, SdLayerTrace st, int stratum )
  {
  SdLayer *layer = getLayer( layerId );
  layer->setColor( ccolor );
  layer->setTrace( st );
  layer->setStratum( stratum );
  }
