/*
  Project "SvLib common library."
  Author
    Alexander Sibilev
  Internet
    www.salilab.com

  Description
    Improving the visibility of json io
    There are two classes here: writer and reader. Signatures of calling read and write functions in
    both classes are completely identical, so calls can be copied to both the write and read functions.
  History
    05.02.2022 v1 Begin version support
    05.03.2022 v2 Append QPoint support
    06.04.2022 v3 Append value of any class support which must be have jsonWrite and jsonRead members
    21.04.2022 v4 Append map of list of any class support
    26.04.2022 v5 Full English remark
    24.05.2022 v6 Replace all const char* on QString which support both using with const char* and QString
    25.05.2022 v7 Append contains member in SvJsonWriter
    25.05.2022 v8 Refactor compound member-functions to support deriving writer and reader from base classes
    08.07.2022 v9 Append possibility to select output format for json file intended or compressed
    16.08.2022 v10 Append helper class SvObjectMap which help to save and restore single hierarhy objects
    15.09.2022 v11 Fix: jsonListCrossPtr, jsonArray, jsonArrayPtr
               v12 Fix: jsonCrossPtr writer with nullptr
    17.02.2023 v13 Append transfer binary data with function jsonHex
    17.03.2023 v14 Append transfer binary void data with function jsonHex
    30.08.2023 v15 Append transfer QByteArray with function jsonHex
    16.02.2026 v16 Append quint32 and quint64 transfer as jsonHexNN
*/
#ifndef SVJSONIO_H
#define SVJSONIO_H

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QCborValue>
#include <QColor>
#include <QMap>
#include <QPoint>

#define SV_JSON_VERSION 16

//!
//! \brief The SvJsonWriter class Unificate json io class, through which json written
//!
class SvJsonWriter
  {
    QJsonObject  mObject;    //!< Working JSON-object if it not passed by caller
    QJsonObject &mObjectRef; //!< Output JSON-object reference
  public:
    //!
    //! \brief SvJsonWriter Constructor with external json object as destignator
    //! \param obj          External destignator json object
    //!
    SvJsonWriter( QJsonObject &obj ) : mObjectRef(obj) {}


    SvJsonWriter( const SvJsonWriter& ) : mObjectRef(mObject) {}

    //!
    //! \brief SvJsonWriter Constructor with internal json object as destignator
    //!
    SvJsonWriter() : mObjectRef(mObject) {}


    //!
    //! \brief object Returns destignator json object
    //! \return       Destignator json object
    //!
    QJsonObject &object() { return mObjectRef; }

    //!
    //! \brief contains Return true if key contains in SvJsonWriter object
    //! \param key      Tested key
    //! \return         true if key contains in SvJsonWriter object
    //!
    bool contains( const QString &key ) const { return mObjectRef.contains( key ); }

    //!
    //! \brief jsonBool Transfer bool value
    //! \param key      Key for value
    //! \param b        Bool value
    //!
    void jsonBool( const QString &key, bool b ) { mObjectRef.insert( key, b ); }
    void jsonBool( const QString &key, bool b, bool ) { mObjectRef.insert( key, b ); }


    //!
    //! \brief jsonInt Transfer int value
    //! \param key     Key for value
    //! \param v       Int value
    //!
    void jsonInt( const QString &key, int v ) { mObjectRef.insert( key, v ); }
    void jsonInt( const QString &key, int v, int ) { mObjectRef.insert( key, v ); }

    //!
    //! \brief jsonHex32 Transfer unsigned as hex string
    //! \param key       Key for value
    //! \param v         Unsigned value
    //!
    void jsonHex32( const QString &key, quint32 v ) { mObjectRef.insert( key, QString::number( v, 16 ) ); }
    void jsonHex32( const QString &key, quint32 v, quint32 ) { mObjectRef.insert( key, QString::number( v, 16 ) ); }

    //!
    //! \brief jsonHex64 Transfer unsigned int 64 as hex string
    //! \param key       Key for value
    //! \param v         Unsigned int 64 value
    //!
    void jsonHex64( const QString &key, quint64 v ) { mObjectRef.insert( key, QString::number( v, 16 ) ); }
    void jsonHex64( const QString &key, quint64 v, quint64 ) { mObjectRef.insert( key, QString::number( v, 16 ) ); }


    //!
    //! \brief jsonInt Transfer color value
    //! \param key     Key for value
    //! \param v       Int value
    //!
    void jsonColor( const QString &key, QColor color ) { mObjectRef.insert( key, color.name() ); }
    void jsonColor( const QString &key, QColor color, QColor ) { mObjectRef.insert( key, color.name() ); }


    //!
    //! \brief jsonDouble Transfer double value
    //! \param key        Key for value
    //! \param d          Double value
    //!
    void jsonDouble( const QString &key, double d ) { mObjectRef.insert( key, d ); }
    void jsonDouble( const QString &key, double d, double ) { mObjectRef.insert( key, d ); }


    //!
    //! \brief jsonString Transfer string value
    //! \param key        Key for value
    //! \param s          String to transfer
    //!
    void jsonString( const QString &key, const QString &s ) { mObjectRef.insert( key, s ); }
    void jsonString( const QString &key, const QString &s, QString ) { mObjectRef.insert( key, s ); }



    //!
    //! \brief jsonHex Transfer any binary data
    //! \param key     Key for value
    //! \param ar      Binary data to transfer
    //!
    void jsonHex( const QString &key, const QByteArray ar )
      {
      mObjectRef.insert( key, QString::fromLatin1( ar.toHex() ) );
      }

    //!
    //! \brief jsonHex Transfer any binary data
    //! \param key     Key for value
    //! \param data    Binary data to transfer
    //! \param size    Size of binary data
    //!
    void jsonHex( const QString &key, const char *data, int size )
      {
      jsonHex( key, QByteArray( data, size ) );
      }

    //!
    //! \brief jsonHex Transfer any void binary data
    //! \param key     Key for value
    //! \param data    Binary data to transfer
    //! \param size    Size of binary data
    //!
    void jsonHex( const QString &key, const void *data, int size ) {
      jsonHex( key, (const char*)data, size );
      }


    //!
    //! \brief jsonPoint Transfer point value
    //! \param key       Key for value
    //! \param p         Point to transfer
    //!
    void jsonPoint( const QString &key, QPoint p ) {
      SvJsonWriter js;
      js.jsonInt("x", p.rx() );
      js.jsonInt("y", p.ry() );
      mObjectRef.insert( key, js.object() );
      }


    //!
    //! \brief jsonListInt Transfer list of int values
    //! \param key         Key for list
    //! \param list        List to transfer
    //!
    void jsonListInt( const QString &key, const QList<int> &list )
      {
      QJsonArray ar;
      for( auto const &item : list )
        ar.append( item );
      mObjectRef.insert( key, ar );
      }


    //!
    //! \brief jsonListListInt Transfer list of list of int values
    //! \param key             Key for list
    //! \param list            List to transfer
    //!
    void jsonListListInt( const QString &key, const QList< QList<int> > &list )
      {
      QJsonArray ar;
      for( auto const &item : list ) {
        QJsonArray sub;
        for( auto v : item )
          sub.append( v );
        ar.append( sub );
        }
      object().insert( key, ar );
      }


    //!
    //! \brief jsonListInt Transfer list of double values
    //! \param key         Key for list
    //! \param list        List to transfer
    //!
    void jsonListDouble( const QString &key, const QList<double> &list )
      {
      QJsonArray ar;
      for( auto const &item : list )
        ar.append( item );
      mObjectRef.insert( key, ar );
      }


    //!
    //! \brief jsonListString Transfer list of string values
    //! \param key            Key for list
    //! \param list           List to transfer
    //!
    void jsonListString( const QString &key, const QStringList &list )
      {
      QJsonArray ar;
      for( auto const &item : list )
        ar.append( item );
      mObjectRef.insert( key, ar );
      }


    //!
    //! \brief jsonList Template Transfer list of any values
    //!                 Value class must contains json method, which returns
    //!                 json object for value object
    //! \param key      Key for list
    //! \param list     List to transfer
    //!
    template<typename SvClass, typename SvJsonWriterImpl>
    void jsonList( SvJsonWriterImpl &sjs, const QString &key, const QList<SvClass> &list )
      {
      QJsonArray ar;
      for( auto const &item : list ) {
        SvJsonWriterImpl js( sjs );
        item.json( js );
        ar.append( js.object() );
        }
      mObjectRef.insert( key, ar );
      }


    //!
    //! \brief jsonListPtr Template Transfer list of pointers to any value
    //!                    Value class must contains json method, which builds
    //!                    object from json. To build Value object Value class must
    //!                    contains static build( Sv
    //! \param key         Key for list
    //! \param list        List to transfer
    //!
    template<typename SvClass, typename SvJsonWriterImpl>
    void jsonListPtr( SvJsonWriterImpl &sjs, const QString &key, const QList<SvClass*> &list )
      {
      QJsonArray ar;
      for( auto const &item : list ) {
        SvJsonWriterImpl js( sjs );
        item->json( js );
        ar.append( js.object() );
        }
      mObjectRef.insert( key, ar );
      }


    //!
    //! \brief jsonListPtr Template Transfer list of pointers to any value
    //!                    Value class must contains json method, which builds
    //!                    object from json. To build Value object Value class must
    //!                    contains static SvClass *build( SvJsonReader &js )
    //! \param key         Key for list
    //! \param list        List to transfer
    //! \param prefixFun   Prefix function. It call for each item in list and if it return true then
    //!                    item written into json in other case nothing done
    //!
    template<typename SvClass, typename SvJsonWriterImpl, typename SvFunctor>
    void jsonListPtr( SvJsonWriterImpl &sjs, const QString &key, const QList<SvClass*> &list, SvFunctor prefixFun )
      {
      QJsonArray ar;
      for( auto const &item : list ) {
        if( prefixFun(item) ) {
          SvJsonWriterImpl js( sjs );
          item->json( js );
          ar.append( js.object() );
          }
        }
      mObjectRef.insert( key, ar );
      }


    //!
    //! \brief jsonListCrossPtr Template Transfer list of pointers to any value
    //!                         Value class must contains jsonPtr method, which writes
    //!                         only pointer to object without object content
    //! \param key              Key for list
    //! \param list             List to transfer
    //! \param prefixFun        Prefix function. It call for each item in list and if it return true then
    //!                         item written into json in other case nothing done
    //!
    template<typename SvClass, typename SvJsonWriterImpl, typename SvFunctor>
    void jsonListCrossPtr( SvJsonWriterImpl &sjs, const QString &key, const QList<SvClass*> &list, SvFunctor prefixFun )
      {
      QJsonArray ar;
      for( auto const &item : list ) {
        if( prefixFun(item) ) {
          SvJsonWriterImpl js( sjs );
          item->jsonPtr( js );
          ar.append( js.object() );
          }
        }
      mObjectRef.insert( key, ar );
      }


    //!
    //! \brief jsonListCrossPtr Template Transfer list of pointers to any value
    //!                         Value class must contains jsonPtr method, which writes
    //!                         only pointer to object without object content
    //! \param key              Key for list
    //! \param list             List to transfer
    //!
    template<typename SvClass, typename SvJsonWriterImpl>
    void jsonListCrossPtr( SvJsonWriterImpl &sjs, const QString &key, const QList<SvClass*> &list )
      {
      jsonListCrossPtr( sjs, key, list, [] ( const SvClass* ) ->bool { return true; } );
      }



    //!
    //! \brief jsonArray Template Transfer array of same objects. Object class must contains
    //!                  json method
    //! \param sjs       Writer class
    //! \param key       Key for array
    //! \param array     Array of objects
    //! \param count     Count of objects
    //!
    template<typename SvClass, typename SvJsonWriterImpl>
    void jsonArray( SvJsonWriterImpl &sjs, const QString &key, const SvClass *array, int count )
      {
      QJsonArray ar;
      for( int i = 0; i < count; i++ ) {
        SvJsonWriterImpl js( sjs );
        array[i].json( js );
        ar.append( js.object() );
        }
      mObjectRef.insert( key, ar );
      }


    //!
    //! \brief jsonArray Template Transfer array of same objects. Object class must contains
    //!                  json method
    //! \param sjs       Writer class
    //! \param key       Key for array
    //! \param array     Array of objects
    //! \param count     Count of objects. It not saved
    //!
    template<typename SvClass, typename SvJsonWriterImpl>
    void jsonArrayPtr( SvJsonWriterImpl &sjs, const QString &key, const SvClass* const *array, int count )
      {
      QJsonArray ar;
      for( int i = 0; i < count; i++ ) {
        SvJsonWriterImpl js( sjs );
        array[i]->json( js );
        ar.append( js.object() );
        }
      mObjectRef.insert( key, ar );
      }



    //!
    //! \brief jsonMapString Transfer map of strings
    //! \param key           Key for map
    //! \param map           Map to transfer
    //!
    void jsonMapString( const QString &key, const QMap<QString,QString> &map )
      {
      QJsonObject obj;
      for( auto i = map.constBegin(); i != map.constEnd(); i++ )
        obj.insert( i.key(), i.value() );
      mObjectRef.insert( key, obj );
      }


    //!
    //! \brief jsonMapInt Transfer map of int
    //! \param key        Key for map
    //! \param map        Map to transfer
    //!
    void jsonMapInt( const QString &key, const QMap<QString,int> &map )
      {
      QJsonObject obj;
      for( auto i = map.constBegin(); i != map.constEnd(); i++ )
        obj.insert( i.key(), i.value() );
      mObjectRef.insert( key, obj );
      }


    //!
    //! \brief jsonMap Template transfer map of any values with QString as key
    //!                Value class must contains json method, which writes value
    //!                to json object
    //! \param key     Key for map
    //! \param list    Map to transfer
    //!
    template<typename SvClass, typename SvJsonWriterImpl>
    void jsonMap( SvJsonWriterImpl &sjs, const QString &key, const QMap<QString,SvClass> &map )
      {
      QJsonObject obj;
      for( auto i = map.constBegin(); i != map.constEnd(); i++ ) {
        SvJsonWriterImpl js( sjs );
        i.value().json( js );
        obj.insert( i.key(), js.object() );
        }
      mObjectRef.insert( key, obj );
      }



    //!
    //! \brief jsonMapList Template transfer map of list of any values with QString
    //!                    as key of map. Value class must contains json method,
    //!                    which do writing into SvJsonWriter object
    //! \param key         Key for map
    //! \param map         Map of list of object to transfer
    //!
    template<typename SvClass, typename SvJsonWriterImpl>
    void jsonMapList( SvJsonWriterImpl &sjs, const QString &key, const QMap<QString, QList<SvClass> > &map )
      {
      QJsonObject obj;
      for( auto i = map.constBegin(); i != map.constEnd(); i++ ) {
        QJsonArray ar;
        auto list = i.value();
        for( auto const &item : list ) {
          SvJsonWriterImpl js( sjs );
          item.json( js );
          ar.append( js.object() );
          }
        obj.insert( i.key(), ar );
        }
      mObjectRef.insert( key, obj );
      }



    //!
    //! \brief jsonMapPtr Template transfer map of any values with QString as key
    //!                   Value class must contains json method, which builds
    //!                   object from json
    //! \param key        Key for map
    //! \param list       Map to transfer
    //!
    template<typename SvClass, typename SvJsonWriterImpl>
    void jsonMapPtr( SvJsonWriterImpl &sjs, const QString &key, const QMap<QString,SvClass*> &map )
      {
      QJsonObject obj;
      for( auto i = map.constBegin(); i != map.constEnd(); i++ ) {
        SvJsonWriterImpl js( sjs );
        i.value()->json( js );
        obj.insert( i.key(), js.object() );
        }
      mObjectRef.insert( key, obj );
      }


    //!
    //! \brief jsonObject Template transfer any value as json object
    //!                   Value class must contains json method, which returns
    //!                   json object for value object
    //! \param key        Key for object
    //! \param obj        Object to transfer
    //!
    template<typename SvClass, typename SvJsonWriterImpl>
    void jsonObject( SvJsonWriterImpl &sjs, const QString &key, const SvClass &obj )
      {
      SvJsonWriterImpl js( sjs );
      obj.json( js );
      mObjectRef.insert( key, js.object() );
      }


    //!
    //! \brief jsonBuildPtr Template transfer any pointer to object as json object
    //!                     Object class must contains json method, which convert object
    //!                     into json format. When reading this method restore object
    //!                     with buildFromJson function
    //! \param key          Key for object
    //! \param objPtr       Object pointer to transfer
    //!
    template<typename SvClass, typename SvJsonWriterImpl>
    void jsonBuildPtr( SvJsonWriterImpl &sjs, const QString &key, const SvClass *objPtr )
      {
      SvJsonWriterImpl js( sjs );
      objPtr->json( js );
      mObjectRef.insert( key, js.object() );
      }



    //!
    //! \brief jsonCrossPtr Template transfer any pointer to object as json object
    //!                     Object class must contains jsonPtr method, which writes
    //!                     object pointer only without object content
    //! \param key          Key for object
    //! \param objPtr       Object pointer to transfer
    //!
    template<typename SvClass, typename SvJsonWriterImpl>
    void jsonCrossPtr( SvJsonWriterImpl &sjs, const QString &key, const SvClass *objPtr )
      {
      SvJsonWriterImpl js( sjs );
      if( objPtr != nullptr )
        objPtr->jsonPtr( js );
      mObjectRef.insert( key, js.object() );
      }




    //!
    //! \brief jsonLeavePtr Template transfer any pointer to object as json object
    //!                     Object class must contains json method, which convert object
    //!                     into json format. When reading with this method object must
    //!                     be already created
    //! \param key          Key for object
    //! \param objPtr       Object pointer to transfer
    //!
    template<typename SvClass, typename SvJsonWriterImpl>
    void jsonLeavePtr( SvJsonWriterImpl &sjs, const QString &key, const SvClass *objPtr )
      {
      SvJsonWriterImpl js( sjs );
      objPtr->json( js );
      mObjectRef.insert( key, js.object() );
      }



    //!
    //! \brief jsonValue Template transfer any value as json value
    //!                  Value class must contains json method which
    //!                  writes object value into json object
    //! \param key       Key for value
    //! \param val       Object value to transfer
    //!
    template<typename SvClass>
    void jsonValue( const QString &key, const SvClass &val )
      {
      val.json( key, *this );
      }
  };




//!
//! \brief The SvJsonReader class Unificate json io class, through which json readen
//!
class SvJsonReader
  {
  protected:
    const QJsonObject &mObject;    //!< JSON object reference from which will readed
  public:
    //!
    //! \brief SvJsonReader Constructor for reader
    //! \param obj          Object which json readed
    //!
    SvJsonReader( const QJsonObject &obj ) : mObject(obj) {}

    SvJsonReader( const QJsonObject &obj, const SvJsonReader& ) : mObject(obj) {}

    const QJsonObject &object() const { return mObject; }

    //!
    //! \brief contains Return true if key contains in SvJsonWriter object
    //! \param key      Tested key
    //! \return         true if key contains in SvJsonWriter object
    //!
    bool contains( const QString &key ) const { return mObject.contains( key ); }

    //!
    //! \brief jsonBool Transfer bool value
    //! \param key      Key for value
    //! \param b        Bool value
    //! \param def      Default value of param when error while reading occur
    //!
    void jsonBool( const QString &key, bool &b ) const { b = mObject.value( key ).toBool(); }
    void jsonBool( const QString &key, bool &b, bool def ) const { b = mObject.value( key ).toBool( def ); }


    //!
    //! \brief jsonInt Transfer int value
    //! \param key     Key for value
    //! \param v       Int value
    //! \param def      Default value of param when error while reading occur
    //!
    void jsonInt( const QString &key, int &v ) const { v = mObject.value( key ).toInt(); }
    void jsonInt( const QString &key, int &v, int def ) const { v = mObject.value( key ).toInt( def ); }


    //!
    //! \brief jsonHex32 Transfer unsigned as hex string
    //! \param key       Key for value
    //! \param v         Unsigned value
    //!
    void jsonHex32( const QString &key, quint32 &v ) const { v = mObject.value( key ).toString().toUInt( nullptr, 16 ); }
    void jsonHex32( const QString &key, quint32 &v, quint32 def ) const { v = mObject.value( key ).toString(QString::number( def, 16 )).toUInt( nullptr, 16 ); }

    //!
    //! \brief jsonHex64 Transfer unsigned int 64 as hex string
    //! \param key       Key for value
    //! \param v         Unsigned int 64 value
    //!
    void jsonHex64( const QString &key, quint64 &v ) const { v = mObject.value( key ).toString().toULongLong( nullptr, 16 ); }
    void jsonHex64( const QString &key, quint64 &v, quint64 def ) const { v = mObject.value( key ).toString(QString::number( def, 16 )).toLongLong( nullptr, 16 ); }

    //!
    //! \brief jsonInt Transfer color value
    //! \param key     Key for value
    //! \param v       Int value
    //! \param def      Default value of param when error while reading occur
    //!
    void jsonColor( const QString &key, QColor &color ) const { color = QColor( mObject.value( key ).toString() ); }
    void jsonColor( const QString &key, QColor &color, QColor def ) const { color = QColor( mObject.value( key ).toString( def.name() ) ); }


    //!
    //! \brief jsonDouble Transfer double value
    //! \param key        Key for value
    //! \param d          Double value
    //! \param def        Default value of param when error while reading occur
    //!
    void jsonDouble( const QString &key, double &d ) const { d = mObject.value( key ).toDouble(); }
    void jsonDouble( const QString &key, double &d, double def ) const { d = mObject.value( key ).toDouble( def ); }


    //!
    //! \brief jsonString Transfer string value
    //! \param key        Key for value
    //! \param s          String to transfer
    //! \param def        Default value of param when error while reading occur
    //!
    void jsonString( const QString &key, QString &s ) const { s = mObject.value( key ).toString(); }
    void jsonString( const QString &key, QString &s, QString def ) const { s = mObject.value( key ).toString( def ); }


    void jsonHex( const QString &key, QByteArray &ar ) const
      {
      ar = QByteArray::fromHex(mObject.value(key).toString().toLatin1());
      }

    //!
    //! \brief jsonHex Transfer any binary data
    //! \param key     Key for value
    //! \param data    Binary data to transfer
    //! \param size    Size of binary data
    //!
    void jsonHex( const QString &key, char *data, int size ) const
      {
      QByteArray ar;
      jsonHex( key, ar );
      memcpy( data, ar.data(), size );
      }

    void jsonHex( const QString &key, void *data, int size ) const {
      jsonHex( key, (char*)data, size );
      }



    //!
    //! \brief jsonPoint Transfer point value
    //! \param key       Key for value
    //! \param p         Point to transfer
    //!
    void jsonPoint( const QString &key, QPoint &p ) const {
      SvJsonReader js( mObject.value( key ).toObject() );
      js.jsonInt("x", p.rx() );
      js.jsonInt("y", p.ry() );
      }


    //!
    //! \brief jsonListInt Transfer list of int values
    //! \param key         Key for list
    //! \param list        List to transfer
    //!
    void jsonListInt( const QString &key, QList<int> &list ) const
      {
      list.clear();
      QJsonArray ar = mObject.value( key ).toArray();
      list.reserve( ar.count() );
      for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
        int v = i->toInt();
        list.append( v );
        }
      }


    //!
    //! \brief jsonListListInt Transfer list of list of int values
    //! \param key             Key for list
    //! \param list            List to transfer
    //!
    void jsonListListInt( const QString &key, QList< QList<int> > &list ) const
      {
      list.clear();
      QJsonArray ar = mObject.value( key ).toArray();
      list.reserve( ar.count() );
      for( auto sub = ar.constBegin(); sub != ar.constEnd(); sub++ ) {
        QList<int> subList;
        QJsonArray sar = sub->toArray();
        subList.reserve( sar.count() );
        for( auto i = sar.constBegin(); i != sar.constEnd(); i++ ) {
          int v = i->toInt();
          subList.append( v );
          }
        list.append( subList );
        }
      }




    //!
    //! \brief jsonListInt Transfer list of double values
    //! \param key         Key for list
    //! \param list        List to transfer
    //!
    void jsonListDouble( const QString &key, QList<double> &list ) const
      {
      list.clear();
      QJsonArray ar = mObject.value( key ).toArray();
      list.reserve( ar.count() );
      for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
        double v = i->toDouble();
        list.append( v );
        }
      }



    //!
    //! \brief jsonListString Transfer list of string values
    //! \param key            Key for list
    //! \param list           List to transfer
    //!
    void jsonListString( const QString &key, QStringList &list ) const
      {
      list.clear();
      QJsonArray ar = mObject.value( key ).toArray();
      list.reserve( ar.count() );
      for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
        QString v = i->toString();
        list.append( v );
        }
      }


    //!
    //! \brief jsonList Template Transfer list of any values
    //!                 Value class must contains json method, which builds
    //!                 object from json
    //! \param key      Key for list
    //! \param list     List to transfer
    //!
    template<typename SvClass, typename SvJsonReaderImpl>
    void jsonList( const SvJsonReaderImpl &sjs, const QString &key, QList<SvClass> &list ) const
      {
      list.clear();
      QJsonArray ar = mObject.value( key ).toArray();
      list.reserve( ar.count() );
      for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
        SvClass item;
        SvJsonReaderImpl js( i->toObject(), sjs );
        item.json( js );
        list.append( item );
        }
      }


    //!
    //! \brief jsonListPtr Template Transfer list of pointers to any value
    //!                    Value class must contains json method, which builds
    //!                    object from json. To build Value object Value class must
    //!                    contains static buildFromJson( SvJsonReaderImpl &js )
    //! \param key         Key for list
    //! \param list        List to transfer
    //!
    template<typename SvClass, typename SvJsonReaderImpl>
    void jsonListPtr( const SvJsonReaderImpl &sjs, const QString &key, QList<SvClass*> &list ) const
      {
      qDeleteAll(list);
      list.clear();
      QJsonArray ar = mObject.value( key ).toArray();
      list.reserve( ar.count() );
      for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
        SvJsonReaderImpl js( i->toObject(), sjs );
        SvClass *item = dynamic_cast<SvClass*>(SvClass::buildFromJson( js ));
        if( item != nullptr )
          list.append( item );
        }
      }



    //!
    //! \brief jsonListPtr Template Transfer list of pointers to any value
    //!                    Value class must contains json method, which builds
    //!                    object from json. To build Value object Value class must
    //!                    contains static buildFromJson( SvJsonReaderImpl &js )
    //! \param key         Key for list
    //! \param list        List to transfer
    //!
    template<typename SvClass, typename SvJsonReaderImpl, typename SvFunctor>
    void jsonListPtr( const SvJsonReaderImpl &sjs, const QString &key, QList<SvClass*> &list, SvFunctor postfixFun ) const
      {
      qDeleteAll(list);
      list.clear();
      QJsonArray ar = mObject.value( key ).toArray();
      list.reserve( ar.count() );
      for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
        SvJsonReaderImpl js( i->toObject(), sjs );
        SvClass *item = SvClass::buildFromJson( js );
        if( postfixFun( item ) )
          list.append( item );
        }
      }



    //!
    //! \brief jsonListCrossPtr Template Transfer list of pointers to any value
    //!                         To build Value object Value class must
    //!                         contains static buildPtrFromJson( SvJsonReaderImpl &js )
    //! \param key              Key for list
    //! \param list             List to transfer
    //! \param prefixFun        Prefix function. It call for each item in list and if it return true then
    //!                         item written into json in other case nothing done
    //!
    template<typename SvClass, typename SvJsonReaderImpl, typename SvFunctor>
    void jsonListCrossPtr( SvJsonReaderImpl &sjs, const QString &key, QList<SvClass*> &list, SvFunctor postfixFun ) const
      {
      qDeleteAll(list);
      list.clear();
      QJsonArray ar = mObject.value( key ).toArray();
      list.reserve( ar.count() );
      for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
        SvJsonReaderImpl js( i->toObject(), sjs );
        SvClass *item = SvClass::buildPtrFromJson( js );
        if( postfixFun( item ) )
          list.append( item );
        }
      }


    //!
    //! \brief jsonListCrossPtr Template Transfer list of pointers to any value
    //!                         To build Value object Value class must
    //!                         contains static buildPtrFromJson( SvJsonReaderImpl &js )
    //! \param key              Key for list
    //! \param list             List to transfer
    //!
    template<typename SvClass, typename SvJsonReaderImpl>
    void jsonListCrossPtr( SvJsonReaderImpl &sjs, const QString &key, QList<SvClass*> &list ) const
      {
      jsonListCrossPtr( sjs, key, list, [] ( SvClass *item ) -> bool { return item != nullptr; } );
      }



    //!
    //! \brief jsonArray Template Transfer array of same objects. Object class must contains
    //!                  json method
    //! \param sjs       Reader class
    //! \param key       Key for array
    //! \param array     Array of objects
    //! \param count     Count of objects. It not saved
    //!
    template<typename SvClass, typename SvJsonReaderImpl>
    void jsonArray( SvJsonReaderImpl &sjs, const QString &key, SvClass *array, int count ) const
      {
      int k = 0;
      QJsonArray ar = mObject.value( key ).toArray();
      for( auto i = ar.constBegin(); i != ar.constEnd() && k < count; i++ ) {
        SvJsonReaderImpl js( i->toObject(), sjs );
        array[k].json( js );
        k++;
        }
      }


    //!
    //! \brief jsonArray Template Transfer array of pointer to objects. Object class must contains
    //!                  json method and static buildFromJson method
    //! \param sjs       Reader class
    //! \param key       Key for array
    //! \param array     Array of pointer to objects
    //! \param count     Count of pointer to objects. On input it contains max count of objects in array at output - fact count of objects
    //!
    template<typename SvClass, typename SvJsonReaderImpl>
    void jsonArrayPtr( SvJsonReaderImpl &sjs, const QString &key, SvClass **array, int &count ) const
      {
      int k = 0;
      QJsonArray ar = mObject.value( key ).toArray();
      for( auto i = ar.constBegin(); i != ar.constEnd() && k < count; i++ ) {
        SvJsonReaderImpl js( i->toObject(), sjs );
        array[k] = dynamic_cast<SvClass*>( SvClass::buildFromJson( js ) );
        k++;
        }
      count = k;
      }





    //!
    //! \brief jsonMapString Transfer map of strings
    //! \param key           Key for map
    //! \param map           Map to transfer
    //!
    void jsonMapString( const QString &key, QMap<QString,QString> &map ) const
      {
      map.clear();
      QJsonObject obj = mObject.value( key ).toObject();
      for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
        QString str = i.value().toString();
        map.insert( i.key(), str );
        }
      }



    //!
    //! \brief jsonMapInt Transfer map of int
    //! \param key        Key for map
    //! \param map        Map to transfer
    //!
    void jsonMapInt( const QString &key, QMap<QString,int> &map ) const
      {
      map.clear();
      QJsonObject obj = mObject.value( key ).toObject();
      for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
        int v = i.value().toInt();
        map.insert( i.key(), v );
        }
      }


    //!
    //! \brief jsonMap Template transfer map of any values with QString as key
    //!                Value class must contains json method, which builds
    //!                object from json
    //! \param key     Key for map
    //! \param list    Map to transfer
    //!
    template<typename SvClass, typename SvJsonReaderImpl>
    void jsonMap( const SvJsonReaderImpl &sjs, const QString &key, QMap<QString,SvClass> &map ) const
      {
      map.clear();
      QJsonObject obj = mObject.value( key ).toObject();
      for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
        SvClass item;
        SvJsonReaderImpl js( i.value().toObject(), sjs );
        item.json( js );
        map.insert( i.key(), item );
        }
      }




    //!
    //! \brief jsonMapList Template transfer map of list of any values with QString
    //!                    as key of map. Value class must contains json method,
    //!                    which do writing into SvJsonWriter object
    //! \param key         Key for map
    //! \param map         Map of list of object to transfer
    //!
    template<typename SvClass, typename SvJsonReaderImpl>
    void jsonMapList( const SvJsonReaderImpl &sjs, const QString &key, const QMap<QString, QList<SvClass> > &map ) const
      {
      map.clear();
      QJsonObject obj = mObject.value( key ).toObject();
      for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
        QJsonArray ar = i.value().toArray();
        QList<SvClass> list;
        list.reserve( ar.count() );
        for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
          SvClass item;
          SvJsonReaderImpl js( i->toObject(), sjs );
          item.json( js );
          list.append( item );
          }
        map.insert( i.key(), list );
        }
      }



    //!
    //! \brief jsonMapPtr Template transfer map of any values with QString as key
    //!                   Value class must contains json method, which builds
    //!                   object from json
    //! \param key        Key for map
    //! \param list       Map to transfer
    //!
    template<typename SvClass, typename SvJsonReaderImpl>
    void jsonMapPtr( const SvJsonReaderImpl &sjs, const QString &key, QMap<QString,SvClass*> &map ) const
      {
      qDeleteAll(map);
      map.clear();
      QJsonObject obj = mObject.value( key ).toObject();
      for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
        SvJsonReaderImpl js( i.value().toObject(), sjs );
        SvClass *item = SvClass::buildFromJson( js );
        if( item )
          map.insert( i.key(), item );
        }
      }


    //!
    //! \brief jsonObject Template transfer any value as json object
    //!                   Value class must contains json method, which builds
    //!                   object from json
    //! \param key        Key for object
    //! \param obj        Object to transfer
    //!
    template<typename SvClass, typename SvJsonReaderImpl>
    void jsonObject( const SvJsonReaderImpl &sjs, const QString &key, SvClass &obj ) const
      {
      SvJsonReaderImpl js( mObject.value( key ).toObject(), sjs );
      obj.json( js );
      }


    //!
    //! \brief jsonBuildPtr Template transfer any pointer to object as json object
    //!                     Object class must contains json method, which convert object
    //!                     into json format. When reading this method restore object
    //!                     with buildFromJson function
    //! \param key          Key for object
    //! \param objPtr       Object pointer to transfer
    //!
    template<typename SvClass, typename SvJsonReaderImpl>
    void jsonBuildPtr( const SvJsonReaderImpl &sjs, const QString &key, SvClass* &objPtr ) const
      {
      SvJsonReaderImpl js( mObject.value( key ).toObject(), sjs );
      objPtr = dynamic_cast<SvClass*>( SvClass::buildFromJson( js ) );
      }



    //!
    //! \brief jsonCrossPtr Template transfer any pointer to object as json object
    //!                     Object class must contains jsonPtr method, which writes
    //!                     object pointer only without object content
    //! \param key          Key for object
    //! \param objPtr       Object pointer to transfer
    //!
    template<typename SvClass, typename SvJsonReaderImpl>
    void jsonCrossPtr( const SvJsonReaderImpl &sjs, const QString &key, SvClass* &objPtr ) const
      {
      SvJsonReaderImpl js( mObject.value( key ).toObject(), sjs );
      objPtr = dynamic_cast<SvClass*>( SvClass::buildPtrFromJson( js ) );
      }




    //!
    //! \brief jsonLeavePtr Template transfer any pointer to object as json object
    //!                     Object class must contains json method, which convert object
    //!                     into json format. When reading with this method object must
    //!                     be already created
    //! \param key          Key for object
    //! \param objPtr       Object pointer to transfer
    //!
    template<typename SvClass, typename SvJsonReaderImpl>
    void jsonLeavePtr( const SvJsonReaderImpl &sjs, const QString &key, SvClass *objPtr ) const
      {
      QJsonObject obj = mObject.value( key ).toObject();
      SvJsonReaderImpl js( obj, sjs );
      objPtr->json( js );
      }




    //!
    //! \brief jsonValue Template transfer any value as json value
    //!                  Value class must contains json method which
    //!                  reads object value from json object
    //! \param key       Key for value
    //! \param val       Object value to transfer
    //!
    template<typename SvClass>
    void jsonValue( const QString &key, SvClass &val )
      {
      val.json( key, *this );
      }
  };






//!
//! \brief The SvJsonReaderExt template used when need translate some common param (property) to
//!                            complex reader, for example dataBase version
//!
template<typename SvProperty>
class SvJsonReaderExt : public SvJsonReader
  {
    SvProperty        *mProperty;  //!< Reader property - special object to build full structure
  public:
    //!
    //! \brief SvJsonReader Constructor for reader
    //! \param obj          Object which json readed
    //!
    SvJsonReaderExt( const QJsonObject &obj, SvProperty *prop ) : SvJsonReader(obj), mProperty(prop) {}

    SvJsonReaderExt( const QJsonObject &obj, const SvJsonReaderExt<SvProperty> &src ) : SvJsonReader(obj), mProperty(src.mProperty) {}

    //!
    //! \brief property Returns current property of reader
    //! \return         Property of reader
    //!
    SvProperty *property() const { return mProperty; }

  };



//!
//! \brief The SvJsonReaderExtInt class Unificate json io class, through which json readed
//!
using SvJsonReaderExtInt = SvJsonReaderExt<int>;




template <typename SvObject>
class SvObjectMap
  {
    using SvObjectPtr = SvObject*;
    QMap<QString,SvObjectPtr> mObjectMap; //!< Map obj objects
  public:
    SvObjectMap() {}

    //!
    //! \brief contains Returns true if object with key contains in map
    //! \param key      Key for test
    //! \return         true if object with key contains in map
    //!
    bool      contains( const QString &key ) const { return mObjectMap.contains(key); }

    //!
    //! \brief object Returns object with key
    //! \param key    key of object
    //! \return       object with key
    //!
    SvObject *object( const QString &key ) const { return mObjectMap.value( key, nullptr ); }

    //!
    //! \brief insert Insert new object in map
    //! \param key    Key of object
    //! \param obj    Object
    //!
    void      insert( const QString &key, SvObject *obj ) { mObjectMap.insert( key, obj ); }

  };



inline QByteArray svJsonObjectToByteArray( const QJsonObject &obj, QJsonDocument::JsonFormat format = QJsonDocument::Indented )
  {
  return QJsonDocument(obj).toJson( format );
  }


inline QJsonObject svJsonObjectFromByteArray( const QByteArray &ar )
  {
  return QJsonDocument::fromJson(ar).object();
  }


//!
//! \brief toCbor Convert json object to cbor binary format
//! \return       CBOR binary format
//!
inline QByteArray svJsonObjectToCbor( const QJsonObject &obj )
  {
  return QCborValue::fromJsonValue( QJsonValue( obj ) ).toCbor();
  }


//!
//! \brief svJsonObjectFromCbor Convert from cbor binary format to json object
//! \param ar                   CBOR binary format
//! \return                     JSON object
//!
inline QJsonObject svJsonObjectFromCbor( const QByteArray &ar )
  {
  return QCborValue::fromCbor( ar ).toJsonValue().toObject();
  }





#endif // SVJSONIO_H
