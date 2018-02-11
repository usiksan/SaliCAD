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

#include <QTextCodec>
#include <QMessageBox>
#include <QDebug>

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
  };


//Шрифт
struct DFont {
  QString name;     //Имя шрифта
  QString typeface; //Имя шрифта, зарегистрированного в системе
  int     num;      //Порядковый номер в списке (визуальном)
  int     id;       //Порядковый номер в таблице шрифтов (для замены шрифта)
  int     flag;     //Флаг общего назначения
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
    delete mLayerTable;
  if( mFontTable != nullptr )
    delete mFontTable;
  }





bool SdPasCadImport::project()
  {
  //DOwnerPic::Read(is);

  //remark.Read(is);       //Коментарии
  readNString();

  //symbol.Read(is,this);  //Символы
  int number = readInt32();
  if( number < 0 ) return error( QObject::tr("Wrong symbol number") );
  for( int i = 0; i < number; i++ )
    if( !symbol() ) return false;

  //prt.Read(is,this);     //Корпуса
  number = readInt32();
  if( number < 0 ) return error( QObject::tr("Wrong part number") );
  for( int i = 0; i < number; i++ )
    if( !part() ) return false;

  //alias.Read(is,this);   //Компоненты
  number = readInt32();
  if( number < 0 ) return error( QObject::tr("Wrong alias number") );
  for( int i = 0; i < number; i++ )
    if( !alias() ) return false;

  //sheets.Read(is,this);  //Схемы
  number = readInt32();
  if( number < 0 ) return error( QObject::tr("Wrong sheet number") );
  for( int i = 0; i < number; i++ )
    if( !sheet() ) return false;

  //plates.Read(is,this);  //Платы
  number = readInt32();
  if( number < 0 ) return error( QObject::tr("Wrong plate number") );
  for( int i = 0; i < number; i++ )
    if( !plate() ) return false;

  //texts.Read(is,this);   //Тексты
  number = readInt32();
  if( number < 0 ) return error( QObject::tr("Wrong text number") );
  for( int i = 0; i < number; i++ )
    if( !text() ) return false;

  return true;
  }




bool SdPasCadImport::symbol()
  {
  SdPItemSymbol *sym = dynamic_cast<SdPItemSymbol*>( readObject(SD_TYPE_SYMBOL) );
  if( sym == nullptr )
    return error( QObject::tr("Internal error symbol") );
  if( !projectItem(sym) )
    return false;

  //is.Read( &info, sizeof(DSymbolInfo) );
  //DInt32           sect;    //Количество секций
  int sections = readInt32();

  //picture.Read( is, this );
  //pins.Read( is, this );
  //ident.Read( is );

  }

bool SdPasCadImport::part()
  {

  }

bool SdPasCadImport::alias()
  {

  }

bool SdPasCadImport::sheet()
  {

  }

bool SdPasCadImport::plate()
  {

  }

bool SdPasCadImport::text()
  {

  }

bool SdPasCadImport::projectItem(SdProjectItem *item)
  {
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
  }



SdObject *SdPasCadImport::readObject( const QString type)
  {
  //Читать идентификатор объекта
  char id;
  mFile->read( &id, 1 );
//  if( id <= dptNull || id >= dptLast )
//    throw CadError( "DStream::ReadObject: Неверный id объекта" );
  //Читать индекс объекта
  int i = readInt32();
  //Получить объект (если -1 то построить, иначе взять из таблицы объектов)
  if( i < 0 )
    return SdObject::build( type );
  if( !mObjectMap.contains(i) )
    mObjectMap.insert( i, SdObject::build(type) );
  return mObjectMap.value( i );
  }



//Import entry point
bool SdPasCadImport::import(const QString fname)
  {
  if( QFile::exists( fname ) ) {
    mFile = new QFile(fname);
    if( mFile->open(QIODevice::ReadOnly) ) {
      //Read and check version
      int version = readInt32();
      if( version != 11 ) return error( QObject::tr("File has version %1, but only v11 supported.").arg(version) );

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
        mLayerTable[i].name = readWName();
        int field = readInt32();
        mLayerTable[i].state = field & 0xf;
        mLayerTable[i].trase = (field >> 4) & 0xf;
        mLayerTable[i].trpos = (field >> 8) & 0x1f;
        mLayerTable[i].color = readInt32();
        mLayerTable[i].num   = readInt32();
        mLayerTable[i].id    = readInt32();
        mLayerTable[i].pair  = readInt32();
        mLayerTable[i].flag  = readInt32();
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





int SdPasCadImport::readInt32()
  {
  char data[4];
  mFile->read( data, 4 );
  return (data[0] & 0xff) | ((data[1] << 8) & 0xff00) | ((data[2] << 16) & 0xff0000) | (data[3] << 24);
  }




bool SdPasCadImport::readBool()
  {
  char data[4];
  mFile->read( data, 4 );
  return data[0] || data[1] || data[2] || data[3];
  }



//Read variable len string
QString SdPasCadImport::readNString()
  {
  static QTextCodec *win1251 = QTextCodec::codecForName("Windows-1250");
  int len = readInt32();
  QByteArray data = mFile->read(len+1);
  if( win1251 == nullptr )
    return QString::fromLocal8Bit( data );
  return win1251->toUnicode( data );
  }



QString SdPasCadImport::readConstString(int len)
  {
  static QTextCodec *win1251 = QTextCodec::codecForName("Windows-1250");
  QByteArray data = mFile->read(len);
  if( win1251 == nullptr )
    return QString::fromLocal8Bit( data );
  return win1251->toUnicode( data );
  }




bool SdPasCadImport::error(QString msg)
  {
  QMessageBox::warning( mOwner, QObject::tr("Error!"), msg );
  return false;
  }

