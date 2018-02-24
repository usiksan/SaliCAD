/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdPasCadImport.h"
#include "SdPItemSymbol.h"
#include "SdGraphLinear.h"
#include "SdGraphLinearLine.h"
#include "SdGraphLinearRect.h"
#include "SdGraphLinearRectFilled.h"
#include "SdGraphLinearCircle.h"
#include "SdGraphText.h"
#include "SdSection.h"
#include "SdEnvir.h"

#include <QTextCodec>
#include <QMessageBox>
#include <QDebug>

//Типы объектов
#define pasCadObjIllegal       -1 //Ошибочный объект
#define pasCadObjNull           0 //Нет объекта
#define pasCadObjLinePic        1 //Линия
#define pasCadObjRectPic        2 //Прямоугольник
#define pasCadObjCirclePic      3 //Окружность
#define pasCadObjArcPic         4 //Дуга
#define pasCadObjTextPic        5 //Текст
#define pasCadObjSymPinPic      6 //Ножка символа
#define pasCadObjIdentPic       7 //Идентификатор
#define pasCadObjWirePic        8 //Сегмент цепи
#define pasCadObjWireNamePic    9 //Имя цепи
#define pasCadObjSheetNetPic   10 //Цепь в схеме
#define pasCadObjTrasePic      11 //Сегмент дорожки
#define pasCadObjPlateNetPic   12 //Цепь в плате
#define pasCadObjSymImpPic     13 //Вхождение символа в схему
#define pasCadObjSymbolPic     14 //Символ (схемное изображение компоненты)
#define pasCadObjAliasPic      15 //Компонент (связывает воедино символ, корпус и параметры)
#define pasCadObjPrtImpPic     16 //Вхождение корпуса в печатную плату
#define pasCadObjPrtPinPic     17 //Ножка корпуса
#define pasCadObjPrtPic        18 //Корпус (изображение корпуса компоненты)
#define pasCadObjSheetPic      19 //Лист схемы
#define pasCadObjPlatePic      20 //Печатная плата
#define pasCadObjListPic       21 //Перечень элементов
#define pasCadObjNetListPic    22 //Текстовый перечень цепей
#define pasCadObjProjectPic    23 //Проект
#define pasCadObjFillRectPic   24 //Залитый прямоугольник
#define pasCadObjPolygonPic    25 //Заполненная область
#define pasCadObjViaPic        26 //Переходное отверстие
#define pasCadObjAreaPic       27 //Область платы на схеме
#define pasCadObjLineSizePic   28 //Линейные размеры
#define pasCadObjArcSizePic    29 //Размер дуг (радиус скруглений)
#define pasCadObjCircleSizePic 30 //Размер окружностей (диаметр)
#define pasCadObjOLEPic        31 //Объект OLE
#define pasCadObjTextDocPic    32 //Текстовые документы
#define pasCadObjFieldPic      33 //Объект-текстовое поле
#define pasCadObjSelectorPic   34 //Объект-выделитель
#define pasCadObjProjectList   35 //Список открытых проектов
#define pasCadObjUserPic       36 //Пользовательский объект (поведение определяется DLL)
#define pasCadObjRegionPic     37 //Замкнутый регион (рисунок)
#define pasCadObjDataBasePic   38 //База данных


struct DLayer {
    QString   name;       //Имя слоя
    int       state : 4;  //Состояние
    int       trase : 4;  //Разрешение трассировки на данном слое
    int       trpos : 5;  //Позиция слоя при трассировке (верх, низ, внутри)
    int       reser : 19; //Резерв
    int       color;      //Цвет
    int       num;        //Порядковый номер в списке (визуальном)
    int       id;         //Порядковый номер в таблице слоев (для замены слоев)
    int       pair;       //Парный слой (для расположения на обратной стороне платы)
    int       flag;       //Флаг общего назначения
    SdLayer  *mLayer;

    DLayer() : name() {}
    ~DLayer() {}
  };


//Шрифт
struct DFont {
  QString name;     //Имя шрифта
  QString typeface; //Имя шрифта, зарегистрированного в системе
  int     num;      //Порядковый номер в списке (визуальном)
  int     id;       //Порядковый номер в таблице шрифтов (для замены шрифта)
  int     flag;     //Флаг общего назначения

  DFont() : name(), typeface() {}
  ~DFont() {}
  };


SdPasCadImport::SdPasCadImport(SdProject *prj, QWidget *owner) :
  mProject(prj),
  mFile(nullptr),
  mOwner(owner),
  mLayerTable(nullptr),
  mFontTable(nullptr)
  {

  }




SdPasCadImport::~SdPasCadImport()
  {
  if( mFile != nullptr )
    delete mFile;
  if( mLayerTable != nullptr )
    delete []mLayerTable;
  if( mFontTable != nullptr )
    delete []mFontTable;
  }




//Import entry point
bool SdPasCadImport::import(const QString fname)
  {
  if( QFile::exists( fname ) ) {
    mFile = new QFile(fname);
    if( mFile->open(QIODevice::ReadOnly) ) {
      //Read and check version
      int version = readInt32();
      if( version != 11 && version != 12 )
        return error( QObject::tr("File has version %1, but only v11 supported.").arg(version) );

      //Read file redaction
      readInt32();

      readNString();

      //Read layer table
      mLayerNumber = readInt32();
      if( mLayerNumber < 1 || mLayerNumber > 100 )
        return error( QObject::tr("Project structure fail. Wrong layer number %1").arg(mLayerNumber) );
      mLayerTable = new DLayer[mLayerNumber];
      readInt32(); //delta
      for( int i = 0; i < mLayerNumber; i++ ) {
        //mLayerTable[i].name =
        readWName();
        int field = readInt32();
        mLayerTable[i].state = field & 0xf;
        mLayerTable[i].trase = (field >> 4) & 0xf;
        mLayerTable[i].trpos = (field >> 8) & 0x1f;
        mLayerTable[i].color = readInt32();
        mLayerTable[i].num   = readInt32();
        mLayerTable[i].id    = readInt32();
        mLayerTable[i].pair  = readInt32();
        mLayerTable[i].flag  = readInt32();
        mLayerTable[i].mLayer = sdEnvir->getLayer( LID0_SCHEMATIC LID1_ELEM );
        qDebug() << "Layer " << i << mLayerTable[i].name;
        }

      //Read font table
      mFontNumber = readInt32();
      if( mFontNumber < 1 || mFontNumber > 100 )
        return error( QObject::tr("Project structure fail. Wrong font number %1").arg(mFontNumber) );
      mFontTable = new DFont[mFontNumber];
      readInt32(); //delta
      for( int i = 0; i < mFontNumber; i++ ) {
        mFontTable[i].name     = readWName();
        mFontTable[i].typeface = readWName();
        mFontTable[i].num      = readInt32();
        mFontTable[i].id       = readInt32();
        mFontTable[i].flag     = readInt32();
        qDebug() << "Font " << i << mFontTable[i].name << mFontTable[i].typeface;
        }
      return project();
      }
    return error( QObject::tr("Can't open file %1 to import").arg(fname) );
    }
  return error( QObject::tr("Can't find file %1 to import").arg(fname) );
  }





bool SdPasCadImport::project()
  {
  //DOwnerPic::Read(is);

  //remark.Read(is);       //Коментарии
  readNString();

  //symbol.Read(is,this);  //Символы
  if( !readObjectTable(mProject) ) return false;

  //prt.Read(is,this);     //Корпуса
  if( !readObjectTable(mProject) ) return false;

  //alias.Read(is,this);   //Компоненты
  if( !readObjectTable(mProject) ) return false;

  //sheets.Read(is,this);  //Схемы
  if( !readObjectTable(mProject) ) return false;

  //plates.Read(is,this);  //Платы
  if( !readObjectTable(mProject) ) return false;

  //texts.Read(is,this);   //Тексты
  if( !readObjectTable(mProject) ) return false;

  return true;
  }





bool SdPasCadImport::projectItem(SdProjectItem *item)
  {
  //DContainerPic::Read
  //DOwnerPic::Read( is );

  //param.Read( is );
  QString params = readNString();
  //Separate on lines
  QStringList lines = params.split( QChar('\r') );
  //Insert params by lines
  for( QString line : lines ) {
    QStringList param = line.split( QChar('=') );
    if( param.count() >= 2 )
      item->paramSet( param.at(0), param.at(1) );
    }

  //is.Read( &info, sizeof(DContainerInfo) );
  //DWName  name;        //Имя объекта
  item->setTitle( readWName(), QString() );
  //DInt32  lockCount;   //Счетчик блокировок
  readInt32();
  //DBool   handCreate;  //Истина если объект создан вручную и не подлежит удалению
  readBool();
  //DPoint origin;
  item->setOrigin( readPoint(), nullptr );
  //DBool  orgPresent;
  readBool();
  return true;
  }







//Build object by pasCad objectId
SdObject *SdPasCadImport::buildObject(int id)
  {
  switch( id ) {
    case pasCadObjLinePic        : //Линия
      return new SdGraphLinearLine();
    case pasCadObjRectPic        : //Прямоугольник
      return new SdGraphLinearRect();
//#define pasCadObjCirclePic      3 //Окружность
//#define pasCadObjArcPic         4 //Дуга
    case pasCadObjTextPic        : //Текст
      return new SdGraphText();
//#define pasCadObjSymPinPic      6 //Ножка символа
//#define pasCadObjIdentPic       7 //Идентификатор
//#define pasCadObjWirePic        8 //Сегмент цепи
//#define pasCadObjWireNamePic    9 //Имя цепи
//#define pasCadObjSheetNetPic   10 //Цепь в схеме
//#define pasCadObjTrasePic      11 //Сегмент дорожки
//#define pasCadObjPlateNetPic   12 //Цепь в плате
//#define pasCadObjSymImpPic     13 //Вхождение символа в схему
    case pasCadObjSymbolPic     : //Символ (схемное изображение компоненты)
      return new SdPItemSymbol();
//#define pasCadObjAliasPic      15 //Компонент (связывает воедино символ, корпус и параметры)
//#define pasCadObjPrtImpPic     16 //Вхождение корпуса в печатную плату
//#define pasCadObjPrtPinPic     17 //Ножка корпуса
//#define pasCadObjPrtPic        18 //Корпус (изображение корпуса компоненты)
//#define pasCadObjSheetPic      19 //Лист схемы
//#define pasCadObjPlatePic      20 //Печатная плата
//#define pasCadObjListPic       21 //Перечень элементов
//#define pasCadObjNetListPic    22 //Текстовый перечень цепей
//#define pasCadObjProjectPic    23 //Проект
    case pasCadObjFillRectPic    : //Залитый прямоугольник
      return new SdGraphLinearRectFilled();
//#define pasCadObjPolygonPic    25 //Заполненная область
//#define pasCadObjViaPic        26 //Переходное отверстие
//#define pasCadObjAreaPic       27 //Область платы на схеме
//#define pasCadObjLineSizePic   28 //Линейные размеры
//#define pasCadObjArcSizePic    29 //Размер дуг (радиус скруглений)
//#define pasCadObjCircleSizePic 30 //Размер окружностей (диаметр)
//#define pasCadObjOLEPic        31 //Объект OLE
//#define pasCadObjTextDocPic    32 //Текстовые документы
//#define pasCadObjFieldPic      33 //Объект-текстовое поле
//#define pasCadObjSelectorPic   34 //Объект-выделитель
//#define pasCadObjProjectList   35 //Список открытых проектов
    }
  return nullptr;
  }




SdLayer *SdPasCadImport::readLayer()
  {
  int layer = readInt16();
  if( layer >= 0 && layer < mLayerNumber )
    return mLayerTable[layer].mLayer;
  return nullptr;
  }




int SdPasCadImport::readInt8()
  {
  char data;
  mFile->read( &data, 1 );
  return data;
  }




int SdPasCadImport::readInt16()
  {
  char data[2];
  mFile->read( data, 2 );
  return (data[0] & 0xff) | ((data[1] << 8) & 0xff00);
  }




int SdPasCadImport::readInt32()
  {
  char data[4];
  mFile->read( data, 4 );
  return (data[0] & 0xff) | ((data[1] << 8) & 0xff00) | ((data[2] << 16) & 0xff0000) | (data[3] << 24);
  }




bool SdPasCadImport::readBool()
  {
  char data;
  mFile->read( &data, 1 );
  return data;
  }



//Read variable len string
QString SdPasCadImport::readNString()
  {
  static QTextCodec *win1251 = QTextCodec::codecForName("Windows-1251");
  int len = readInt32();
  QByteArray data;
  if( len ) data = mFile->read(len+1);
  else return QString();
  if( win1251 == nullptr )
    return QString::fromLocal8Bit( data );
  return win1251->toUnicode( data.constData() );
  }



QString SdPasCadImport::readConstString(int len)
  {
  static QTextCodec *win1251 = QTextCodec::codecForName("Windows-1251");
  QByteArray data = mFile->read(len);
  if( win1251 == nullptr )
    return QString::fromLocal8Bit( data );
  return win1251->toUnicode( data.constData() );
  }




bool SdPasCadImport::error(QString msg)
  {
  QMessageBox::warning( mOwner, QObject::tr("Error!"), msg );
  return false;
  }




bool SdPasCadImport::readObjectTable(SdContainer *container)
  {
  int number = readInt32();
  if( number < 0 ) return error( QObject::tr("Wrong objects number") );
  for( int i = 0; i < number; i++ )
    if( !readSingleObject(container) ) return false;
  return true;
  }



bool SdPasCadImport::readSingleObject(SdContainer *container)
  {
  //Читать идентификатор объекта
  char id;
  mFile->read( &id, 1 );
//  if( id <= dptNull || id >= dptLast )
//    throw CadError( "DStream::ReadObject: Неверный id объекта" );
  SdObject *obj;
  //Читать индекс объекта
  int i = readInt32();
  //Получить объект (если -1 то построить, иначе взять из таблицы объектов)
  if( i < 0 )
    obj = buildObject( id );
  else {
    if( !mObjectMap.contains(i) )
      mObjectMap.insert( i, buildObject(id) );
    obj = mObjectMap.value( i );
    }
  if( obj == nullptr )
    return false;
  container->insertChild( obj, nullptr );
  //В зависимости от типа объекта применить функцию чтения
  switch( id ) {
    case pasCadObjLinePic        : //Линия
      return readLine( obj );
    case pasCadObjFillRectPic    : //Залитый прямоугольник
    case pasCadObjRectPic        : //Прямоугольник
      return readRect( obj );
//#define pasCadObjCirclePic      3 //Окружность
//#define pasCadObjArcPic         4 //Дуга
//#define pasCadObjTextPic        5 //Текст
//#define pasCadObjSymPinPic      6 //Ножка символа
//#define pasCadObjIdentPic       7 //Идентификатор
//#define pasCadObjWirePic        8 //Сегмент цепи
//#define pasCadObjWireNamePic    9 //Имя цепи
//#define pasCadObjSheetNetPic   10 //Цепь в схеме
//#define pasCadObjTrasePic      11 //Сегмент дорожки
//#define pasCadObjPlateNetPic   12 //Цепь в плате
//#define pasCadObjSymImpPic     13 //Вхождение символа в схему
    case pasCadObjSymbolPic      : //Символ (схемное изображение компоненты)
      return readSymbol( obj );
//#define pasCadObjAliasPic      15 //Компонент (связывает воедино символ, корпус и параметры)
//#define pasCadObjPrtImpPic     16 //Вхождение корпуса в печатную плату
//#define pasCadObjPrtPinPic     17 //Ножка корпуса
//#define pasCadObjPrtPic        18 //Корпус (изображение корпуса компоненты)
//#define pasCadObjSheetPic      19 //Лист схемы
//#define pasCadObjPlatePic      20 //Печатная плата
//#define pasCadObjListPic       21 //Перечень элементов
//#define pasCadObjNetListPic    22 //Текстовый перечень цепей
//#define pasCadObjProjectPic    23 //Проект
//#define pasCadObjPolygonPic    25 //Заполненная область
//#define pasCadObjViaPic        26 //Переходное отверстие
//#define pasCadObjAreaPic       27 //Область платы на схеме
//#define pasCadObjLineSizePic   28 //Линейные размеры
//#define pasCadObjArcSizePic    29 //Размер дуг (радиус скруглений)
//#define pasCadObjCircleSizePic 30 //Размер окружностей (диаметр)
//#define pasCadObjOLEPic        31 //Объект OLE
//#define pasCadObjTextDocPic    32 //Текстовые документы
//#define pasCadObjFieldPic      33 //Объект-текстовое поле
//#define pasCadObjSelectorPic   34 //Объект-выделитель
//#define pasCadObjProjectList   35 //Список открытых проектов

    }
  return false;
  }




bool SdPasCadImport::readSymbol(SdObject *obj)
  {
  SdPItemSymbol *sym = dynamic_cast<SdPItemSymbol*>( obj );
  if( sym == nullptr )
    return error( QObject::tr("Internal error symbol") );
  if( !projectItem(sym) )
    return false;

  //is.Read( &info, sizeof(DSymbolInfo) );
  //DInt32           sect;    //Количество секций
  int sections = readInt32();

  for( int i = 0; i < sections; i++ ) {
    SdSection *section = new SdSection();
    section->updateFromSymbol( sym );
    sym->insertChild( section, nullptr );
    }

  //picture.Read( is, this );
  if( !readObjectTable( sym ) ) return false;

  //pins.Read( is, this );
  if( !readObjectTable( sym ) ) return false;

  //ident.Read( is );
  return false;
  }



SdPoint SdPasCadImport::readPoint()
  {
  SdPoint p;
  p.set( readInt32(), readInt32() );
  return p;
  }




bool SdPasCadImport::readLinear(SdObject *obj)
  {
  SdGraphLinear *linear = dynamic_cast<SdGraphLinear*>(obj);
  if( linear == nullptr ) return false;

  //read DLineProp
  //DWidth
  linear->mProp.mWidth = readInt32();
  //DLineType
  linear->mProp.mType  = readInt8();
  //DLayer
  linear->mProp.mLayer = readLayer();
  return linear->mProp.mLayer.layer() != nullptr;
  }




bool SdPasCadImport::readRect(SdObject *obj)
  {
  SdGraphLinearRect *rect = dynamic_cast<SdGraphLinearRect*>(obj);
  if( rect == nullptr ) return false;

  if( !readLinear(obj) ) return false;

  rect->a = readPoint();
  rect->b = readPoint();

  return true;
  }




bool SdPasCadImport::readLine(SdObject *obj)
  {
  SdGraphLinearLine *line = dynamic_cast<SdGraphLinearLine*>(obj);
  if( line == nullptr ) return false;

  if( !readLinear(obj) ) return false;

  line->a = readPoint();
  line->b = readPoint();

  return true;
  }

