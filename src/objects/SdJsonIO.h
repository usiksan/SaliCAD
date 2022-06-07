#ifndef SDJSONIO_H
#define SDJSONIO_H

#include "SvJsonIO.h"
#include "SdObjectMap.h"
#include "SdObject.h"
#include "SdRect.h"
#include "Sd3dPoint.h"


class SdJsonWriter : public SvJsonWriter
  {
  public:
    //!
    //! \brief SdJsonWriter Constructor with external json object as destignator
    //! \param obj          External destignator json object
    //!
    SdJsonWriter( QJsonObject &obj ) : SvJsonWriter(obj) {}


    //!
    //! \brief SvJsonWriter Constructor with internal json object as destignator
    //!
    SdJsonWriter() : SvJsonWriter() {}


    void jsonObjectPtr( const QString &key, const SdObjectPtr ptr )
      {
      SdJsonWriter js( *this );
      SdObject::writePtr( ptr, js );
      object().insert( key, js.object() );
      }

    void jsonRect( const QString &key, const SdRect &r )
      {
      SvJsonWriter js;
      r.json(js);
      object().insert( key, js.object() );
      }

    void json3dPoint( const QString &key, const Sd3dPoint &p )
      {
      SvJsonWriter js;
      js.jsonDouble( QStringLiteral("x"), p.x() );
      js.jsonDouble( QStringLiteral("y"), p.y() );
      js.jsonDouble( QStringLiteral("z"), p.z() );
      object().insert( key, js.object() );
      }

    QJsonArray matrix4x4( const QMatrix4x4 &map )
      {
      float val[16];
      map.copyDataTo(val);
      QJsonArray ar;
      for( int i = 0; i < 16; i++ )
        ar.append( val[i] );
      return ar;
      }

    void jsonMatrix4x4( const QString &key, const QMatrix4x4 &map )
      {
      object().insert( key, matrix4x4(map) );
      }

    void jsonListMatrix4x4( const QString &key, const QList<QMatrix4x4> &list )
      {
      QJsonArray ar;
      for( auto const &map : list )
        ar.append( matrix4x4(map) );
      object().insert( key, ar );
      }

  };


class SdJsonReader : public SvJsonReaderExt<SdObjectMap>
  {
  public:
    //!
    //! \brief SvJsonReader Constructor for reader
    //! \param obj          Object which json readed
    //!
    SdJsonReader( const QJsonObject &obj, SdObjectMap *prop ) : SvJsonReaderExt<SdObjectMap>( obj, prop ) {}

    SdJsonReader( const QJsonObject &obj, const SdJsonReader &src ) : SvJsonReaderExt<SdObjectMap>( obj, src.property() ) {}

    template <class SvClassPtr>
    void jsonObjectPtr( const QString &key, SvClassPtr &ptr ) const
      {
      SdJsonReader js( object().value(key).toObject(), *this );
      ptr = dynamic_cast<SvClassPtr>( SdObject::readPtr( js ) );
      }

    void jsonRect( const QString &key, SdRect &r ) const
      {
      SvJsonReader js( object().value( key ).toObject() );
      r.json(js);
      }

    void json3dPoint( const QString &key, Sd3dPoint &p ) const
      {
      QJsonObject obj = object().value( key ).toObject();
      p.setX( obj.value( QStringLiteral("x") ).toDouble() );
      p.setY( obj.value( QStringLiteral("y") ).toDouble() );
      p.setZ( obj.value( QStringLiteral("z") ).toDouble() );
      }

    QMatrix4x4 matrix4x4( const QJsonArray &ar ) const
      {
      float val[16];
      bool notNull = false;
      for( int i = 0; i < 16; i++ ) {
        val[i] = ar.at(i).toDouble();
        notNull = notNull || val[i] != 0;
        }
      return notNull ? QMatrix4x4(val) : QMatrix4x4();
      }

    void jsonMatrix4x4( const QString &key, QMatrix4x4 &map ) const
      {
      map = matrix4x4( object().value(key).toArray() );
      }

    void jsonListMatrix4x4( const QString &key, QList<QMatrix4x4> &list ) const
      {
      QJsonArray ar = object().value(key).toArray();
      list.clear();
      list.reserve(ar.size());
      for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
        QMatrix4x4 v( matrix4x4(i->toArray()) );
        list.append( v );
        }
      }

  };

#endif // SDJSONIO_H
