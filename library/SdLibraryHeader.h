/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  All variable object saved in library. It are cad objects, translate strings, hierarhy system and so on
  Every object have its unical id (hash). With its hash we store reference to header - small object portion and
  to object - main object body.

  All information saved in three files: references file, headers file and objects file. References file contains
  map of all known objects with reference to its header and object.

  This is Header class. Header is a short object description. When sincronise with remote servers
  always doing reading id's with headers. Object itself reading on demand.
*/
#ifndef SDLIBRARYHEADER_H
#define SDLIBRARYHEADER_H

#include "SdStringMap.h"

#include <QString>
#include <QMap>
#include <QDataStream>
#include <QList>

#define sdUidDelimiter QChar('\r')

//Create uid from type, object name and author name
inline QString headerUid( QString type, QString name, QString author )
  {
  return type + sdUidDelimiter + name + sdUidDelimiter + author;
  }


//Extract variant from variant table
inline void sdExtractVariant( int index, SdStringMap &map, int fieldCount, const QStringList &table ) {
  //If index great than 0 then append variant params
  if( index > 0 ) {
    //Offset variant params for index
    int off = index * fieldCount;
    for( int i = 0; i < fieldCount; i++ )
      map.insert( table.at(i), table.at( off + i ) );
    }
  }




//Library object header
struct SdLibraryHeader
  {
    QString     mName;               //Name of stored object
    QString     mType;               //Type of stored object
    QString     mAuthor;             //Author who create object
    qint32      mTime;               //Object time creation
    quint64     mClass;              //Object class. When equals 0 then corresponded object is deleted
    QString     mInherit;            //Inheritance object

    SdStringMap mParamTable;         //User defined object params

    //Extended param table
    qint32      mVariantFieldCount; //Variable fields count in extended param table
    QStringList mVariantTable;      //Variant table

    SdLibraryHeader() : mName(), mType(), mAuthor(), mTime(0), mClass(0), mParamTable(), mVariantFieldCount(0),mVariantTable() {}

    void    write( QDataStream &os ) const { os << mName << mType << mAuthor << mTime << mClass << mInherit << mParamTable << mVariantFieldCount << mVariantTable; }

    void    read( QDataStream &is ) { is >> mName >> mType >> mAuthor >> mTime >> mClass >> mInherit >> mParamTable >> mVariantFieldCount >> mVariantTable; }

    QString uid() const { return headerUid( mType, mName, mAuthor ); }

    bool    isDeleted() const { return mClass == 0; }

    void    setDeleted() { mClass = 0; mTime++; }

    //Extended param access
    bool    variantTableExist() const { return mVariantFieldCount != 0; }
    int     variantCount() const { return mVariantTable.count() / mVariantFieldCount; }
    void    variant( SdLibraryHeader &hdr, int index ) {
      //Build variant "index" in header "hdr"
      //When index == 0 then builded base variant, i.e. that in mParamTable
      //Write base variant
      hdr.mName       = mName;
      hdr.mType       = mType;
      hdr.mAuthor     = mAuthor;
      hdr.mTime       = mTime;
      hdr.mClass      = mClass;
      hdr.mInherit    = mInherit;
      hdr.mParamTable = mParamTable;
      hdr.mVariantFieldCount = 0;
      hdr.mVariantTable.clear();
      sdExtractVariant( index, hdr.mParamTable, mVariantFieldCount, mVariantTable );
      }

  };

inline QDataStream &operator << ( QDataStream &os, const SdLibraryHeader &header ) {
  header.write( os );
  return os;
  }

inline QDataStream &operator >> ( QDataStream &is, SdLibraryHeader &header ) {
  header.read( is );
  return is;
  }

//Headers list
typedef QList<SdLibraryHeader> SdLibraryHeaderList;

#endif // SDLIBRARYHEADER_H
