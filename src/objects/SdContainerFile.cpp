#include "SdConfig.h"
#include "SdContainerFile.h"
#include "SdUtil.h"
#include "library/SdLibraryStorage.h"
#include "SvLib/SvTime2x.h"
#include "SdJsonIO.h"

#include <QSettings>
#include <QCryptographicHash>


SdContainerFile::SdContainerFile() :
  SdContainer(),
  mCreateTime(0),
  mEditEnable(true),
  mThereNewer(false)
  {

  }





//!
//! \brief getUidName Builds world unical name of object  as combination object type, object title and objects author public key
//!                   for previous version of SaliCAD
//! \return           Worlds unical name of object
//!
QString SdContainerFile::getUidName() const
  {
  //Id consist from name, user and time creation
  return headerUid( getType(), mTitle, mAuthorKey );
  }










QString SdContainerFile::getExtendTitle() const
  {
  return QString("%1 [r%2] (%3)").arg( mTitle, SvTime2x::toLocalString(getTime()), authorGlobalName( mAuthorKey ) );
  }



void SdContainerFile::getHeader(SdLibraryHeader &hdr) const
  {
  hdr.mName        = mTitle;
  hdr.mType        = getType();
  hdr.mAuthorKey   = mAuthorKey;
  hdr.mHashUidName = mHashUidName;
  hdr.mTime        = getTime();
  hdr.mClass       = getClass();
  hdr.mParamTable  = mParamTable;
  }





bool SdContainerFile::isAnotherAuthor() const
  {
  return mAuthorKey != getDefaultAuthor();
  }





//!
//! \brief titleSet Sets new title for object
//! \param title    New title for object
//!
void SdContainerFile::titleSet(const QString title)
  {
  //Title setup
  mTitle      = title;
  //Update creation time
  updateCreationTime();
  //Item author (registered program copy name)
  updateAuthorAndHash();
  }





//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void SdContainerFile::json(SdJsonWriter &js) const
  {
  js.jsonString( QStringLiteral("Title"),       mTitle );
  js.jsonString( QStringLiteral("AuthorKey"),   mAuthorKey );
  js.jsonString( QStringLiteral("HashUidName"), mHashUidName );
  js.jsonInt(  QStringLiteral("Created"),       mCreateTime );
  js.jsonBool( QStringLiteral("Edit enable"),   mEditEnable );
  SdContainer::json( js );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void SdContainerFile::json(const SdJsonReader &js)
  {
  js.jsonString( QStringLiteral("Title"),       mTitle );
  js.jsonInt(  QStringLiteral("Created"),       mCreateTime );
  js.jsonBool( QStringLiteral("Edit enable"),   mEditEnable, true );
  if( js.property()->mVersion == SD_BASE_VERSION_2 ) {
    //Convert previous author into current and build hash
    updateAuthorAndHash();
    }
  else {
    js.jsonString( QStringLiteral("AuthorKey"),   mAuthorKey );
    js.jsonString( QStringLiteral("HashUidName"), mHashUidName );
    }
  SdContainer::json( js );
  }




//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdContainerFile::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdContainer::cloneFrom( src, copyMap, next );
  SdPtrConst<SdContainerFile> sour(src);
  if( sour.isValid() ) {
    mTitle       = sour->mTitle;
    mAuthorKey   = sour->mAuthorKey;
    mHashUidName = sour->mHashUidName;
    mCreateTime  = sour->mCreateTime;
    mEditEnable  = sour->mEditEnable;
    }
  }




//Return current registered author
QString SdContainerFile::getDefaultAuthor()
  {
  static QString cachePrivateKey;
  static QString cacheAuthorKey;
  QSettings s;
  if( s.value( SDK_PRIVATE_KEY ).toString() != cachePrivateKey ) {
    cachePrivateKey = s.value( SDK_PRIVATE_KEY ).toString();
    QByteArray hash = QCryptographicHash::hash( cachePrivateKey.toUtf8(), QCryptographicHash::Sha3_256 );
    cacheAuthorKey = QString::fromUtf8( hash.toHex() ).first( 16 );
    }
  return cacheAuthorKey;
  }






//!
//! \brief hashUidName Builds hash code for unical name of object which consists from three parts
//! \param objectType  Object type
//! \param objectTitle Object title
//! \param authorKey   Author for object
//! \return            hex hash code
//!
QString SdContainerFile::hashUidName(const QString &objectType, const QString &objectTitle, const QString &authorKey)
  {
  QString unicalName( objectType + objectTitle + authorKey );
  QByteArray hash = QCryptographicHash::hash( unicalName.toUtf8(), QCryptographicHash::Sha3_256 );
  return QString::fromUtf8( hash.toHex() );
  }





//!
//! \brief hashUidFile Builds hash uid code for file name of object on base unical hash code of name and version
//! \param hashUidName Unical hash code of object name
//! \param timeVersion Object version (object time creation)
//! \return            Full file name for object
//!
QString SdContainerFile::hashUidFile(const QString &hashUidName, int timeVersion)
  {
  return hashUidName + QString( "-%1" SD_BINARY_EXTENSION ).arg( timeVersion, 8, 16, QChar('0') );
  }





//!
//! \brief authorGlobalName Retrive author global name if registered, otherwise return "anonim" or "yourown"
//! \param publicAuthorKey  Public author key
//! \return                 Author global name
//!
QString SdContainerFile::authorGlobalName(const QString &publicAuthorKey)
  {

  }






//On call this function time setup after previous time
void SdContainerFile::updateCreationTime()
  {
  int time = SvTime2x::current();
  if( time <= mCreateTime )
    mCreateTime++;
  else
    mCreateTime = time;
  }




//!
//! \brief updateAuthorAndHash Set author as current and calculate new hash
//!
void SdContainerFile::updateAuthorAndHash()
  {
  //Set author as current
  mAuthorKey = getDefaultAuthor();
  //calculate new hash
  mHashUidName = hashUidName( getType(), mTitle, mAuthorKey );
  }



