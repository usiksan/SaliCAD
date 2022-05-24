/*
  Project "Improving the visibility of json io"
  Author
    Alexander Sibilev
  Description
    There are two classes here: writer and reader. Signatures of calling read and write functions in
    both classes are completely identical, so calls can be copied to both the write and read functions.
  www
    www.salilab.com
  History
    05.02.2022 v1 Begin version support
    05.03.2022 v2 Append QPoint support
    06.04.2022 v3 Append value of any class support which must be have jsonWrite and jsonRead members
    21.04.2022 v4 Append map of list of any class support
    26.04.2022 v5 Full English remark
    24.05.2022 v6 Replace all const char* on QString which support both using with const char* and QString
*/
#ifndef SVJSONIO_H
#define SVJSONIO_H

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QColor>
#include <QMap>
#include <QPoint>

#define SV_JSON_VERSION 6

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
    //!                 Value class must contains jsonWrite method, which returns
    //!                 json object for value object
    //! \param key      Key for list
    //! \param list     List to transfer
    //!
    template<typename SvClass>
    void jsonList( const QString &key, const QList<SvClass> &list )
      {
      QJsonArray ar;
      for( auto const &item : list ) {
        SvJsonWriter js;
        item.jsonWrite( js );
        ar.append( js.object() );
        }
      mObjectRef.insert( key, ar );
      }


    //!
    //! \brief jsonListPtr Template Transfer list of pointers to any value
    //!                    Value class must contains jsonRead method, which builds
    //!                    object from json. To build Value object Value class must
    //!                    contains static build( Sv
    //! \param key         Key for list
    //! \param list        List to transfer
    //!
    template<typename SvClass>
    void jsonListPtr( const QString &key, const QList<SvClass*> &list )
      {
      QJsonArray ar;
      for( auto const &item : list ) {
        SvJsonWriter js;
        item->jsonWrite( js );
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
    //!                Value class must contains jsonWrite method, which returns
    //!                json object for value object
    //! \param key     Key for map
    //! \param list    Map to transfer
    //!
    template<typename SvClass>
    void jsonMap( const QString &key, const QMap<QString,SvClass> &map )
      {
      QJsonObject obj;
      for( auto i = map.constBegin(); i != map.constEnd(); i++ ) {
        SvJsonWriter js;
        i.value().jsonWrite( js );
        obj.insert( i.key(), js.object() );
        }
      mObjectRef.insert( key, obj );
      }



    //!
    //! \brief jsonMapList Template transfer map of list of any values with QString
    //!                    as key of map. Value class must contains jsonWrite method,
    //!                    which do writing into SvJsonWriter object
    //! \param key         Key for map
    //! \param map         Map of list of object to transfer
    //!
    template <typename SvClass>
    void jsonMapList( const QString &key, const QMap<QString, QList<SvClass> > &map )
      {
      QJsonObject obj;
      for( auto i = map.constBegin(); i != map.constEnd(); i++ ) {
        QJsonArray ar;
        auto list = i.value();
        for( auto const &item : list ) {
          SvJsonWriter js;
          item.jsonWrite( js );
          ar.append( js.object() );
          }
        obj.insert( i.key(), ar );
        }
      mObjectRef.insert( key, obj );
      }



    //!
    //! \brief jsonMapPtr Template transfer map of any values with QString as key
    //!                   Value class must contains jsonRead method, which builds
    //!                   object from json
    //! \param key        Key for map
    //! \param list       Map to transfer
    //!
    template<typename SvClass>
    void jsonMapPtr( const QString &key, const QMap<QString,SvClass*> &map )
      {
      QJsonObject obj;
      for( auto i = map.constBegin(); i != map.constEnd(); i++ ) {
        SvJsonWriter js;
        i.value()->jsonWrite( js );
        obj.insert( i.key(), js.object() );
        }
      mObjectRef.insert( key, obj );
      }


    //!
    //! \brief jsonObject Template transfer any value as json object
    //!                   Value class must contains jsonWrite method, which returns
    //!                   json object for value object
    //! \param key        Key for object
    //! \param obj        Object to transfer
    //!
    template<typename SvClass>
    void jsonObject( const QString &key, const SvClass &obj )
      {
      SvJsonWriter js;
      obj.jsonWrite( js );
      mObjectRef.insert( key, js.object() );
      }


    //!
    //! \brief jsonObjectPtr Template transfer any value as json object
    //!                      Value class must contains jsonWrite method, which returns
    //!                      json object for value object
    //! \param key           Key for object
    //! \param objPtr        Object pointer to transfer
    //!
    template<typename SvClass>
    void jsonObjectPtr( const QString &key, const SvClass *objPtr )
      {
      SvJsonWriter js;
      objPtr->jsonWrite( js );
      mObjectRef.insert( key, js.object() );
      }


    //!
    //! \brief jsonValue Template transfer any value as json value
    //!                  Value class must contains jsonWrite method which
    //!                  writes object value into json object
    //! \param key       Key for value
    //! \param val       Object value to transfer
    //!
    template<typename SvClass>
    void jsonValue( const QString &key, const SvClass &val )
      {
      val.jsonWrite( key, *this );
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

    const QJsonObject &object() const { return mObject; }

    //!
    //! \brief jsonBool Transfer bool value
    //! \param key      Key for value
    //! \param b        Bool value
    //! \param def      Default value of param when error while reading occur
    //!
    void jsonBool( const QString &key, bool &b ) { b = mObject.value( key ).toBool(); }
    void jsonBool( const QString &key, bool &b, bool def ) { b = mObject.value( key ).toBool( def ); }


    //!
    //! \brief jsonInt Transfer int value
    //! \param key     Key for value
    //! \param v       Int value
    //! \param def      Default value of param when error while reading occur
    //!
    void jsonInt( const QString &key, int &v ) { v = mObject.value( key ).toInt(); }
    void jsonInt( const QString &key, int &v, int def ) { v = mObject.value( key ).toInt( def ); }


    //!
    //! \brief jsonInt Transfer color value
    //! \param key     Key for value
    //! \param v       Int value
    //! \param def      Default value of param when error while reading occur
    //!
    void jsonColor( const QString &key, QColor &color ) { color = QColor( mObject.value( key ).toString() ); }
    void jsonColor( const QString &key, QColor &color, QColor def ) { color = QColor( mObject.value( key ).toString( def.name() ) ); }


    //!
    //! \brief jsonDouble Transfer double value
    //! \param key        Key for value
    //! \param d          Double value
    //! \param def        Default value of param when error while reading occur
    //!
    void jsonDouble( const QString &key, double &d ) { d = mObject.value( key ).toDouble(); }
    void jsonDouble( const QString &key, double &d, double def ) { d = mObject.value( key ).toDouble( def ); }


    //!
    //! \brief jsonString Transfer string value
    //! \param key        Key for value
    //! \param s          String to transfer
    //! \param def        Default value of param when error while reading occur
    //!
    void jsonString( const QString &key, QString &s ) { s = mObject.value( key ).toString(); }
    void jsonString( const QString &key, QString &s, QString def ) { s = mObject.value( key ).toString( def ); }


    //!
    //! \brief jsonPoint Transfer point value
    //! \param key       Key for value
    //! \param p         Point to transfer
    //!
    void jsonPoint( const QString &key, QPoint &p ) {
      SvJsonReader js( mObject.value( key ).toObject() );
      js.jsonInt("x", p.rx() );
      js.jsonInt("y", p.ry() );
      }


    //!
    //! \brief jsonListInt Transfer list of int values
    //! \param key         Key for list
    //! \param list        List to transfer
    //!
    void jsonListInt( const QString &key, QList<int> &list )
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
    //! \brief jsonListInt Transfer list of double values
    //! \param key         Key for list
    //! \param list        List to transfer
    //!
    void jsonListDouble( const QString &key, QList<double> &list )
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
    void jsonListString( const QString &key, QStringList &list )
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
    //!                 Value class must contains jsonRead method, which builds
    //!                 object from json
    //! \param key      Key for list
    //! \param list     List to transfer
    //!
    template<typename SvClass>
    void jsonList( const QString &key, QList<SvClass> &list )
      {
      list.clear();
      QJsonArray ar = mObject.value( key ).toArray();
      list.reserve( ar.count() );
      for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
        SvClass item;
        SvJsonReader js( i->toObject() );
        item.jsonRead( js );
        list.append( item );
        }
      }


    //!
    //! \brief jsonListPtr Template Transfer list of pointers to any value
    //!                    Value class must contains jsonRead method, which builds
    //!                    object from json. To build Value object Value class must
    //!                    contains static build( Sv
    //! \param key         Key for list
    //! \param list        List to transfer
    //!
    template<typename SvClass>
    void jsonListPtr( const QString &key, QList<SvClass*> &list )
      {
      qDeleteAll(list);
      list.clear();
      QJsonArray ar = mObject.value( key ).toArray();
      list.reserve( ar.count() );
      for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
        SvJsonReader js( i->toObject() );
        SvClass *item = SvClass::build( js );
        if( item ) {
          item->jsonRead( js );
          list.append( item );
          }
        }
      }




    //!
    //! \brief jsonMapString Transfer map of strings
    //! \param key           Key for map
    //! \param map           Map to transfer
    //!
    void jsonMapString( const QString &key, QMap<QString,QString> &map )
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
    void jsonMapInt( const QString &key, QMap<QString,int> &map )
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
    //!                Value class must contains jsonRead method, which builds
    //!                object from json
    //! \param key     Key for map
    //! \param list    Map to transfer
    //!
    template<typename SvClass>
    void jsonMap( const QString &key, QMap<QString,SvClass> &map )
      {
      map.clear();
      QJsonObject obj = mObject.value( key ).toObject();
      for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
        SvClass item;
        SvJsonReader js( i.value().toObject() );
        item.jsonRead( js );
        map.insert( i.key(), item );
        }
      }




    //!
    //! \brief jsonMapList Template transfer map of list of any values with QString
    //!                    as key of map. Value class must contains jsonWrite method,
    //!                    which do writing into SvJsonWriter object
    //! \param key         Key for map
    //! \param map         Map of list of object to transfer
    //!
    template <typename SvClass>
    void jsonMapList( const QString &key, const QMap<QString, QList<SvClass> > &map )
      {
      map.clear();
      QJsonObject obj = mObject.value( key ).toObject();
      for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
        QJsonArray ar = i.value().toArray();
        QList<SvClass> list;
        list.reserve( ar.count() );
        for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
          SvClass item;
          SvJsonReader js( i->toObject() );
          item.jsonRead( js );
          list.append( item );
          }
        map.insert( i.key(), list );
        }
      }



    //!
    //! \brief jsonMapPtr Template transfer map of any values with QString as key
    //!                   Value class must contains jsonRead method, which builds
    //!                   object from json
    //! \param key        Key for map
    //! \param list       Map to transfer
    //!
    template<typename SvClass>
    void jsonMapPtr( const QString &key, QMap<QString,SvClass*> &map )
      {
      qDeleteAll(map);
      map.clear();
      QJsonObject obj = mObject.value( key ).toObject();
      for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
        SvJsonReader js( i.value().toObject() );
        SvClass *item = SvClass::build( js );
        if( item ) {
          item->jsonRead( js );
          map.insert( i.key(), item );
          }
        }
      }


    //!
    //! \brief jsonObject Template transfer any value as json object
    //!                   Value class must contains jsonRead method, which builds
    //!                   object from json
    //! \param key        Key for object
    //! \param obj        Object to transfer
    //!
    template<typename SvClass>
    void jsonObject( const QString &key, SvClass &obj )
      {
      SvJsonReader js( mObject.value( key ).toObject() );
      obj.jsonRead( js );
      }


    //!
    //! \brief jsonObjectPtr Template transfer any value as json object
    //!                      Value class must contains jsonRead method, which builds
    //!                      object from json
    //! \param key           Key for object
    //! \param objPtr        Object pointer to transfer
    //!
    template<typename SvClass>
    void jsonObjectPtr( const QString &key, SvClass *objPtr )
      {
      SvJsonReader js( mObject.value( key ).toObject() );
      objPtr->jsonRead( js );
      }


    //!
    //! \brief jsonValue Template transfer any value as json value
    //!                  Value class must contains jsonRead method which
    //!                  reads object value from json object
    //! \param key       Key for value
    //! \param val       Object value to transfer
    //!
    template<typename SvClass>
    void jsonValue( const QString &key, SvClass &val )
      {
      val.jsonRead( key, *this );
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

    //!
    //! \brief property Returns current property of reader
    //! \return         Property of reader
    //!
    SvProperty *property() { return mProperty; }

    //!
    //! \brief jsonList Template Transfer list of any values
    //!                 Value class must contains jsonRead method, which builds
    //!                 object from json
    //! \param key      Key for list
    //! \param list     List to transfer
    //!
    template<typename SvClass>
    void jsonList( const QString &key, QList<SvClass> &list )
      {
      list.clear();
      QJsonArray ar = mObject.value( key ).toArray();
      for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
        SvClass item;
        SvJsonReaderExt<SvProperty> js( i->toObject(), mProperty );
        item.jsonRead( js );
        list.append( item );
        }
      }


    //!
    //! \brief jsonListPtr Template Transfer list of pointers to any value
    //!                    Value class must contains jsonRead method, which builds
    //!                    object from json. To build Value object Value class must
    //!                    contains static build( Sv
    //! \param key         Key for list
    //! \param list        List to transfer
    //!
    template<typename SvClass>
    void jsonListPtr( const QString &key, QList<SvClass*> &list )
      {
      qDeleteAll(list);
      list.clear();
      QJsonArray ar = mObject.value( key ).toArray();
      for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
        SvJsonReaderExt<SvProperty> js( i->toObject(), mProperty );
        SvClass *item = SvClass::build( js );
        if( item ) {
          item->jsonRead( js );
          list.append( item );
          }
        }
      }


    //!
    //! \brief jsonMap Template transfer map of any values with QString as key
    //!                Value class must contains jsonRead method, which builds
    //!                object from json
    //! \param key     Key for map
    //! \param list    Map to transfer
    //!
    template<typename SvClass>
    void jsonMap( const QString &key, QMap<QString,SvClass> &map )
      {
      map.clear();
      QJsonObject obj = mObject.value( key ).toObject();
      for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
        SvClass item;
        SvJsonReaderExt<SvProperty> js( i.value().toObject(), mProperty );
        item.jsonRead( js );
        map.insert( i.key(), item );
        }
      }




    //!
    //! \brief jsonMapList Template transfer map of list of any values with QString
    //!                    as key of map. Value class must contains jsonWrite method,
    //!                    which do writing into SvJsonWriter object
    //! \param key         Key for map
    //! \param map         Map of list of object to transfer
    //!
    template <typename SvClass>
    void jsonMapList( const QString &key, QMap<QString, QList<SvClass> > &map )
      {
      map.clear();
      QJsonObject obj = mObject.value( key ).toObject();
      for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
        QJsonArray ar = i.value().toArray();
        QList<SvClass> list;
        list.reserve( ar.count() );
        for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
          SvClass item;
          SvJsonReaderExt<SvProperty> js( i->toObject(), mProperty );
          item.jsonRead( js );
          list.append( item );
          }
        map.insert( i.key(), list );
        }
      }


    //!
    //! \brief jsonMapPtr Template transfer map of any values with QString as key
    //!                   Value class must contains jsonRead method, which builds
    //!                   object from json
    //! \param key        Key for map
    //! \param list       Map to transfer
    //!
    template<typename SvClass>
    void jsonMapPtr( const QString &key, QMap<QString,SvClass*> &map )
      {
      qDeleteAll(map);
      map.clear();
      QJsonObject obj = mObject.value( key ).toObject();
      for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
        SvJsonReaderExt<SvProperty> js( i.value().toObject(), mProperty );
        SvClass *item = SvClass::build( js );
        if( item ) {
          item->jsonRead( js );
          map.insert( i.key(), item );
          }
        }
      }


    //!
    //! \brief jsonObject Template transfer any value as json object
    //!                   Value class must contains jsonRead method, which builds
    //!                   object from json
    //! \param key        Key for object
    //! \param obj        Object to transfer
    //!
    template<typename SvClass>
    void jsonObject( const QString &key, SvClass &obj )
      {
      SvJsonReaderExt<SvProperty> js( mObject.value( key ).toObject(), mProperty );
      obj.jsonRead( js );
      }


    //!
    //! \brief jsonObjectPtr Template transfer any value as json object
    //!                      Value class must contains jsonRead method, which builds
    //!                      object from json
    //! \param key           Key for object
    //! \param objPtr        Object pointer to transfer
    //!
    template<typename SvClass>
    void jsonObjectPtr( const QString &key, SvClass *objPtr )
      {
      SvJsonReaderExt<SvProperty> js( mObject.value( key ).toObject(), mProperty );
      objPtr->jsonRead( js );
      }


    //!
    //! \brief jsonValue Template transfer any value as json value
    //!                  Value class must contains jsonRead method which
    //!                  reads object value from json object
    //! \param key       Key for value
    //! \param val       Object value to transfer
    //!
    template<typename SvClass>
    void jsonValue( const QString &key, SvClass &val )
      {
      val.jsonRead( key, *this );
      }
  };



//!
//! \brief The SvJsonReaderExtInt class Unificate json io class, through which json readed
//!
using SvJsonReaderExtInt = SvJsonReaderExt<int>;


inline QByteArray svJsonObjectToByteArray( const QJsonObject &obj )
  {
  return QJsonDocument(obj).toJson();
  }


inline QJsonObject svJsonObjectFromByteArray( const QByteArray &ar )
  {
  return QJsonDocument::fromJson(ar).object();
  }

#endif // SVJSONIO_H
