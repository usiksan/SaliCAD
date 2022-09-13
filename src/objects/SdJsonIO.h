#ifndef SDJSONIO_H
#define SDJSONIO_H

#include "SvLib/SvJson3dIO.h"
#include "SdObjectMap.h"
#include "SdObject.h"
#include "SdRect.h"
#include "Sd3dPoint.h"


class SdJsonWriter : public SvJsonWriter3d
  {
  public:
    //!
    //! \brief SdJsonWriter Constructor with external json object as destignator
    //! \param obj          External destignator json object
    //!
    SdJsonWriter( QJsonObject &obj ) : SvJsonWriter3d(obj) {}


    //!
    //! \brief SvJsonWriter Constructor with internal json object as destignator
    //!
    SdJsonWriter() : SvJsonWriter3d() {}


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

  };


class SdJsonReader : public SvJsonReader3dExt<SdObjectMap>
  {
  public:
    //!
    //! \brief SvJsonReader Constructor for reader
    //! \param obj          Object which json readed
    //!
    SdJsonReader( const QJsonObject &obj, SdObjectMap *prop ) : SvJsonReader3dExt<SdObjectMap>( obj, prop ) {}

    SdJsonReader( const QJsonObject &obj, const SdJsonReader &src ) : SvJsonReader3dExt<SdObjectMap>( obj, src.property() ) {}

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
  };

#endif // SDJSONIO_H
