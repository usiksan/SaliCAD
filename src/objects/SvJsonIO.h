/*
  Проект     "Повышение наглядности ввода-вывода json"
  Автор
    Сибилев А.С.
  Описание
    Здесь два класса: писатель и читатель. Сигнатуры вызова функций чтения и записи в
    обоих классах полностью идентичны, поэтому вызовы могут быть скопированы и в функцию
    записи и чтения
  History
    05.02.2022 v1 Begin version support
*/
#ifndef SVJSONIO_H
#define SVJSONIO_H

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QColor>
#include <QMap>

#define SV_JSON_VERSION 1

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
    QJsonObject object() { return mObjectRef; }

    QJsonObject &ref() { return mObjectRef; }


    //!
    //! \brief jsonBool Transfer bool value
    //! \param key      Key for value
    //! \param b        Bool value
    //!
    void jsonBool( const char *key, bool b ) { mObjectRef.insert( QString(key), b ); }
    void jsonBool( const char *key, bool b, bool ) { mObjectRef.insert( QString(key), b ); }


    //!
    //! \brief jsonInt Transfer int value
    //! \param key     Key for value
    //! \param v       Int value
    //!
    void jsonInt( const char *key, int v ) { mObjectRef.insert( QString(key), v ); }
    void jsonInt( const char *key, int v, int ) { mObjectRef.insert( QString(key), v ); }


    //!
    //! \brief jsonInt Transfer color value
    //! \param key     Key for value
    //! \param v       Int value
    //!
    void jsonColor( const char *key, QColor color ) { mObjectRef.insert( QString(key), color.name() ); }
    void jsonColor( const char *key, QColor color, QColor ) { mObjectRef.insert( QString(key), color.name() ); }


    //!
    //! \brief jsonDouble Transfer double value
    //! \param key        Key for value
    //! \param d          Double value
    //!
    void jsonDouble( const char *key, double d ) { mObjectRef.insert( QString(key), d ); }
    void jsonDouble( const char *key, double d, double ) { mObjectRef.insert( QString(key), d ); }


    //!
    //! \brief jsonString Transfer string value
    //! \param key        Key for value
    //! \param s          String to transfer
    //!
    void jsonString( const char *key, const QString &s ) { mObjectRef.insert( QString(key), s ); }
    void jsonString( const char *key, const QString &s, QString ) { mObjectRef.insert( QString(key), s ); }


    //!
    //! \brief jsonListInt Transfer list of int values
    //! \param key         Key for list
    //! \param list        List to transfer
    //!
    void jsonListInt( const char *key, const QList<int> &list )
      {
      QJsonArray ar;
      for( auto const &item : list )
        ar.append( item );
      mObjectRef.insert( QString(key), ar );
      }


    //!
    //! \brief jsonListString Transfer list of string values
    //! \param key            Key for list
    //! \param list           List to transfer
    //!
    void jsonListString( const char *key, const QStringList &list )
      {
      QJsonArray ar;
      for( auto const &item : list )
        ar.append( item );
      mObjectRef.insert( QString(key), ar );
      }


    //!
    //! \brief jsonList Template Transfer list of any values
    //!                 Value class must contains jsonWrite method, which returns
    //!                 json object for value object
    //! \param key      Key for list
    //! \param list     List to transfer
    //!
    template<typename SvClass>
    void jsonList( const char *key, const QList<SvClass> &list )
      {
      QJsonArray ar;
      for( auto const &item : list ) {
        SvJsonWriter js;
        item.jsonWrite( js );
        ar.append( js.object() );
        }
      mObjectRef.insert( QString(key), ar );
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
    void jsonListPtr( const char *key, const QList<SvClass*> &list )
      {
      QJsonArray ar;
      for( auto const &item : list ) {
        SvJsonWriter js;
        item->jsonWrite( js );
        ar.append( js.object() );
        }
      mObjectRef.insert( QString(key), ar );
      }


    //!
    //! \brief jsonMapString Transfer map of strings
    //! \param key           Key for map
    //! \param map           Map to transfer
    //!
    void jsonMapString( const char *key, const QMap<QString,QString> &map )
      {
      QJsonObject obj;
      for( auto i = map.constBegin(); i != map.constEnd(); i++ )
        obj.insert( i.key(), i.value() );
      mObjectRef.insert( QString(key), obj );
      }


    //!
    //! \brief jsonMapInt Transfer map of int
    //! \param key        Key for map
    //! \param map        Map to transfer
    //!
    void jsonMapInt( const char *key, const QMap<QString,int> &map )
      {
      QJsonObject obj;
      for( auto i = map.constBegin(); i != map.constEnd(); i++ )
        obj.insert( i.key(), i.value() );
      mObjectRef.insert( QString(key), obj );
      }


    //!
    //! \brief jsonMap Template transfer map of any values with QString as key
    //!                Value class must contains jsonWrite method, which returns
    //!                json object for value object
    //! \param key     Key for map
    //! \param list    Map to transfer
    //!
    template<typename SvClass>
    void jsonMap( const char *key, const QMap<QString,SvClass> &map )
      {
      QJsonObject obj;
      for( auto i = map.constBegin(); i != map.constEnd(); i++ ) {
        SvJsonWriter js;
        i.value().jsonWrite( js );
        obj.insert( i.key(), js.object() );
        }
      mObjectRef.insert( QString(key), obj );
      }


    //!
    //! \brief jsonMap Template transfer map of any values with QString as key
    //!                Value class must contains jsonRead method, which builds
    //!                object from json
    //! \param key     Key for map
    //! \param list    Map to transfer
    //!
    template<typename SvClass>
    void jsonMapPtr( const char *key, const QMap<QString,SvClass*> &map )
      {
      QJsonObject obj;
      for( auto i = map.constBegin(); i != map.constEnd(); i++ ) {
        SvJsonWriter js;
        i.value()->jsonWrite( js );
        obj.insert( i.key(), js.object() );
        }
      mObjectRef.insert( QString(key), obj );
      }


    //!
    //! \brief jsonObject Template transfer any value as json object
    //!                   Value class must contains jsonWrite method, which returns
    //!                   json object for value object
    //! \param key        Key for object
    //! \param obj        Object to transfer
    //!
    template<typename SvClass>
    void jsonObject( const char *key, const SvClass &obj )
      {
      SvJsonWriter js;
      obj.jsonWrite( js );
      mObjectRef.insert( QString(key), js.object() );
      }


    //!
    //! \brief jsonObjectPtr Template transfer any value as json object
    //!                      Value class must contains jsonWrite method, which returns
    //!                      json object for value object
    //! \param key           Key for object
    //! \param objPtr        Object pointer to transfer
    //!
    template<typename SvClass>
    void jsonObjectPtr( const char *key, const SvClass *objPtr )
      {
      SvJsonWriter js;
      objPtr->jsonWrite( js );
      mObjectRef.insert( QString(key), js.object() );
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
    //!
    void jsonBool( const char *title, bool &b ) { b = mObject.value( QString(title) ).toBool(); }
    void jsonBool( const char *title, bool &b, bool def ) { b = mObject.value( QString(title) ).toBool( def ); }


    //!
    //! \brief jsonInt Transfer int value
    //! \param key     Key for value
    //! \param v       Int value
    //!
    void jsonInt( const char *key, int &v ) { v = mObject.value( QString(key) ).toInt(); }
    void jsonInt( const char *key, int &v, int def ) { v = mObject.value( QString(key) ).toInt( def ); }


    //!
    //! \brief jsonInt Transfer color value
    //! \param key     Key for value
    //! \param v       Int value
    //!
    void jsonColor( const char *key, QColor &color ) { color = QColor( mObject.value( QString(key) ).toString() ); }
    void jsonColor( const char *key, QColor &color, QColor def ) { color = QColor( mObject.value( QString(key) ).toString( def.name() ) ); }


    //!
    //! \brief jsonDouble Transfer double value
    //! \param key        Key for value
    //! \param d          Double value
    //!
    void jsonDouble( const char *key, double &d ) { d = mObject.value( QString(key) ).toDouble(); }
    void jsonDouble( const char *key, double &d, double def ) { d = mObject.value( QString(key) ).toDouble( def ); }


    //!
    //! \brief jsonString Transfer string value
    //! \param key        Key for value
    //! \param s          String to transfer
    //!
    void jsonString( const char *key, QString &s ) { s = mObject.value( QString(key) ).toString(); }
    void jsonString( const char *key, QString &s, QString def ) { s = mObject.value( QString(key) ).toString( def ); }


    //!
    //! \brief jsonListInt Transfer list of int values
    //! \param key         Key for list
    //! \param list        List to transfer
    //!
    void jsonListInt( const char *key, QList<int> &list )
      {
      list.clear();
      QJsonArray ar = mObject.value( QString(key) ).toArray();
      list.reserve( ar.count() );
      for( auto i = ar.constBegin(); i != ar.constEnd(); i++ ) {
        int v = i->toInt();
        list.append( v );
        }
      }


    //!
    //! \brief jsonListString Transfer list of string values
    //! \param key            Key for list
    //! \param list           List to transfer
    //!
    void jsonListString( const char *key, QStringList &list )
      {
      list.clear();
      QJsonArray ar = mObject.value( QString(key) ).toArray();
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
    void jsonList( const char *key, QList<SvClass> &list )
      {
      list.clear();
      QJsonArray ar = mObject.value( QString(key) ).toArray();
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
    void jsonListPtr( const char *key, QList<SvClass*> &list )
      {
      qDeleteAll(list);
      list.clear();
      QJsonArray ar = mObject.value( QString(key) ).toArray();
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
    void jsonMapString( const char *key, QMap<QString,QString> &map )
      {
      map.clear();
      QJsonObject obj = mObject.value( QString(key) ).toObject();
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
    void jsonMapInt( const char *key, QMap<QString,int> &map )
      {
      map.clear();
      QJsonObject obj = mObject.value( QString(key) ).toObject();
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
    void jsonMap( const char *key, QMap<QString,SvClass> &map )
      {
      map.clear();
      QJsonObject obj = mObject.value( QString(key) ).toObject();
      for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
        SvClass item;
        SvJsonReader js( i.value().toObject() );
        item.jsonRead( js );
        map.insert( i.key(), item );
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
    void jsonMapPtr( const char *key, QMap<QString,SvClass*> &map )
      {
      qDeleteAll(map);
      map.clear();
      QJsonObject obj = mObject.value( QString(key) ).toObject();
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
    void jsonObject( const char *key, SvClass &obj )
      {
      SvJsonReader js( mObject.value( QString(key) ).toObject() );
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
    void jsonObjectPtr( const char *key, SvClass *objPtr )
      {
      SvJsonReader js( mObject.value( QString(key) ).toObject() );
      objPtr->jsonRead( js );
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
    SvJsonReaderExt( const QJsonObject &obj, SvProperty *prop = nullptr ) : SvJsonReader(obj), mProperty(prop) {}

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
    void jsonList( const char *key, QList<SvClass> &list )
      {
      list.clear();
      QJsonArray ar = mObject.value( QString(key) ).toArray();
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
    void jsonListPtr( const char *key, QList<SvClass*> &list )
      {
      qDeleteAll(list);
      list.clear();
      QJsonArray ar = mObject.value( QString(key) ).toArray();
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
    void jsonMap( const char *key, QMap<QString,SvClass> &map )
      {
      map.clear();
      QJsonObject obj = mObject.value( QString(key) ).toObject();
      for( auto i = obj.constBegin(); i != obj.constEnd(); i++ ) {
        SvClass item;
        SvJsonReaderExt<SvProperty> js( i.value().toObject(), mProperty );
        item.jsonRead( js );
        map.insert( i.key(), item );
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
    void jsonMapPtr( const char *key, QMap<QString,SvClass*> &map )
      {
      qDeleteAll(map);
      map.clear();
      QJsonObject obj = mObject.value( QString(key) ).toObject();
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
    void jsonObject( const char *key, SvClass &obj )
      {
      SvJsonReaderExt<SvProperty> js( mObject.value( QString(key) ).toObject(), mProperty );
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
    void jsonObjectPtr( const char *key, SvClass *objPtr )
      {
      SvJsonReaderExt<SvProperty> js( mObject.value( QString(key) ).toObject(), mProperty );
      objPtr->jsonRead( js );
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
