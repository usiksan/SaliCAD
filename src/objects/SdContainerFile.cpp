#include "SdConfig.h"
#include "SdContainerFile.h"
#include "library/SdLibraryStorage.h"
#include "SvLib/SvTime2x.h"
#include "SdJsonIO.h"

#include <QSettings>
#include <QCryptographicHash>


SdContainerFile::SdContainerFile() :
  SdContainer(),
  mFileUid(),
  mEditEnable(true),
  mIsPublic(false),
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
  return QString("%1 [%4%2] (%3)").arg( mTitle, SvTime2x::toLocalString(getTime()), authorGlobalName( mAuthorKey ), isPublic() ? QString("🌐") : QString("") );
  }



void SdContainerFile::getHeader(SdLibraryHeader &hdr) const
  {
  hdr.mName        = mTitle;
  hdr.mType        = getType();
  hdr.mAuthorKey   = mAuthorKey;
  hdr.mFileUid     = mFileUid;
  hdr.mClass       = getClass();
  hdr.mParamTable  = mParamTable;
  hdr.mIsPublic    = mIsPublic;
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
  mIsPublic   = false;
  //Update creation time
  updateCreationTime();
  //Item author (registered program copy name)
  updateAuthorAndHash();
  }




//!
//! \brief publicSet Mark object as public
//!
void SdContainerFile::publicSet()
  {
  if( !isAnotherAuthor() ) {
    //Public mark enabled only for author
    mIsPublic = true;
    //Update creation time
    updateCreationTime();
    //Write object to local library
    SdLibraryStorage::instance()->cfObjectInsert( this );
    }
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
  js.jsonString( QStringLiteral("HashUidName"), mFileUid.mHashUidName );
  js.jsonInt(  QStringLiteral("Created"),       mFileUid.mCreateTime );
  js.jsonBool( QStringLiteral("Edit enable"),   mEditEnable );
  js.jsonBool( QStringLiteral("Is public"),     mIsPublic );
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
  js.jsonInt(  QStringLiteral("Created"),       mFileUid.mCreateTime );
  js.jsonBool( QStringLiteral("Edit enable"),   mEditEnable, true );
  if( js.property()->mVersion == SD_BASE_VERSION_2 ) {
    //Convert previous author into current and build hash
    updateAuthorAndHash();
    mIsPublic = false;
    }
  else {
    js.jsonString( QStringLiteral("AuthorKey"),   mAuthorKey );
    js.jsonString( QStringLiteral("HashUidName"), mFileUid.mHashUidName );
    js.jsonBool( QStringLiteral("Is public"),     mIsPublic );
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
    mFileUid     = sour->mFileUid;
    mEditEnable  = sour->mEditEnable;
    mIsPublic    = sour->mIsPublic;
    }
  }




//Return current registered author
QString SdContainerFile::getDefaultAuthor()
  {
  return SdLibraryStorage::authorPublicKey();
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
  QByteArray hash = QCryptographicHash::hash( unicalName.toUtf8(), QCryptographicHash::Blake2s_128 );

  //Convert binary hash to string
  static const char alphabet[] = "0123456789abcdefghijklmnopqrstuv";

  QByteArray result;

  //Prefix
  result.append('x');

  //First symbol - 3bit for subdir name
  int buffer = (quint8)hash[0];
  int bits = 5;
  result.append(alphabet[(buffer >> bits) & 0x1F]);

  // Обрабатываем 16 байт
  for( int i = 1; i < 16; i++ ) {
    buffer = (buffer << 8) | (quint8)hash[i];
    bits += 8;

    while( bits >= 5 ) {
      bits -= 5;
      result.append(alphabet[(buffer >> bits) & 0x1F]);
      }
    }

  if( result.size() != SD_HASH_UID_NAME_LEN )
    qFatal() << "Hash uid name generator length not equal to global SD_HASH_UID_NAME_LEN";

  return QString::fromLatin1(result);
  }







//!
//! \brief authorGlobalName Retrive author global name if registered, otherwise return "anonim" or "yourown"
//! \param publicAuthorKey  Public author key
//! \return                 Author global name
//!
QString SdContainerFile::authorGlobalName(const QString &publicAuthorKey)
  {
  return SdLibraryStorage::instance()->authorGlobalName(publicAuthorKey);
  }






//On call this function time setup after previous time
void SdContainerFile::updateCreationTime()
  {
  int time = SvTime2x::current();
  if( time <= mFileUid.mCreateTime )
    mFileUid.mCreateTime++;
  else
    mFileUid.mCreateTime = time;
  }




//!
//! \brief updateAuthorAndHash Set author as current and calculate new hash
//!
void SdContainerFile::updateAuthorAndHash()
  {
  //Set author as current
  mAuthorKey = getDefaultAuthor();
  //calculate new hash
  mFileUid.mHashUidName = hashUidName( getType(), mTitle, mAuthorKey );
  }



