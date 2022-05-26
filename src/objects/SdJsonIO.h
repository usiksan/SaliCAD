#ifndef SDJSONIO_H
#define SDJSONIO_H

#include "SvJsonIO.h"
#include "SdObjectMap.h"
#include "SdObject.h"
#include "SdRect.h"


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
  };

#endif // SDJSONIO_H
