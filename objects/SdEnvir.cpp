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
#include <QDir>
#include <QDebug>

SdEnvir *sdEnvir;

SdEnvir::SdEnvir()
  {
  }

SdEnvir::~SdEnvir()
  {
  deleteLayers();
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




QString SdEnvir::getSysFont(int fontId)
  {
  if( fontId >= 0 && fontId < FONT_COUNT )
    return mFonts[fontId];
  return mFonts[0];
  }




void SdEnvir::setSysFont(int fontId, const QString fontName)
  {
  if( fontId >= 0 && fontId < FONT_COUNT )
    mFonts[fontId] = fontName;
  }




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
       >> mPrtPinSize           //Размер окружности ножки корпуса
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
       >> mViaSize              //Размер переходного отверстия по умолчанию
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
       >> mPadStackFile         //Файл контактных площадок
       >> mPadStackObject       //Объект содержащий контактные площадки
       >> mGridHistory;         //Previous grid history table
    }
  else defaultEnvir();
  }




void SdEnvir::saveEnvir()
  {
  QByteArray ar;
  QDataStream os( &ar, QIODevice::WriteOnly );

  //Записали цвета
  for( int i = 0; i < scLast; i++ )
    os << mSysColors[i];

  //Write fonts
  for( int i = 0; i < FONT_COUNT; i++ )
    os << mFonts[i];

  //Записали слои
  int c = mLayerTable.count();
  os << c;
  for( SdLayer *layer : mLayerTable ) {
    os << layer->id();
    layer->write( os );
    }


  //Записали остальные значения
  os << mDotSize              //Размер точки соединений сегментов цепи
     << mDotWidth             //Толщина линии точки соединения сегментов цепи
     << mSymPinSize           //Размер перекрестья ножки символа
     << mPrtPinSize           //Размер окружности ножки корпуса
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
     << mViaSize              //Размер переходного отверстия по умолчанию
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
     << mCursorAlignGrid           //Включение движения курсора по сетке
     << mCenterCursor         //Центровать курсор при увеличении и уменьшении
     << mHomePath             //Каталог пользователя
     << mLibraryPath          //Каталог библиотек
     << mPatternPath          //Каталог шаблонов
     << mPadStackFile         //Файл контактных площадок
     << mPadStackObject       //Объект содержащий контактные площадки
     << mGridHistory;         //Previous grid history table

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
  mSysColors[scNotLinked] = QColor(200,50,50);    //Color for not linked objects

  //Default fonts
  mFonts[0] = QString("FreeSerif");
  mFonts[1] = QString("FreeSans");
  mFonts[2] = QString("FreeMono");
  mFonts[3] = QString("Source Code Pro");
  mFonts[4] = QString("Arial");

  //Значения глобальных переменных настройки по умолчанию
  mDotSize           = 5;             //Размер точки соединений сегментов цепи
  mDotWidth          = 60;             //Толщина линии точки соединения сегментов цепи
  mSymPinSize        = 5;              //Размер перекрестья ножки символа
  mPrtPinSize        = 500;            //Размер окружности ножки корпуса
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
  mViaSize           = 100;            //Размер переходного отверстия по умолчанию
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
  mSchPPM            = 0.1;            //Коэффициент преобразования в физическую величину в схемном редакторе
  mPrtPPM            = 0.001;          //Коэффициент преобразования в физическую величину в конструкциях
  mGridSyncXY        = true;           //Syncronisated edition x and grid steps
  mGridShow          = true;           //Включение сетки
  mCursorAlignGrid        = true;           //Включение движения курсора по сетке
  mCenterCursor      = true;           //Центровать курсор при увеличении и уменьшении
  mGridHistory.clear();

  mGuiderEnabled     = true;           //Флаг разрешения/запрещения путеводителя
  mGuiderPosition    = 0;              //Положение разделителя путеводителя
  mProjectEnabled    = true;           //Флаг разрешения/запрещения проекта
  mProjectPosition   = 0;              //Позиция разделителя проекта
  mProjectRemoveEnabled = false;       //Разрешение автоматического запрещения проекта
  mProjectRemoveTime = 15000;          //Время удержания проекта

  mHomePath = QDir::homePath();        //Каталог пользователя
  if( !mHomePath.endsWith( QChar('/') ) )
    mHomePath.append( QChar('/') );
  mHomePath.append( QString("SaliLAB/SaliCAD/") );
  mLibraryPath = mHomePath;            //Каталог библиотек
  mLibraryPath.append( QString("library/") );
  mPatternPath = mHomePath;            //Каталог шаблонов
  mPatternPath.append( QString("pattern/") );
  mPadStackFile = mHomePath;           //Файл контактных площадок
  mPadStackFile.append( QString("pads/defaultStack.pads") );
  mPadStackObject = QString("default");//Объект содержащий контактные площадки

  //  QString         mLastFile;             //Последний файл пользователя
//  QString         mHome;                 //Каталог пользователя
//  QString         mLibrary;              //Каталог библиотек
//  QString         mPattern;              //Каталог шаблонов
//  double          mWidthStep;            //Шаг изменения толщины
//  double          mTextSizeStep;         //Шаг изменения высоты текста
//  bool            mMaxFrame;             //Истина, если главное окно на весь экран
//  QString         mPadStackFile;          //Файл контактных площадок
//  QString         mPadStackObject;        //Объект содержащий контактные площадки
//  bool            mAutoPads;             //Автоматическая расстановка КП
//  bool            mCreateBack;           //Создавать BAK файл при сохранении
//  QPointF         mGridHistory[GRID_HISTORY_SIZE];  //Предыдущие значения сетки
//  double          mPolyClear;             //Зазор между дорожками и полигоном
//  bool            mShowConflict;         //Показывать конфликты трассировки

  //Перечень слоев по умолчанию
  //Default layer list
  deleteLayers();
  for( int i = 0; sdLayerDescrActual[i].mId != nullptr; i++ )
    addLayerId( QString(sdLayerDescrActual[i].mId), sdLayerDescrActual[i].mColor );
  for( int i = 0; sdLayerDescrAddon[i].mId != nullptr; i++ )
    addLayerId( QString(sdLayerDescrAddon[i].mId), sdLayerDescrAddon[i].mColor );


  //Assign paired layers
  setPair( QString( LID0_COMPONENT ),  QString( LID0_COMPONENT LID1_BOT ) );
  setPair( QString( LID0_PIN ),        QString( LID0_PIN LID1_BOT ) );
  setPair( QString( LID0_PIN_NAME ),   QString( LID0_PIN_NAME LID1_BOT ) );
  setPair( QString( LID0_PIN_NUMBER ), QString( LID0_PIN_NUMBER LID1_BOT ) );
  setPair( QString( LID0_IDENT ),      QString( LID0_IDENT LID1_BOT ) );

  setPair( QString( LID0_PAD LID1_TOP ),  QString( LID0_PAD LID1_BOT ) );
  setPair( QString( LID0_CLEAR LID1_TOP ), QString( LID0_CLEAR LID1_BOT ) );
  setPair( QString( LID0_SOLDER_MASK LID1_TOP ),  QString( LID0_SOLDER_MASK LID1_BOT ) );
  }




//Get existing layer, if it is not exist - then it's created as default
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
    if( lid0 == QString(LID0_COMMON) )          name = QObject::tr("Common");
    else if( lid0 == QString(LID0_INVISIBLE) )  name = QObject::tr("Invisible");
    //Schematic specific
    else if( lid0 == QString(LID0_NET) )        name = QObject::tr("Schematic net");
    else if( lid0 == QString(LID0_NET_NAME) )   name = QObject::tr("Schematic net name");
    else if( lid0 == QString(LID0_BUS) )        name = QObject::tr("Schematic net bus");
    else if( lid0 == QString(LID0_AREA) )       name = QObject::tr("Schematic pcb area contour");
    //PCB specific
    else if( lid0 == QString(LID0_PCB) )        name = QObject::tr("PCB contour");
    else if( lid0 == QString(LID0_WIRE) )       name = QObject::tr("PCB wire");
    else if( lid0 == QString(LID0_POLYGON) )    name = QObject::tr("PCB polygon");
    else if( lid0 == QString(LID0_PAD) )        name = QObject::tr("PCB pad");
    else if( lid0 == QString(LID0_HOLE) )       name = QObject::tr("PCB hole");
    else if( lid0 == QString(LID0_CLEAR) )      name = QObject::tr("PCB trace clear");
    else if( lid0 == QString(LID0_SOLDER_MASK) )       name = QObject::tr("PCB solder mask");
    else if( lid0 == QString(LID0_STENSIL) )    name = QObject::tr("PCB stensil aperture");
    else if( lid0 == QString(LID0_STENSIL_REPER) ) name = QObject::tr("PCB stensil reper");
    else if( lid0 == QString(LID0_EXCLUSION) )  name = QObject::tr("PCB trace exclusion area");
    else if( lid0 == QString(LID0_TRACE) )      name = QObject::tr("PCB trace area");
    //Both schematic and PCB
    else if( lid0 == QString(LID0_COMPONENT) )  name = QObject::tr("Component");
    else if( lid0 == QString(LID0_PIN) )        name = QObject::tr("Pin");
    else if( lid0 == QString(LID0_PIN_NAME) )   name = QObject::tr("Pin name");
    else if( lid0 == QString(LID0_PIN_NUMBER) ) name = QObject::tr("Pin number");
    else if( lid0 == QString(LID0_IDENT) )      name = QObject::tr("Component ident");
    else if( lid0 == QString(LID0_PICTURE) )    name = QObject::tr("Picture");
    else if( lid0 == QString(LID0_REMARK) )     name = QObject::tr("Remark");
    else if( lid0 == QString(LID0_VALUE) )      name = QObject::tr("Value");
    else if( lid0 == QString(LID0_CONTOUR) )    name = QObject::tr("Contour");
    else name = lid0;

    if( list.count() > 1 ) {
      QString lid1clear = list.at(1);
      QString lid1 = QString(".") + lid1clear;
      if( lid1 == QString(LID1_TOP) )        name.append( QObject::tr(" on top") );
      else if( lid1 == QString(LID1_BOT) )   name.append( QObject::tr(" on bottom") );
      else if( lid1 == QString(LID1_INT00) ) name.append( QObject::tr(" on internal 1") );
      else if( lid1 == QString(LID1_INT01) ) name.append( QObject::tr(" on internal 2") );
      else if( lid1 == QString(LID1_INT02) ) name.append( QObject::tr(" on internal 3") );
      else if( lid1 == QString(LID1_INT03) ) name.append( QObject::tr(" on internal 4") );
      else name.append( " " ).append( lid1 );
      }
    addLayer( new SdLayer(id, name, 0x3f803f) );
    }

  return mLayerTable.value(id);
  }



//Set layer pair
void SdEnvir::setPair(QString idTop, QString idBot)
  {
  getLayer(idTop)->setPair( getLayer(idBot) );
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




void SdEnvir::addLayerId(const QString layerId, unsigned ccolor)
  {
  getLayer( layerId )->setColor( ccolor );
  }
