/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Creation dxf file from current object
*/
#include "SdPExport_Dxf.h"
#include "objects/SdContext.h"
#include "SdDLayers.h"
#include "objects/SdEnvir.h"

#include <QPushButton>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QList>
#include <QMap>
#include <QLabel>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>

//===========================================================================================
//              DXF export context section
//Dxf context
//With this context when we 'draw' picture we actualy not draw
// Instead, we append items to dxf file
class SdDxfContext : public SdContext {
    QString           mBuffer; //Buffer export to
    SdPoint           mPos;    //Current point to store current position
    double            mScale;
    const QChar eolnChar = '\n';
  public:
    SdDxfContext() :
      SdContext( SdPoint(), new QPainter() ),
      mScale(1.0)
      {
      mPos = SdPoint::farPoint();
      }
    ~SdDxfContext() override { delete mPainter; }

    QString buffer() const { return mBuffer; }

    void    putHeader();

    void    putPicture( SdProjectItem *item );

    // SdContext interface
    virtual void setPen(int width, QColor color, int lineStyle) override;
    virtual void line(SdPoint a, SdPoint b) override;
    virtual void fillRect(SdRect r) override;
    virtual void arc(SdPoint center, SdPoint start, SdPoint stop) override;
    virtual void circle(SdPoint center, int radius) override;
    virtual void circleFill(SdPoint center, int radius) override;
    virtual void regionFill( const SdPointList &points, const SdPropLine &prop ) override;
    virtual void polygon(const SdPointList &points, const SdPolyWindowList &windows, SdLayer *layer) override;

  protected:
    //PutXXX primitives
    void put_n() { mBuffer.append( eolnChar ); }

    void puts( QString str ) { mBuffer.append( str ); }

    void puts_n( QString str ) { puts(str); put_n(); }

    void puti( int i ) { puts( QString::number(i) ); }

    void puti_n( int i ) { puti( i ); put_n(); }

    void putCoord_n( int i ) { puts_n( QString("%1").arg( mScale * static_cast<double>(i), 0, 'g', 3 ) ); }

    void putd_n( double i ) { puts( QString::number(i) ); put_n(); }

    void putGroup_n( int group ) { puts_n( QString("%1").arg(group, 3, 10, QChar(' ')) ); }

    void putGroupName_n( int group, QString name ) { putGroup_n(group); puts_n(name); }

    void putGroupNum6_n( int group, int num ) { putGroup_n(group); puts_n( QString("%1").arg( num, 6, 10, QChar(' ')) ); }

    void putHandle_n() {
      putGroup_n(5);
      static int handle = 32;
      puti_n(handle++);
      }

    void putDbItem_n( QString str ) { putGroupName_n( 100, str ); }

  };


void SdDxfContext::putHeader()
  {
  //Секция HEADER
  putGroupName_n( 0, QString("SECTION") );
  putGroupName_n( 2, QString("HEADER") );

  //Версия для AutoCAD
  putGroupName_n( 9, QString("$ACADVER") );
  putGroupName_n( 1, QString("AC1021") );

  //Направление измерения углов - против часовой стрелки
  putGroupName_n( 9, QString("$ANGDIR") );
  putGroupNum6_n( 70, 1 );

  //В милиметрах
  putGroupName_n( 9, QString("$MEASUREMENT") );
  putGroupNum6_n( 70, 1 );

  putGroupName_n( 0, QString("ENDSEC") );

  //Секция CLASSES
  putGroupName_n( 0, QString("SECTION") );
  putGroupName_n( 2, QString("CLASSES") );
  putGroupName_n( 0, QString("CLASS") );
  putGroupName_n( 1, QString("LWPOLYLINE") );
  putGroupName_n( 2, QString("AcDbPolyline") );
  putGroupName_n( 3, QString("\"AutoCAD\"") );
  putGroupNum6_n( 90, 0 );
  putGroupNum6_n( 280, 0 );
  putGroupNum6_n( 281, 1 );
  putGroupName_n( 0, QString("ENDSEC") );

  //Секция TABLES
  putGroupName_n( 0, QString("SECTION") );
  putGroupName_n( 2, QString("TABLES") );

    //Таблица типов линий
    putGroupName_n( 0, QString("TABLE") );
    putGroupName_n( 2, QString("LTYPE") );
    putHandle_n();
    putDbItem_n( QString("AcDbSymbolTable") );

    //Количество элементов в таблице типов линий
    putGroupNum6_n( 70, 3 );
    for( int i = 0; i < 3; ++i ) {
      //Запись для одной линии
      putGroupName_n( 0, QString("LTYPE") );
      putHandle_n();
      putDbItem_n( QString("AcDbSymbolTableRecord") );
      putDbItem_n( QString("AcDbLinetypeTableRecord") );
      //Имя типа линии
      putGroup_n( 2 );
      puti_n( i );
      putGroupNum6_n( 70, 0 );
      //Описание типа
      switch( i ) {
        case 0 : putGroupName_n( 3, QString("SOLID") ); break;
        case 1 : putGroupName_n( 3, QString("DOTTED") ); break;
        case 2 : putGroupName_n( 3, QString("DASHED") ); break;
        }
      putGroupNum6_n( 72, 65 ); //Выравнивание
      putGroupNum6_n( 73, 0 ); //Количество элементов шаблона
      putGroupNum6_n( 40, 0 ); //Общая длина
      }
    putGroupName_n( 0, QString("ENDTAB") );

    //Таблица слоев
    putGroupName_n( 0, QString("TABLE") );
    putGroupName_n( 2, QString("LAYER") );
    putHandle_n();
    putDbItem_n( QString("AcDbSymbolTable") );

    //Количество элементов в таблице слоев
    putGroupNum6_n( 70, sdEnvir->mLayerTable.count() );
    for( auto iter = sdEnvir->mLayerTable.cbegin(); iter != sdEnvir->mLayerTable.cend(); iter++ ) {
      SdLayer *layer = iter.value();
      //Запись для одного слоя
      putGroupName_n( 0, QString("LAYER") );
      putHandle_n();
      putDbItem_n( QString("AcDbSymbolTableRecord") );
      putDbItem_n( QString("AcDbLayerTableRecord") );
      putGroupNum6_n( 70, layer->state() != layerStateEdit ? 4 : 0 ); //Флаг блокировки слоя
      //Имя слоя
      putGroupName_n( 2, iter.key() );
      //Цвет слоя
      unsigned c = layer->color().rgb();
      int color = ((c & 0xc0)>>6) | ((c & 0xc000)>>12) | ((c & 0xc00000)>>18) | ((c & 0xc0000000)>>24 );
      if( !color ) color = 1;
      if( layer->state() == layerStateOff ) color = -color; //Флаг - слой выключен
      putGroupNum6_n( 62, color );
      //Тип линии
      putGroupName_n( 6, QString("CONTINUOUS") );
      }
    putGroupName_n( 0, QString("ENDTAB") );

    putGroupName_n( 0, QString("TABLE") );
    putGroupName_n( 2, QString("APPID") );
    putHandle_n();
    putDbItem_n( QString("AcDbSymbolTable") );
    putGroupNum6_n( 70, 1 );   //Количество элементов в таблице типов линий
    putGroupName_n( 0, QString("APPID") );
    putHandle_n();
    putDbItem_n( QString("AcDbSymbolTableRecord") );
    putDbItem_n( QString("AcDbRegAppTableRecord") );
    putGroupName_n( 2, QString("SaliCAD") );
    putGroupName_n( 0, QString("ENDTAB") );

    putGroupName_n( 0, QString("ENDSEC") );
  }

void SdDxfContext::putPicture(SdProjectItem *item)
  {
  mScale = item->getClass() & (dctPlate | dctPart) ? 0.001 : 0.01;
  //Рисунок
  putGroupName_n( 0, QString("SECTION") );
  putGroupName_n( 2, QString("ENTITIES") );
  item->forEach( dctAll, [this] (SdObject *obj) ->bool {
    SdPtr<SdGraph> graph( obj );
    if( graph.isValid() )
      graph->draw( this );
    return true;
    });
  putGroupName_n( 0, QString("ENDSEC") );

  putGroupName_n( 0, QString("EOF") );
  }



void SdDxfContext::setPen(int width, QColor color, int lineStyle)
  {

  }

void SdDxfContext::line(SdPoint a, SdPoint b)
  {
#if 0
  a = ConvertPoint(a);
  b = ConvertPoint(b);
  putin( 0 );            //Групповой код
  if( prop.width ) {
    if( dxfVersion >= pdfV2000 ) {
      putsn( "POLYLINE" ); //Линия
      putHandle();
      putDbItem( "AcDbEntity" );

      putin( 8 );             //Название слоя
      putLayerName( prop.layer );

      //putin( 6 );             //Название типа линии
      //putin( prop.type );     //Название в форме числа

      putDbItem( "AcDb2dPolyline" );

      putin( 66 );
      putin( 1 );

      putin( 10 );
      putCoord( 0 );

      putin( 20 );
      putCoord( 0 );

      putin( 30 );
      putCoord( 0 );

      putin( 40 );            //Толщина линии
      putCoord( prop.width );

      putin( 41 );            //Толщина линии
      putCoord( prop.width );

      putin( 0 );            //Групповой код
      putsn( "VERTEX" );     //Излом
      putHandle();
      putDbItem( "AcDbEntity" );

      putin( 8 );             //Название слоя
      putLayerName( prop.layer );

      putDbItem( "AcDbVertex" );
      putDbItem( "AcDb2dVertex" );

      //Start point
      putin( 10 ); //X
      putCoord( a.x );
      putin( 20 ); //Y
      putCoord( a.y );
      putin( 30 ); //Z
      putCoord( 0 );

      putin( 0 );            //Групповой код
      putsn( "VERTEX" );     //Излом
      putHandle();
      putDbItem( "AcDbEntity" );

      putin( 8 );             //Название слоя
      putLayerName( prop.layer );

      putDbItem( "AcDbVertex" );
      putDbItem( "AcDb2dVertex" );

      //Stop point
      putin( 10 ); //X
      putCoord( b.x );
      putin( 20 ); //Y
      putCoord( b.y );
      putin( 30 ); //Z
      putCoord( 0 );

      //Завершение последовательности
      putin( 0 );            //Групповой код
      putsn( "SEQEND" );     //Излом
      putHandle();
      putDbItem( "AcDbEntity" );

      putin( 8 );             //Название слоя
      putLayerName( prop.layer );
      }
    else {
      putsn( "LWPOLYLINE" );
      putHandle();
      putDbItem( "AcDbEntity" );
      putin( 8 );             //Название слоя
      putLayerName( prop.layer );
      putDbItem( "AcDbPolyline" );

      putin( 90 );  //Количество вершин
      putin( 2 );

      putin( 70 );
      putin( 0 );

      putin( 43 );
      putCoord( prop.width );

      putin( 10 );
      putCoord( a.x );

      putin( 20 );
      putCoord( a.y );

      putin( 10 );
      putCoord( b.x );

      putin( 20 );
      putCoord( b.y );
      }
    if( dxfFillVertex && dxfVersion >= pdfV14 ) {
      //Если заполняем точки соединения, то ставить кружочки
      FilledCircle( a, prop.width, prop );
      FilledCircle( b, prop.width, prop );
      }
    }
  else {
    putsn( "LINE" ); //Линия

    dxfLineProp( prop );   //Свойства линии

    putDbItem( "AcDbLine" );

    //Start point
    putin( 10 ); //X
    putCoord( a.x );
    putin( 20 ); //Y
    putCoord( a.y );
    putin( 30 ); //Z
    putCoord( 0 );

    //Stop point
    putin( 11 ); //X
    putCoord( b.x );
    putin( 21 ); //Y
    putCoord( b.y );
    putin( 31 ); //Z
    putCoord( 0 );
    }
#endif
  }

void SdDxfContext::fillRect(SdRect r)
  {

  }

void SdDxfContext::arc(SdPoint center, SdPoint start, SdPoint stop)
  {

  }

void SdDxfContext::circle(SdPoint center, int radius)
  {

  }

void SdDxfContext::circleFill(SdPoint center, int radius)
  {

  }

void SdDxfContext::regionFill(const SdPointList &points, const SdPropLine &prop)
  {

  }

void SdDxfContext::polygon(const SdPointList &points, const SdPolyWindowList &windows, SdLayer *layer)
  {

  }




//===========================================================================================
//              DXF export section
SdPExport_Dxf::SdPExport_Dxf(SdProjectItem *item, int step, SdPMasterList *list, QWidget *parent) :
  QWizardPage( parent ),
  mProjectItem(item)
  {
  //Refistration of master in master list
  setMinimumWidth(400);
  list->addMaster( tr("DXF file"), tr("Creates DXF file from current object"), step, QString(":/pic/exportDxf.png") );

  //Build interface
  QVBoxLayout *vbox = new QVBoxLayout();
  //Layers selection
  QHBoxLayout *fbox = new QHBoxLayout();
  fbox->addWidget( new QLabel(tr("Layers for DXF:")) );
  QPushButton *but = new QPushButton( tr("Layers...") );
  fbox->addWidget( but );
  connect( but, &QPushButton::clicked, this, &SdPExport_Dxf::onLayers );
  vbox->addLayout( fbox );
  setLayout( vbox );
  }



bool SdPExport_Dxf::validatePage()
  {
  //Build export data
  SdDxfContext ctx;
  ctx.putHeader();
  ctx.putPicture( mProjectItem );

  //Query file to store export
  QString str = QFileDialog::getSaveFileName( this, tr("Enter or select DXF file name") );
  if( !str.isEmpty() ) {
    QFile file(str);
    if( file.open(QIODevice::WriteOnly) ) {
      file.write( ctx.buffer().toUtf8() );
      file.close();
      QMessageBox::information( this, tr("Info!"), tr("DXF file \'%1\' successfully created.").arg(str) );
      }
    else
      QMessageBox::warning( this, tr("Error!"), tr("Can't create DXF file \'%1\'").arg(str) );
    }
  return true;
  }




//When press button "layers" we show layers dialog to on-off desired layers
void SdPExport_Dxf::onLayers()
  {
  //Create and execute layers dialog
  SdDLayers dlg( mProjectItem->getProject(), this );
  dlg.exec();
  }







