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
#include "SdJsonIO.h"
#include "SvLib/SvTime2x.h"

#include <QClipboard>
#include <QByteArray>
#include <QMimeData>
#include <QCborValue>
#include <QGuiApplication>
#include <QImage>
#include <QPainter>
#include <QtSvg/QSvgGenerator>
#include <QBuffer>
#include <QByteArray>

SdSelector::SdSelector() :
  SdObject(),
  mOwner(false)
  {

  }




SdSelector::~SdSelector()
  {
  removeAll();
  }




//!
//! \brief selectAll Select into selector all object from container which match to mask
//! \param container Container from which need to be select all objects
//! \param mask      Mask for object prediction
//!
void SdSelector::selectAll(SdContainer *container, quint64 mask)
  {
  container->forEach( mask, [this] (SdObject *obj) -> bool {
    SdPtr<SdGraph> graph(obj);
    if( graph.isValid() )
      graph->select( this );
    return true;
    });
  }




void SdSelector::markDeleteAll()
  {
  for( SdGraph *graph : qAsConst(mTable) ) {
    graph->markDeleted(true);
    }
  }




void SdSelector::removeAll()
  {
  for( SdGraph *graph : qAsConst(mTable) )
    graph->mSelector = nullptr;
  mTable.clear();
  }




void SdSelector::remove(SdGraph *graph)
  {
  if( graph->mSelector == this ) {
    graph->mSelector = nullptr;
    mTable.removeOne( graph );
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
  mTable.append( graph );  //Добавить ссылку на объект в массив
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




void SdSelector::putToClipboard(const SdProject *project, double scale , SdWEditor::SdCopyFormat format)
  {
  //Prepare Json object with project and selection
  SdJsonWriter js;

  //Write project
  js.jsonBuildPtr( js, QStringLiteral("Project"), project );

  //Write selection
  js.jsonLeavePtr( js, QStringLiteral("Selector"), this );

  //Convert to byteArray
  QByteArray array = svJsonObjectToCbor( js.object() );

  SdRect r = getOverRect();    //Охватывающий прямоугольник

  //Prepare mime data
  QMimeData *mime = new QMimeData();

  if( format == SdWEditor::SdCopyFormat::sdcfDefault ) {
    mime->setData( QStringLiteral(SD_CLIP_FORMAT_SELECTOR), array );

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
    }
  else if( format == SdWEditor::SdCopyFormat::sdcfSvg ) {
    //Yet alternative copy as svg
    QSvgGenerator svgGenerator;
    QByteArray svgArray;
    QBuffer svgBuffer( &svgArray );
    svgGenerator.setOutputDevice( &svgBuffer );
    //svgGenerator.setResolution( 25 );
    svgGenerator.setSize( QSize(600,400) );
    svgGenerator.setViewBox( QRect( 0, 0, r.width(), r.height() ) );
    svgGenerator.setTitle( "SaliCAD SVG drawing" );
    svgGenerator.setDescription( "An SVG drawing created by the SaliCAD SVG Generator " );
    QPainter svgPainter;
    svgPainter.begin( &svgGenerator );

    //Draw context
    SdContext svgCtx( SdPoint(10,10), &svgPainter );
    //View converter
    SdConverterView svgView( r.size(), r.center(), 1 );
    svgCtx.setConverter( &svgView );
    //Draw process
    draw( &svgCtx );

    svgPainter.end();
    //Copy drawing as svg text
    mime->setText( QString::fromUtf8(svgArray) );
    }
  else if( format == SdWEditor::SdCopyFormat::sdcfWeb ) {
    //Yet another alternative - textual packet object
    QByteArray packedHex = qCompress( array ).toHex();
    QString packedString;
    packedString.reserve( 20 + (packedHex.length() / 80 + 1) * 81 );
    packedString.append( QStringLiteral("`SaliCAD compressed") );
    for( int i = 0; i < packedHex.length(); i += 80 ) {
      packedString.append( QChar('\n') );
      packedString.append( packedHex.mid( i, 80 ) );
      }
    packedString.append( QChar('`') );

    //Build web page
    int fragmentId = SvTime2x::current();
    mime->setText( QStringLiteral( "<script>\nfunction fragment%1() {\nnavigator.clipboard.writeText(%2)\n"
                                   ".then(() => {\n   // Success!\n })\n"
                                   ".catch(() => {\n   // Fail :(\n });\n"
                                   "  }\n</script>\n"
                                   "<button onclick=\"fragment%1();\">Copy fragment %1</button>" ).arg(fragmentId).arg(packedString) );
    }



  //Insert into clipboard
  QGuiApplication::clipboard()->setMimeData( mime );
  }





SdProject *SdSelector::getFromClipboard()
  {
  const QMimeData *mime = QGuiApplication::clipboard()->mimeData();
  if( mime != nullptr && mime->hasFormat( QStringLiteral(SD_CLIP_FORMAT_SELECTOR)) ) {
    //Data with appropriate format present, read it

    //Retrive Json object from clipboard
    QJsonObject obj = QCborValue::fromCbor( mime->data(QStringLiteral(SD_CLIP_FORMAT_SELECTOR)) ).toJsonValue().toObject();

    SdObjectMap map;
    SdJsonReader js( obj, &map );

    //Create project
    SdProject *project = nullptr;

    //Project reading
    js.jsonBuildPtr( js, QStringLiteral("Project"), project );

    //selection reading
    js.jsonLeavePtr( js, QStringLiteral("Selector"), this );

    return project;
    }

  //Try convert from textual coding
  if( mime != nullptr && mime->hasText() ) {
    QString src = mime->text();
    if( src.startsWith( QStringLiteral("SaliCAD compressed") )  ) {
      //This contens is SaliCAD compressed representation
      QByteArray packedHex;
      packedHex.reserve( src.size() );
      //Remove service information
      for( int i = 19; i < src.size(); i++ )
        if( src.at(i).isLetterOrNumber() )
          packedHex.append( src.at(i).toLatin1() );

      QJsonObject obj = QCborValue::fromCbor( qUncompress(QByteArray::fromHex(packedHex)) ).toJsonValue().toObject();

      SdObjectMap map;
      SdJsonReader js( obj, &map );

      //Create project
      SdProject *project = nullptr;

      //Project reading
      js.jsonBuildPtr( js, QStringLiteral("Project"), project );

      //selection reading
      js.jsonLeavePtr( js, QStringLiteral("Selector"), this );

      return project;
      }
    }

  //No data in clipboard or wrong data format
  return nullptr;
  }






//!
//! \brief storeSelectionToFile Store selection picture to file
//! \param scale                Current scale factor for editor
//! \param fname                File name for stored picture
//!
void SdSelector::storeSelectionToFile( double scale, const QString fname)
  {
  SdRect r = getOverRect();    //Охватывающий прямоугольник
  if( fname.endsWith( QStringLiteral(".svg") )  ) {
    //Store as svg file
    QSvgGenerator svgGenerator;
    svgGenerator.setFileName( fname );
    //svgGenerator.setResolution( 25 );
    svgGenerator.setSize( QSize(1200,800) );
    svgGenerator.setViewBox( QRect( 0, 0, r.width(), r.height() ) );
    svgGenerator.setTitle( "SaliCAD SVG drawing" );
    svgGenerator.setDescription( "An SVG drawing created by the SaliCAD SVG Generator " );
    QPainter svgPainter;
    svgPainter.begin( &svgGenerator );

    //Draw context
    SdContext svgCtx( SdPoint(10,10), &svgPainter );
    svgCtx.setOverZeroWidth(2);
    //View converter
    SdConverterView svgView( r.size(), r.center(), 1 );
    svgCtx.setConverter( &svgView );
    //Draw process
    draw( &svgCtx );

    svgPainter.end();
    }
  else {
    //Store as png file
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

    //Store image
    image.save( fname );
    }
  }




//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void SdSelector::json(SdJsonWriter &js) const
  {
  //Origin
  js.jsonPoint( QStringLiteral("FragmentOrigin"), mOrigin );

  //Count of objects
  int count = mTable.count();
  js.jsonInt( QStringLiteral("Count"), count );
  count = 0;
  for( SdGraphPtr ptr : mTable )
    js.jsonObjectPtr( QString::number(count++), ptr );

  //Base object
  SdObject::json( js );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void SdSelector::json(const SdJsonReader &js)
  {
  clear();
  mOwner = false;

  //Origin
  js.jsonPoint( QStringLiteral("FragmentOrigin"), mOrigin );

  //Count of objects
  int count;
  js.jsonInt( QStringLiteral("Count"), count );

  SdGraphPtr ptr;
  for( int i = 0; i < count; i++ ) {
    js.jsonObjectPtr( QString::number(i), ptr );
    ptr->select( this );
    }

  //Base object
  SdObject::json( js );
  }








void SdSelector::forEach(quint64 classMask, std::function<bool (SdGraph *)> fun1)
  {
  for( int i = 0; i < mTable.count(); i++ ) {
    SdGraph *graph = mTable.at(i);
    if( graph && !graph->isDeleted() && (graph->getClass() & classMask) ) {
      if( !fun1(graph) ) return;
      }
    }
  }



SdRect SdSelector::getOverRect()
  {
  if( mTable.count() ) {
    SdRect r;
    bool first = true;
    for( SdGraph *graph : qAsConst(mTable) )
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
  for( SdGraph *graph : qAsConst(mTable) )
    if( graph != nullptr && !graph->isDeleted() )
      graph->draw( ctx );
  }





bool SdSelector::isClipboardAvailable()
  {
  const QMimeData *mime = QGuiApplication::clipboard()->mimeData();
  return mime != nullptr && (mime->hasFormat( QStringLiteral(SD_CLIP_FORMAT_SELECTOR) ) || mime->hasText());
  }


