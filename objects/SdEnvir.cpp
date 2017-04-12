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

    //Записали остальные значения
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
       >> mShowCursor           //Показывать курсор
       >> mTraseDotSize         //Размер точки, показывающей цепь
       >> mSchPPM               //Коэффициент преобразования в физическую величину в схемном редакторе
       >> mPrtPPM               //Коэффициент преобразования в физическую величину в конструкциях
       >> mGridView             //Включение сетки
       >> mCursorGrid           //Включение движения курсора по сетке
       >> mCenterCursor         //Центровать курсор при увеличении и уменьшении
       >> mLineEnterType;        //Vertex type at enter line
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
     << mShowCursor           //Показывать курсор
     << mTraseDotSize         //Размер точки, показывающей цепь
     << mSchPPM               //Коэффициент преобразования в физическую величину в схемном редакторе
     << mPrtPPM               //Коэффициент преобразования в физическую величину в конструкциях
     << mGridView             //Включение сетки
     << mCursorGrid           //Включение движения курсора по сетке
     << mCenterCursor         //Центровать курсор при увеличении и уменьшении
     << mLineEnterType;        //Vertex type at enter line

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
  mSmartMask         = MAX64_MASK;     //Маска разрешенных для поиска разумных точек
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

  //PPM показывает сколько физической величины приходится на одну логическую единицу
  mSchPPM            = 0.1;            //Коэффициент преобразования в физическую величину в схемном редакторе
  mPrtPPM            = 0.001;          //Коэффициент преобразования в физическую величину в конструкциях
  mGridView          = true;           //Включение сетки
  mCursorGrid        = true;           //Включение движения курсора по сетке
  mCenterCursor      = true;           //Центровать курсор при увеличении и уменьшении
  mLineEnterType     = dleOrtho;       //By default orthogonal vertex line

  mGuiderEnabled     = true;           //Флаг разрешения/запрещения путеводителя
  mGuiderPosition    = 0;              //Положение разделителя путеводителя
  mProjectEnabled    = true;           //Флаг разрешения/запрещения проекта
  mProjectPosition   = 0;              //Позиция разделителя проекта
  mProjectRemoveEnabled = false;       //Разрешение автоматического запрещения проекта
  mProjectRemoveTime = 15000;          //Время удержания проекта
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
  deleteLayers();
  addLayer( new SdLayer( QString( LID0_INVISIBLE ), QObject::tr("Invisible"), 0xff007f7f ) );

  addLayer( new SdLayer( QString( LID0_SCHEMATIC LID1_ELEM ), QObject::tr("Schematic element"), 0x000000ff ) );
  addLayer( new SdLayer( QString( LID0_SCHEMATIC LID1_PIN ), QObject::tr("Schematic element pin"), 0x0000ff00 ) );
  addLayer( new SdLayer( QString( LID0_SCHEMATIC LID1_PIN_NAME ), QObject::tr("Schematic element pin name"), 0x007f7f00 ) );
  addLayer( new SdLayer( QString( LID0_SCHEMATIC LID1_PIN_NUMBER ), QObject::tr("Schematic element pin number"), 0x00ff0000 ) );
  addLayer( new SdLayer( QString( LID0_SCHEMATIC LID1_IDENT ), QObject::tr("Schematic element ident"), 0x00ff7f00 ) );
  addLayer( new SdLayer( QString( LID0_SCHEMATIC LID1_NET ), QObject::tr("Schematic net"), 0x00ffff7f ) );
  addLayer( new SdLayer( QString( LID0_SCHEMATIC LID1_NET_NAME ), QObject::tr("Schematic net name"), 0x00ff00ff ) );
  addLayer( new SdLayer( QString( LID0_SCHEMATIC LID1_BUS ), QObject::tr("Schematic net bus"), 0xff0000ff ) );
  addLayer( new SdLayer( QString( LID0_SCHEMATIC LID1_PICTURE ), QObject::tr("Schematic picture"), 0xff00ff00 ) );
  addLayer( new SdLayer( QString( LID0_SCHEMATIC LID1_REMARK ), QObject::tr("Schematic remark"), 0xffff0000 ) );

  addLayer( new SdLayer( QString( LID0_PCB_TOP LID1_ELEM ), QObject::tr("Pcb top part"), 0x000000ff ) );
  addLayer( new SdLayer( QString( LID0_PCB_TOP LID1_PIN ), QObject::tr("Pcb top pin"), 0x0000ff00 ) );
  addLayer( new SdLayer( QString( LID0_PCB_TOP LID1_PIN_NAME ), QObject::tr("Pcb top pin name"), 0x007f7f00 ) );
  addLayer( new SdLayer( QString( LID0_PCB_TOP LID1_PIN_NUMBER ), QObject::tr("Pcb top pin number"), 0x00ff0000 ) );
  addLayer( new SdLayer( QString( LID0_PCB_TOP LID1_IDENT ), QObject::tr("Pcb top ident"), 0x00ff7f00 ) );
  addLayer( new SdLayer( QString( LID0_PCB_TOP LID1_PICTURE ), QObject::tr("Pcb top picture"), 0xff00ff00 ) );
  addLayer( new SdLayer( QString( LID0_PCB_TOP LID1_REMARK ), QObject::tr("Pcb top remark"), 0xffff0000 ) );

  addLayer( new SdLayer( QString( LID0_PCB_BOT LID1_ELEM ), QObject::tr("Pcb bot part"), 0x000000ff ) );
  addLayer( new SdLayer( QString( LID0_PCB_BOT LID1_PIN ), QObject::tr("Pcb bot pin"), 0x0000ff00 ) );
  addLayer( new SdLayer( QString( LID0_PCB_BOT LID1_PIN_NAME ), QObject::tr("Pcb bot pin name"), 0x007f7f00 ) );
  addLayer( new SdLayer( QString( LID0_PCB_BOT LID1_PIN_NUMBER ), QObject::tr("Pcb bot pin number"), 0x00ff0000 ) );
  addLayer( new SdLayer( QString( LID0_PCB_BOT LID1_IDENT ), QObject::tr("Pcb bot ident"), 0x00ff7f00 ) );

  addLayer( new SdLayer( QString( LID0_TRACE LID1_TOP ), QObject::tr("Trace top"), 0xffffff00 ) );
  addLayer( new SdLayer( QString( LID0_TRACE LID1_BOT ), QObject::tr("Trace bot"), 0xff00007f ) );
  addLayer( new SdLayer( QString( LID0_TRACE LID1_INT00 ), QObject::tr("Trace int1"), 0xffffff40 ) );
  addLayer( new SdLayer( QString( LID0_TRACE LID1_INT01 ), QObject::tr("Trace int2"), 0xffffff80 ) );
  addLayer( new SdLayer( QString( LID0_TRACE LID1_INT02 ), QObject::tr("Trace int3"), 0xff00407f ) );
  addLayer( new SdLayer( QString( LID0_TRACE LID1_INT03 ), QObject::tr("Trace int4"), 0xff00807f ) );

  addLayer( new SdLayer( QString( LID0_POLYGON LID1_TOP ), QObject::tr("Polygon top"), 0xffffff00 ) );
  addLayer( new SdLayer( QString( LID0_POLYGON LID1_BOT ), QObject::tr("Polygon bot"), 0xff00007f ) );

  addLayer( new SdLayer( QString( LID0_PADS LID1_TOP ), QObject::tr("Pads top"), 0xff3fff00 ) );
  addLayer( new SdLayer( QString( LID0_PADS LID1_BOT ), QObject::tr("Pads bot"), 0xff40007f ) );

  addLayer( new SdLayer( QString( LID0_HOLE ), QObject::tr("Holes"), 0xff3f3f40 ) );

  addLayer( new SdLayer( QString( LID0_CLEAR LID1_TOP ), QObject::tr("Clears top"), 0xff3f3f00 ) );
  addLayer( new SdLayer( QString( LID0_CLEAR LID1_BOT ), QObject::tr("Clears bot"), 0xff40407f ) );

  addLayer( new SdLayer( QString( LID0_MASK LID1_TOP ), QObject::tr("Mask top"), 0xff3fff00 ) );
  addLayer( new SdLayer( QString( LID0_MASK LID1_BOT ), QObject::tr("Mask bot"), 0xff40007f ) );

  addLayer( new SdLayer( QString( LID0_PCB_TOP LID1_BORDER ), QObject::tr("Pcb countur"), 0xff00ffff ) );

  //Назначить парные слои
  setPair( QString( LID0_PCB_TOP LID1_ELEM ),       QString( LID0_PCB_BOT LID1_ELEM ) );
  setPair( QString( LID0_PCB_TOP LID1_PIN ),        QString( LID0_PCB_BOT LID1_PIN ) );
  setPair( QString( LID0_PCB_TOP LID1_PIN_NAME ),   QString( LID0_PCB_BOT LID1_PIN_NAME ) );
  setPair( QString( LID0_PCB_TOP LID1_PIN_NUMBER ), QString( LID0_PCB_BOT LID1_PIN_NUMBER ) );
  setPair( QString( LID0_PCB_TOP LID1_IDENT ),      QString( LID0_PCB_BOT LID1_IDENT ) );

  setPair( QString( LID0_PADS LID1_TOP ),  QString( LID0_PADS LID1_BOT ) );
  setPair( QString( LID0_CLEAR LID1_TOP ), QString( LID0_CLEAR LID1_BOT ) );
  setPair( QString( LID0_MASK LID1_TOP ),  QString( LID0_MASK LID1_BOT ) );
  }




//Get existing layer, if it is not exist - then it's created as default
SdLayer *SdEnvir::getLayer(QString id)
  {
  if( !mLayerTable.contains(id) ) {
    //Not exist. Create new one.

    //Build layer name
    QString name;
    QString lid0 = id.mid(0,3);
    if( lid0 == QString(LID0_SCHEMATIC) ) name = QObject::tr("In schematic ");
    else if( lid0 == QString(LID0_PCB_TOP) ) name = QObject::tr("In pcb top ");
    else if( lid0 == QString(LID0_PCB_BOT) ) name = QObject::tr("In pcb bottom ");
    else if( lid0 == QString(LID0_INVISIBLE) ) name = QObject::tr("Invisible ");
    else if( lid0 == QString(LID0_TRACE) ) name = QObject::tr("Trace ");
    else if( lid0 == QString(LID0_POLYGON) ) name = QObject::tr("Polygon ");
    else if( lid0 == QString(LID0_PADS) ) name = QObject::tr("Pad ");
    else if( lid0 == QString(LID0_HOLE) ) name = QObject::tr("Hole ");
    else if( lid0 == QString(LID0_CLEAR) ) name = QObject::tr("Clear ");
    else if( lid0 == QString(LID0_MASK) ) name = QObject::tr("Mask ");
    else if( lid0 == QString(LID0_STENSIL) ) name = QObject::tr("Stensil ");
    else if( lid0 == QString(LID0_STENSIL_REPER) ) name = QObject::tr("Stensil reper ");

    QString lid1 = id.mid(3,3);
    if( lid1.length() == 3 ) {
      if( lid1 == QString(LID1_TOP) ) name.append( QObject::tr("top") );
      else if( lid1 == QString(LID1_BOT) ) name.append( QObject::tr("bottom") );
      else if( lid1 == QString(LID1_INT00) ) name.append( QObject::tr("internal 1") );
      else if( lid1 == QString(LID1_INT01) ) name.append( QObject::tr("internal 2") );
      else if( lid1 == QString(LID1_INT02) ) name.append( QObject::tr("internal 3") );
      else if( lid1 == QString(LID1_INT03) ) name.append( QObject::tr("internal 4") );
      else if( lid1 == QString(LID1_BORDER) ) name.append( QObject::tr("border") );
      else if( lid1 == QString(LID1_ELEM) ) name.append( QObject::tr("element") );
      else if( lid1 == QString(LID1_PIN) ) name.append( QObject::tr("pin") );
      else if( lid1 == QString(LID1_PIN_NAME) ) name.append( QObject::tr("pin name") );
      else if( lid1 == QString(LID1_PIN_NUMBER) ) name.append( QObject::tr("pin number") );
      else if( lid1 == QString(LID1_IDENT) ) name.append( QObject::tr("ident") );
      else if( lid1 == QString(LID1_NET) ) name.append( QObject::tr("net") );
      else if( lid1 == QString(LID1_NET_NAME) ) name.append( QObject::tr("net name") );
      else if( lid1 == QString(LID1_BUS) ) name.append( QObject::tr("bus") );
      else if( lid1 == QString(LID1_PICTURE) ) name.append( QObject::tr("picture") );
      else if( lid1 == QString(LID1_REMARK) ) name.append( QObject::tr("remark") );
      else if( lid1 == QString(LID1_VALUES) ) name.append( QObject::tr("values") );
      else if( lid1 == QString(LID1_COUNTUR) ) name.append( QObject::tr("countur") );
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
