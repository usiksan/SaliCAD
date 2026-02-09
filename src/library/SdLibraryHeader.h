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
#include "objects/SdFileUid.h"

#include <QString>
#include <QMap>
#include <QDataStream>
#include <QList>

#define sdUidDelimiter QChar('\r')

//Create uid from type, object name and author name
inline QString headerUid( QString type, QString name, QString authorKey )
  {
  return type + sdUidDelimiter + name + sdUidDelimiter + authorKey;
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
    QString     mName;              //!< Name of stored object
    QString     mType;              //!< Type of stored object
    QString     mAuthorKey;         //!< Public Key of Author who create object
    SdFileUid   mFileUid;           //!< File unical id as combination hash of unical object name and time of object creation
    quint64     mClass;             //!< Object class. When equals 0 then corresponded object is deleted
    bool        mPartPresent;       //!< Flag of part present in component or symbol
    bool        mIsPublic;          //!< True for public objects

    SdStringMap mParamTable;        //!< User defined object params

    //Extended param table
    qint32      mVariantFieldCount; //!< Variable fields count in extended param table
    QStringList mVariantTable;      //!< Variant table

    //!
    //! \brief SdLibraryHeader Default constructor
    //!
    SdLibraryHeader() : mName(), mType(), mAuthorKey(), mClass(0), mPartPresent(false), mIsPublic(false), mParamTable(), mVariantFieldCount(0),mVariantTable() {}

    //!
    //! \brief write         Write object header to data stream
    //! \param os            Output data stream
    //!
    void    write( QDataStream &os ) const { os << mName << mType << mAuthorKey << mFileUid << mClass << mPartPresent << mIsPublic << mParamTable << mVariantFieldCount << mVariantTable; }

    //!
    //! \brief read          Read object header from data stream
    //! \param is            Input data stream
    //!
    void    read( QDataStream &is ) { is >> mName >> mType >> mAuthorKey >> mFileUid >> mClass >> mPartPresent >> mIsPublic >> mParamTable >> mVariantFieldCount >> mVariantTable; }

    //!
    //! \brief hashUidName   Return hash of unical object name
    //! \return              Hash UID name string
    //!
    QString hashUidName() const { return mFileUid.mHashUidName; }

    //!
    //! \brief isPublic      Check if object is public
    //! \return              True if object is public
    //!
    bool    isPublic() const { return mIsPublic; }

    //!
    //! \brief isDeleted     Check if object is marked as deleted
    //! \return              True if object class equals 0 (deleted)
    //!
    bool    isDeleted() const { return mClass == 0; }

    //!
    //! \brief setDeleted    Mark object as deleted by setting class to 0 and incrementing creation time
    //!
    void    setDeleted() { mClass = 0; mFileUid.mCreateTime++; }

    //==============================================================
    //        Extended param access
    //!
    //! \brief variantTableExist Check if variant table exists (has variable fields)
    //! \return                  True if variant table has fields
    //!
    bool    variantTableExist() const { return mVariantFieldCount != 0; }


    //!
    //! \brief variantCount      Get count of variants in variant table
    //! \return                  Number of variants
    //!
    int     variantCount() const { return mVariantTable.count() / mVariantFieldCount; }


    //!
    //! \brief variant           Build specified variant into provided header
    //! \param hdr               Target header to receive variant data
    //! \param index             Variant index (0 for base variant from mParamTable)
    //!
    void    variant( SdLibraryHeader &hdr, int index ) {
      //Build variant "index" in header "hdr"
      //When index == 0 then builded base variant, i.e. that in mParamTable
      //Write base variant
      hdr.mName        = mName;
      hdr.mType        = mType;
      hdr.mAuthorKey   = mAuthorKey;
      hdr.mFileUid     = mFileUid;
      hdr.mClass       = mClass;
      hdr.mPartPresent = mPartPresent;
      hdr.mIsPublic    = mIsPublic;
      hdr.mParamTable  = mParamTable;
      hdr.mVariantFieldCount = 0;
      hdr.mVariantTable.clear();
      sdExtractVariant( index, hdr.mParamTable, mVariantFieldCount, mVariantTable );
      }

    //!
    //! \brief authorGlobalName Return global author name associated with mAuthorKey (public author key)
    //! \return                 Global author name associated with mAuthorKey (public author key)
    //!
    QString authorGlobalName() const;
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
