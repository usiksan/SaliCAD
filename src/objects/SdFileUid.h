#ifndef SDFILEUID_H
#define SDFILEUID_H

#include "SdConfig.h"
#include "SdUtil.h"
#include "library/SdLibraryServerDef.h"

#include <QString>
#include <QDataStream>


struct SdFileUid
  {
    QString mHashUidName; //!< Hash for unical name of object as combination object type, object title and objects author public key
    qint32  mCreateTime;  //!< Create time with sec from 2000year

    //!
    //! \brief SdFileUid   Default constructor
    //!
    SdFileUid() : mCreateTime(0) {}

    //!
    //! \brief SdFileUid   Parameterized constructor
    //! \param hashUidName Unique hash string identifying the object
    //! \param createTime  Creation timestamp in seconds since year 2000
    //!
    SdFileUid( const QString &hashUidName, int createTime ) : mHashUidName(hashUidName), mCreateTime(createTime) {}

    //!
    //! \brief SdFileUid   Constructor that parses a full file UID string
    //! \param fileUid     The complete file UID string in format "hash-timestamp.sdb"
    //!
    SdFileUid( const QString &fileUid ) : mCreateTime(0) { fromFileUid(fileUid); }

    //!
    //! \brief fromFileUid Parses a file UID string and extracts hash and timestamp components
    //! \param fileUid     The file UID string to parse (expected format: 27-char-hash + "-" + 8-char-hex-timestamp + ".sdb")
    //! \return            True if parsing succeeded, false if format is invalid
    //!
    bool    fromFileUid( const QString &fileUid )
      {
      //Testing path is correct
      if( fileUid.contains( QChar('.') ) ) {
        //File name with extension
        if( fileUid.size() != (SD_HASH_UID_NAME_LEN + 9 + SD_BINARY_EXTENSION_LENGTH) || !fileUid.endsWith(SD_BINARY_EXTENSION) || fileUid.at(SD_HASH_UID_NAME_LEN) != QChar('-') )
          return false;
        }
      else {
        //File name without extension
        if( fileUid.size() != (SD_HASH_UID_NAME_LEN + 9) || fileUid.at(SD_HASH_UID_NAME_LEN) != QChar('-') )
          return false;
        }
      mHashUidName = fileUid.mid( 0, SD_HASH_UID_NAME_LEN );
      mCreateTime  = fileUid.mid( SD_HASH_UID_NAME_LEN + 1, 8 ).toInt( nullptr, 16 );
      return true;
      }

    //!
    //! \brief fileUid     Generates the complete file UID string from current object state
    //! \return            Full file UID in format "hash-timestamp.sdb"
    //!
    QString fileUid() const { return fileUid( mHashUidName, mCreateTime ); }

    //!
    //! \brief isEqualName Compares if two file UIDs have the same hash name (ignores timestamp)
    //! \param uid         The other SdFileUid object to compare with
    //! \return            True if hash names are identical, false otherwise
    //!
    bool    isEqualName( const SdFileUid &uid ) const { return mHashUidName == uid.mHashUidName; }

    //!
    //! \brief isOlderThen Checks if this object is older than another based on creation time
    //! \param uid         The other SdFileUid object to compare with
    //! \return            True if this object's createTime is less than the other's
    //!
    bool    isOlderThen( const SdFileUid &uid ) const { return mCreateTime < uid.mCreateTime; }

    //!
    //! \brief isOlderOrEqualThen Checks if this object is older or equal to another based on creation time
    //! \param uid                The other SdFileUid object to compare with
    //! \return                   True if this object's createTime is less than or equal to the other's
    //!
    bool    isOlderOrEqualThen( const SdFileUid &uid ) const { return mCreateTime <= uid.mCreateTime; }


    void    swap( SdFileUid &src )
      {
      src.mHashUidName.swap( mHashUidName );
      SdUtil::swap( src.mCreateTime, &mCreateTime );
      }

    //!
    //! \brief fileUid     Builds hash uid code for file name of object on base unical hash code of name and version
    //! \param hashUidName Unical hash code of object name
    //! \param createTime  Object version (object time creation)
    //! \return            Full file name for object
    //!
    static QString fileUid( const QString hashUidName, int createTime )
      {
      return hashUidName + QString( "-%1" SD_BINARY_EXTENSION ).arg( createTime, 8, 16, QChar('0') );
      }
  };


//Write function
inline QDataStream &operator << ( QDataStream &os, const SdFileUid &uid )
  {
  os << uid.mHashUidName << uid.mCreateTime;
  return os;
  }


//Read function
inline QDataStream &operator >> ( QDataStream &is, SdFileUid &uid )
  {
  is >> uid.mHashUidName >> uid.mCreateTime;
  return is;
  }


#endif // SDFILEUID_H
