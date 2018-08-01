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

//Create uid from type, object name and author name
inline QString headerUid( QString type, QString name, QString author )
  {
  return type + name + author;
  }

//Library object header
struct SdLibraryHeader
  {
    QString     mName;       //Name of stored object
    QString     mType;       //Type of stored object
    QString     mAuthor;     //Author who create object
    QString     mTag;        //Object's tags
    qint32      mTime;       //Object time creation
    quint64     mClass;      //Object class
    QString     mInherit;    //Inheritance object

    SdStringMap mParamTable; //User defined object params

    SdLibraryHeader() : mName(), mType(), mAuthor(), mTime(0), mClass(0), mParamTable() {}

    void    write( QDataStream &os ) const { os << mName << mType << mAuthor << mTag << mTime << mClass << mInherit << mParamTable; }

    void    read( QDataStream &is ) { is >> mName >> mType >> mAuthor >> mTag >> mTime >> mClass >> mInherit >> mParamTable; }

    QString uid() const { return headerUid( mType, mName, mAuthor ); }
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
