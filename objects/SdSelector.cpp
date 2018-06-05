/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdSelector.h"
#include "SdRect.h"
#include "SdProject.h"
#include "SdContext.h"
#include "SdConverterView.h"

#include <QClipboard>
#include <QByteArray>
#include <QMimeData>
#include <QJsonDocument>
#include <QGuiApplication>
#include <QImage>
#include <QPainter>

SdSelector::SdSelector() :
  SdObject(),
  mOwner(false)
  {

  }




SdSelector::~SdSelector()
  {
  clear();
  }




void SdSelector::markDeleteAll()
  {
  for( SdGraph *graph : mTable ) {
    graph->markDeleted(true);
    }
  }




void SdSelector::removeAll()
  {
  for( SdGraph *graph : mTable )
    graph->mSelector = nullptr;
  mTable.clear();
  }




void SdSelector::remove(SdGraph *graph)
  {
  if( graph->mSelector == this ) {
    graph->mSelector = nullptr;
    mTable.remove( graph );
    }
  }




void SdSelector::insert(SdGraph *graph)
  {
  //Залезть в объект и проверить текущий селектор
  if( graph->mSelector ) {
    //Объект уже принадлежит какому-то селектору
    if( graph->mSelector != this ) {
      //Это другой селектор, освободить объект от предыдущего селектора
      graph->mSelector->remove( graph );
      }
    else return;
    }
  graph->mSelector = this; //Объект принадлежит данному селектору
  mTable.insert( graph );  //Добавить ссылку на объект в массив
  }




void SdSelector::clear()
  {
  if( mOwner ) qDeleteAll(mTable);
  mTable.clear();
  }




void SdSelector::operator =(const SdSelector &sour)
  {
  mOwner  = false;
  mOrigin = sour.mOrigin;
  mTable  = sour.mTable;
  }




void SdSelector::putToClipboard( const SdProject *project, double scale )
  {
  //Prepare Json object with project and selection
  QJsonObject obj;

  //Write project
  project->writeObject( obj );

  //Write selection
  writeObject( obj );

  //Convert to byteArray
  QByteArray array = QJsonDocument( obj ).toBinaryData();

  //Prepare mime data
  QMimeData *mime = new QMimeData();
  mime->setData( QStringLiteral(SD_CLIP_FORMAT_SELECTOR), array );

  SdRect r = getOverRect();    //Охватывающий прямоугольник
  SdPoint a = r.getBottomLeft();
  SdPoint b = r.getTopRight();
  a.move( SdPoint(-10,-10) );
  b.move( SdPoint(10,10) ); //Расширить, чтобы вошли пограничные объекты
  r.set( a, b );
  QSize s;                             //Размер битовой карты в пикселах
  s.setWidth( qMin( static_cast<int>(r.width() / scale + 10), CLIP_IMAGE_WIDTH ) ); //Вычисление размера
  s.setHeight( qMin( static_cast<int>(r.height() / scale + 10), CLIP_IMAGE_HEIGHT ) );


  //Alternative copy as image
  QImage image( s, QImage::Format_ARGB32 );
  //Fill white color
  image.fill( Qt::white );
  //Qt painter
  QPainter painter( &image );
  //Draw context
  SdContext ctx( SdPoint(10,10), &painter );
  //View converter
  SdConverterView view( s, r.center(), scale );
  ctx.setConverter( &view );
  //Draw process
  draw( &ctx );

  //Put picture into mime
  mime->setImageData( image );


  //Insert into clipboard
  QGuiApplication::clipboard()->setMimeData( mime );
  }





SdProject *SdSelector::getFromClipboard()
  {
  const QMimeData *mime = QGuiApplication::clipboard()->mimeData();
  if( mime != nullptr && mime->hasFormat( QStringLiteral(SD_CLIP_FORMAT_SELECTOR)) ) {
    //Data with appropriate format present, read it

    //Retrive Json object from clipboard
    QJsonObject obj = QJsonDocument::fromBinaryData( mime->data(QStringLiteral(SD_CLIP_FORMAT_SELECTOR)) ).object();

    //Create project
    SdProject *project = new SdProject();
    SdObjectMap map;

    //Project reading
    project->readObject( &map, obj );

    //selection reading
    readObject( &map, obj );

    return project;
    }
  //No data in clipboard or wrong data format
  return nullptr;
  }




void SdSelector::writeObject(QJsonObject &obj) const
  {
  //Write base object
  SdObject::writeObject( obj );

  //Origin
  mOrigin.write( QStringLiteral("FragmentOrigin"), obj );

  //Count of objects
  int count = mTable.count();
  obj.insert( QStringLiteral("Count"), count );
  count = 0;
  for( SdGraphPtr ptr : mTable )
    writePtr( ptr, QString::number(count++), obj );
  }



void SdSelector::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  clear();
  mOwner = false;

  //Read base object
  SdObject::readObject( map, obj );

  //Origin
  mOrigin.read( QStringLiteral("FragmentOrigin"), obj );

  //Count of objects
  int count = obj.value( QStringLiteral("Count") ).toInt();
  for( int i = 0; i < count; i++ ) {
    SdGraphPtr ptr = dynamic_cast<SdGraphPtr>( readPtr( QString::number(i), map, obj ) );
    ptr->select( this );
    }
  }



void SdSelector::forEach(quint64 classMask, std::function<bool (SdGraph *)> fun1)
  {
  for( SdGraph *graph : mTable )
    if( graph && !graph->isDeleted() && (graph->getClass() & classMask) ) {
      if( !fun1(graph) ) return;
      }
  }



SdRect SdSelector::getOverRect()
  {
  if( mTable.count() ) {
    SdRect r;
    bool first = true;
    for( SdGraph *graph : mTable )
      if( !graph->isDeleted() ) {
        if( first ) {
          r = graph->getOverRect();
          first = false;
          }
        else r.grow( graph->getOverRect() );
        }
    return r;
    }
  return SdRect();
  }




void SdSelector::draw(SdContext *ctx)
  {
  for( SdGraph *graph : mTable )
    if( graph != nullptr && !graph->isDeleted() )
      graph->draw( ctx );
  }





bool SdSelector::isClipboardAvailable()
  {
  const QMimeData *mime = QGuiApplication::clipboard()->mimeData();
  return mime != nullptr && mime->hasFormat( QStringLiteral(SD_CLIP_FORMAT_SELECTOR) );
  }


