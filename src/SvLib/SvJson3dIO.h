/*
  Project "SvLib common library."
  Author
    Alexander Sibilev
  Internet
    www.salilab.com

  Description
    Extends SvJsonIO to support some 3d objects
  History
    25.07.2022 v1 Begin version support
*/
#ifndef SVJSON3DIO_H
#define SVJSON3DIO_H


#include "SvJsonIO.h"

#include <QVector3D>
#include <QVector4D>
#include <QQuaternion>
#include <QMatrix4x4>

class SvJsonWriter3d : public SvJsonWriter
  {
  public:
    //!
    //! \brief SdJsonWriter Constructor with external json object as destignator
    //! \param obj          External destignator json object
    //!
    SvJsonWriter3d( QJsonObject &obj ) : SvJsonWriter(obj) {}


    //!
    //! \brief SvJsonWriter Constructor with internal json object as destignator
    //!
    SvJsonWriter3d() : SvJsonWriter() {}



    void jsonVector3d( const QString &key, const QVector3D &p )
      {
      SvJsonWriter js;
      js.jsonDouble( QStringLiteral("x"), p.x() );
      js.jsonDouble( QStringLiteral("y"), p.y() );
      js.jsonDouble( QStringLiteral("z"), p.z() );
      object().insert( key, js.object() );
      }


    void jsonVector4d( const QString &key, const QVector4D &v )
      {
      SvJsonWriter js;
      js.jsonDouble( QStringLiteral("x"), v.x() );
      js.jsonDouble( QStringLiteral("y"), v.y() );
      js.jsonDouble( QStringLiteral("z"), v.z() );
      js.jsonDouble( QStringLiteral("w"), v.w() );
      object().insert( key, js.object() );
      }



    void jsonQuaternion( const QString &key, const QQuaternion &q )
      {
      SvJsonWriter js;
      js.jsonDouble( QStringLiteral("x"), q.x() );
      js.jsonDouble( QStringLiteral("y"), q.y() );
      js.jsonDouble( QStringLiteral("z"), q.z() );
      js.jsonDouble( QStringLiteral("s"), q.scalar() );
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

    void jsonListVector3d( const QString &key, const QList<QVector3D> &list )
      {
      QJsonArray ar;
      for( auto const &v : list ) {
        ar.append( v.x() );
        ar.append( v.y() );
        ar.append( v.z() );
        }
      object().insert( key, ar );
      }
  };




class SvJsonReader3d : public SvJsonReader
  {
  public:
    //!
    //! \brief SvJsonReader Constructor for reader
    //! \param obj          Object which json readed
    //!
    SvJsonReader3d( const QJsonObject &obj ) : SvJsonReader( obj ) {}

    SvJsonReader3d( const QJsonObject &obj, const SvJsonReader3d &reader ) : SvJsonReader( obj, reader ) {}

    void jsonVector3d( const QString &key, QVector3D &p ) const
      {
      QJsonObject obj = object().value( key ).toObject();
      p.setX( obj.value( QStringLiteral("x") ).toDouble() );
      p.setY( obj.value( QStringLiteral("y") ).toDouble() );
      p.setZ( obj.value( QStringLiteral("z") ).toDouble() );
      }


    void jsonVector4d( const QString &key, QVector4D &v ) const
      {
      QJsonObject obj = object().value( key ).toObject();
      v.setX( obj.value( QStringLiteral("x") ).toDouble() );
      v.setY( obj.value( QStringLiteral("y") ).toDouble() );
      v.setZ( obj.value( QStringLiteral("z") ).toDouble() );
      v.setW( obj.value( QStringLiteral("w") ).toDouble() );
      }



    void jsonQuaternion( const QString &key, QQuaternion &q ) const
      {
      QJsonObject obj = object().value( key ).toObject();
      q.setX( obj.value( QStringLiteral("x") ).toDouble() );
      q.setY( obj.value( QStringLiteral("y") ).toDouble() );
      q.setZ( obj.value( QStringLiteral("z") ).toDouble() );
      q.setScalar( obj.value( QStringLiteral("s") ).toDouble() );
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

    void jsonListVector3d( const QString &key, QList<QVector3D> &list ) const
      {
      QJsonArray ar = object().value(key).toArray();
      list.clear();
      list.reserve(ar.size() / 3);
      for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
        QVector3D v;
        v.setX( i->toDouble() ); i++;
        v.setY( i->toDouble() ); i++;
        v.setZ( i->toDouble() );
        list.append( v );
        }
      }


  };




//!
//! \brief The SvJsonReaderExt template used when need translate some common param (property) to
//!                            complex reader, for example dataBase version
//!
template<typename SvProperty>
class SvJsonReader3dExt : public SvJsonReader3d
  {
    SvProperty        *mProperty;  //!< Reader property - special object to build full structure
  public:
    //!
    //! \brief SvJsonReader Constructor for reader
    //! \param obj          Object which json readed
    //!
    SvJsonReader3dExt( const QJsonObject &obj, SvProperty *prop ) : SvJsonReader3d(obj), mProperty(prop) {}

    SvJsonReader3dExt( const QJsonObject &obj, const SvJsonReader3dExt<SvProperty> &src ) : SvJsonReader3d(obj), mProperty(src.mProperty) {}

    //!
    //! \brief property Returns current property of reader
    //! \return         Property of reader
    //!
    SvProperty *property() const { return mProperty; }

  };



#endif // SVJSON3DIO_H
