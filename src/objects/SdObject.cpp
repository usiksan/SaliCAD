/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Root object of Database object hierarhy
*/

#include "SdObject.h"
#include "SdCopyMap.h"
#include "SdContainer.h"
#include "SdProject.h"
#include "SdPItemSymbol.h"
#include "SdPItemPart.h"
#include "SdPItemComponent.h"
#include "SdPItemSheet.h"
#include "SdPItemPlate.h"
#include "SdPItemRich.h"
#include "SdPadAssociation.h"
#include "SdGraphArea.h"
#include "SdGraphLinearArc.h"
#include "SdGraphLinearLine.h"
#include "SdGraphLinearRect.h"
#include "SdGraphLinearRectFilled.h"
#include "SdGraphLinearCircle.h"
#include "SdGraphLinearCircleFilled.h"
#include "SdGraphLinearRegion.h"
#include "SdGraphLinearRegionFilled.h"
#include "SdGraphTracedRoad.h"
#include "SdGraphTracedPolygon.h"
#include "SdGraphTracedVia.h"
#include "SdGraphText.h"
#include "SdGraphIdent.h"
#include "SdGraphValue.h"
#include "SdGraphSymPin.h"
#include "SdGraphPartPin.h"
#include "SdGraphIdent.h"
#include "SdGraphSymImp.h"
#include "SdGraphPartImp.h"
#include "SdGraphNetWire.h"
#include "SdGraphNetName.h"
#include "SdGraphScript.h"
#include "Sd3dGraphModel.h"
#include "SdSection.h"
#include "SdPartVariant.h"
#include "library/SdLibraryStorage.h"

#include <QJsonValue>
#include <QFile>
#include <QDebug>

int SdObject::mCpidCount(1);

SdObject::SdObject() :
  mParent(nullptr),
  mCpid(0),
  mDeleted(false)
  {
  }



QString SdObject::getId() const
  {
  return QString::number( (quintptr)this, 16 );
  }



SdContainer *SdObject::getRoot() const
  {
  SdContainer *p = mParent;
  while(p) {
    if( p->mParent == nullptr ) return p;
    p = p->mParent;
    }
  return nullptr;
  }




void SdObject::setParent(SdContainer *parent)
  {
  Q_ASSERT( mParent == nullptr );
  mParent = parent;
  }





void SdObject::setProjectDirtyFlag()
  {
  //Project may be on top hierarchy
  SdProject *project = dynamic_cast<SdProject*>( getRoot() );
  if( project == nullptr )
    //or project may be object itself
    project = dynamic_cast<SdProject*>( this );

  //If project accepted then set its dirty flag
  if( project )
    project->setDirty();
  }




void SdObject::attach(SdUndo *undo)
  {
  Q_UNUSED(undo)
  }




void SdObject::detach(SdUndo *undo)
  {
  Q_UNUSED(undo)
  }



void SdObject::deleteObject(SdUndo *undo)
  {
  if( mParent && !mDeleted ) {
    //qDebug() << "deleteObject" << mParent;
    mParent->deleteChild( this, undo );
    }
  }




bool SdObject::isUsed(SdObject *obj) const
  {
  Q_UNUSED(obj)
  return false;
  }





//Signal to replace item
bool SdObject::upgradeProjectItem(SdUndo *undo, QWidget *parent)
  {
  Q_UNUSED(parent)
  Q_UNUSED(undo)
  return true;
  }





//!
//! \brief copy    Returns copy of this object. Copy is not exact but modified with copyMap
//! \param copyMap Structure for mapping copying substitutes
//! \param next    Make simple or next copy. Next copy available not for all objects.
//!                For example: pin name A23 with next copy return A24
//! \return        Copy of this object
//!
SdObjectPtr SdObject::copy(SdCopyMap &copyMap, bool next) const
  {
  //Build object of current type
  SdObject *obj = build( getType() );
  //Make it as clone of this object
  obj->cloneFrom( this, copyMap, next );
  //Return cloned object
  return obj;
  }





//!
//! \brief cloneFrom Clone contents object except mParent field. This must be overrided in all subclasses to
//!                  generate right copy of object
//!                  Cloned object has no parent
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdObject::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  Q_UNUSED(copyMap)
  Q_UNUSED(next)
  if( getType() != src->getType() ) {
    qDebug() << Q_FUNC_INFO << "Illegal clone source";
    throw( QString("Illegal clone source") );
    }
  mCpid = src->mCpid;
  }






//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void SdObject::json(SdJsonWriter &js) const
  {
  js.jsonString( QStringLiteral(SDKO_ID), getId() );
  js.jsonString( QStringLiteral(SDKO_TYPE), getType() );

  js.jsonInt( QStringLiteral("Cpid"), mCpid );
  }






//!
//! \brief json Overloaded function to read object content from json reader
//! \param js   Json reader
//!
void SdObject::json( const SdJsonReader &js )
  {
  //At this moment object is fully readed
  js.jsonInt( QStringLiteral("Cpid"), mCpid, 0 );
  }






QJsonObject SdObject::jsonObjectTo() const
  {
  SdJsonWriter js;
  //Prepare version and type
  int version = SD_BASE_VERSION;
  QString jsonType(SD_BASE_TYPE);

  //Store version and type
  js.jsonInt( "A version", version, 0 );
  js.jsonString( "A file type", jsonType );

  json( js );

  return js.object();
  }





SdObject *SdObject::jsonObjectFrom(const QJsonObject obj)
  {
  SdJsonReaderProperty map;
  SdJsonReader js( obj, &map );
  js.jsonInt( "A version", map.mVersion, SD_BASE_VERSION_1 );
  return buildFromJson( js );
  }






QByteArray SdObject::jsonTextTo() const
  {
  //Return QByteArray representation
  //return QJsonDocument( jsonObjectTo() ).toJson( QJsonDocument::Compact );
  return svJsonObjectToByteArray( jsonObjectTo() );
  }




SdObject *SdObject::jsonTextFrom(const QByteArray &ar)
  {
  //Build from QByteArray representation
  QJsonDocument doc = QJsonDocument::fromJson( ar, nullptr );
  if( !doc.isEmpty() )
    return jsonObjectFrom( doc.object() );
  return nullptr;
  }




//Convert object to-from binary cbor format
QByteArray SdObject::jsonCborTo() const
  {
  return QCborValue::fromJsonValue( jsonObjectTo() ).toCbor();
  }



SdObject *SdObject::jsonCborFrom(const QByteArray &ar)
  {
  return jsonObjectFrom( QCborValue::fromCbor( ar ).toJsonValue().toObject() );
  }





//Convert object to-from binary cbor compressed format
QByteArray SdObject::jsonCborCompressedTo() const
  {
  return qCompress( jsonCborTo() );
  }

SdObject *SdObject::jsonCborCompressedFrom(const QByteArray &ar)
  {
  return jsonCborFrom( qUncompress(ar) );
  }





//Read-write to-from file
bool SdObject::fileJsonSave(const QString fname) const
  {
  QFile file(fname);
  if( file.open(QIODevice::WriteOnly) ) {
    if( fname.endsWith( QStringLiteral(SD_BASE_EXTENSION)) )
      file.write( jsonTextTo() );
    else
      file.write( jsonCborCompressedTo() );
    file.close();
    return true;
    }
  return false;
  }



SdObject *SdObject::fileJsonLoad(const QString fname)
  {
  QFile file(fname);
  if( file.open(QIODevice::ReadOnly) ) {
    if( fname.endsWith( QStringLiteral(SD_BASE_EXTENSION) ) || fname.endsWith( QStringLiteral(SD_SALICAD_EXTENSION) )  )
      return jsonTextFrom( file.readAll() );
    if( fname.endsWith( QStringLiteral(SD_BINARY_EXTENSION)) || fname.endsWith( QStringLiteral(SD_SALICAD_BIN_EXTENSION) ) )
      return jsonCborCompressedFrom( file.readAll() );
    }
  return nullptr;
  }







void SdObject::writePtr(const SdObject *ptr, SdJsonWriter &js)
  {
  if( ptr ) {
    js.jsonString( QStringLiteral(SDKO_ID), ptr->getId() );
    js.jsonString( QStringLiteral(SDKO_TYPE), ptr->getType() );
    }
  else
    js.jsonString( QStringLiteral(SDKO_ID), QString{} );
  }







SdObject *SdObject::readPtr(const SdJsonReader &js)
  {
  //Get object id
  QString id;
  js.jsonString( QStringLiteral(SDKO_ID), id );

  //If id equals zero then no object
  if( id.isEmpty() )
    return nullptr;

  //Check if object already in the map
  if( js.property()->mMap.contains(id) )
    return js.property()->mMap.value(id);

  //Build new object
  QString type;
  js.jsonString( QStringLiteral(SDKO_TYPE), type );
  SdObject *r = build( type );
  //Register new object in the map
  js.property()->mMap.insert( id, r );
  //and return new object
  return r;
  }





SdObject *SdObject::build(QString type)
  {
  if( type == QStringLiteral(SD_TYPE_ARC)                 ) return new SdGraphLinearArc();
  if( type == QStringLiteral(SD_TYPE_LINE)                ) return new SdGraphLinearLine();
  if( type == QStringLiteral(SD_TYPE_RECT)                ) return new SdGraphLinearRect();
  if( type == QStringLiteral(SD_TYPE_RECT_FILLED)         ) return new SdGraphLinearRectFilled();
  if( type == QStringLiteral(SD_TYPE_CIRCLE)              ) return new SdGraphLinearCircle();
  if( type == QStringLiteral(SD_TYPE_CIRCLE_FILLED)       ) return new SdGraphLinearCircleFilled();
  if( type == QStringLiteral(SD_TYPE_REGION)              ) return new SdGraphLinearRegion();
  if( type == QStringLiteral(SD_TYPE_REGION_FILLED)       ) return new SdGraphLinearRegionFilled();
  if( type == QStringLiteral(SD_TYPE_TEXT)                ) return new SdGraphText();
  if( type == QStringLiteral(SD_TYPE_IDENT)               ) return new SdGraphIdent();
  if( type == QStringLiteral(SD_TYPE_VALUE)               ) return new SdGraphValue();
  if( type == QStringLiteral(SD_TYPE_SYM_PIN)             ) return new SdGraphSymPin();
  if( type == QStringLiteral(SD_TYPE_PART_PIN)            ) return new SdGraphPartPin();
  if( type == QStringLiteral(SD_TYPE_AREA)                ) return new SdGraphArea();
  if( type == QStringLiteral(SD_TYPE_IDENT)               ) return new SdGraphIdent();
  if( type == QStringLiteral(SD_TYPE_SYM_IMP)             ) return new SdGraphSymImp();
  if( type == QStringLiteral(SD_TYPE_PART_IMP)            ) return new SdGraphPartImp();
  if( type == QStringLiteral(SD_TYPE_NET_WIRE)            ) return new SdGraphNetWire();
  if( type == QStringLiteral(SD_TYPE_NET_NAME)            ) return new SdGraphNetName();
  if( type == QStringLiteral(SD_TYPE_SCRIPT)              ) return new SdGraphScript();

  if( type == QStringLiteral(SD_TYPE_GRAPH_TRACE_ROAD)    ) return new SdGraphTracedRoad();
  if( type == QStringLiteral(SD_TYPE_GRAPH_TRACE_POLYGON) ) return new SdGraphTracedPolygon();
  if( type == QStringLiteral(SD_TYPE_GRAPH_TRACE_VIA)     ) return new SdGraphTracedVia();

  if( type == QStringLiteral(SD_TYPE_SYMBOL)              ) return new SdPItemSymbol();
  if( type == QStringLiteral(SD_TYPE_PART)                ) return new SdPItemPart();
  if( type == QStringLiteral(SD_TYPE_SHEET)               ) return new SdPItemSheet();
  if( type == QStringLiteral(SD_TYPE_PLATE)               ) return new SdPItemPlate();
  if( type == QStringLiteral(SD_TYPE_COMPONENT)           ) return new SdPItemComponent();
  if( type == QStringLiteral(SD_TYPE_RICH)                ) return new SdPItemRich();

  if( type == QStringLiteral(SD_TYPE_SECTION)             ) return new SdSection();
  if( type == QStringLiteral(SD_TYPE_PART_VARIANT)        ) return new SdPartVariant();

  if( type == QStringLiteral(SD_TYPE_PAD_ASSOCIATION)     ) return new SdPadAssociation();

  if( type == QStringLiteral(SD_TYPE_3D_GRAPH_MODEL)      ) return new Sd3dGraphModel();

  if( type == QStringLiteral(SD_TYPE_PROJECT)             ) return new SdProject();
  return nullptr;
  }




SdObject *SdObject::buildFromJson(const SdJsonReader &js)
  {
  SdObject *obj = readPtr( js );
  if( obj != nullptr )
    obj->json( js );
  return obj;
  }




