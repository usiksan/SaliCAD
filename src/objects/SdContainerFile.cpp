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


QString SdContainerFile::getUid() const
  {
  //Id consist from name, user and time creation
  return headerUid( getType(), mTitle, mAuthorKey );
  }


//!
//! \brief crc  Calculates crc code of line which is hash code for unical file name
//! \param line
//! \return
//!
//! Previous realisation use qHash, but with change qt version algorithm
//! of calculation - changed. To fix up this problem we release our owner crc calculation
static unsigned short crc( const QString &line )
  {
  const ushort *ptr = line.utf16();
  int len = line.length();
  unsigned short h = 0;
  //(h << 5) + h ^ c
  while( len ) {
    h = (h << 5) + (h ^ *ptr);
    ptr++;
    len--;
    }
  return h;
  }



//!
//! \brief getLibraryFName Builds and return file name for library storage from full uid and time of contaner file
//! \param fullUid         Full uid of container file
//! \param time            Time of containter file
//! \return                File name for library storage
//!
QString SdContainerFile::getLibraryFName( const QString &fullUid, qint32 time )
  {
  QString uid( fullUid );
  if( uid.length() > 64 )
    uid = uid.left(64);
  for( int i = 0; i < uid.length(); i++ )
    if( !uid.at(i).isLetterOrNumber() )
      uid[i] = QChar('_');
  QString hash( QStringLiteral("%1").arg( crc(fullUid), 4, 16, QChar('0') ) );
  return QStringLiteral("%1/%2-%3-%4" SD_BINARY_EXTENSION ).arg( hash.left(2), hash.right(2), uid ).arg( time, 0, 16 );
  }




//!
//! \brief getLibraryFName Build and return file name for library storage
//! \return                File name for library storage
//!
QString SdContainerFile::getLibraryFName() const
  {
  return getLibraryFName( getUid(), getTime() );
  }








QString SdContainerFile::getExtendTitle() const
  {
  return QString("%1 [r%2] (%3)").arg( mTitle, SvTime2x::toLocalString(getTime()), mAuthorKey );
  }



void SdContainerFile::getHeader(SdLibraryHeader &hdr) const
  {
  hdr.mName       = mTitle;
  hdr.mType       = getType();
  hdr.mAuthorKey  = mAuthorKey;
  hdr.mTime       = getTime();
  hdr.mClass      = getClass();
  hdr.mParamTable = mParamTable;
  }





bool SdContainerFile::isAnotherAuthor() const
  {
  return mAuthorKey != getDefaultAuthor();
  }





void SdContainerFile::titleSet(const QString title)
  {
  //Item author (registered program copy name)
  updateAuthor();
  //Update creation time
  updateCreationTime();
  //Title setup
  mTitle      = title;
  }





//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void SdContainerFile::json(SdJsonWriter &js) const
  {
  js.jsonString( QStringLiteral("Title"),     mTitle );
  js.jsonString( QStringLiteral("Author"),    mAuthorKey );
  js.jsonInt(  QStringLiteral("Created"),     mCreateTime );
  js.jsonBool( QStringLiteral("Edit enable"), mEditEnable );
  SdContainer::json( js );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void SdContainerFile::json(const SdJsonReader &js)
  {
  js.jsonString( QStringLiteral("Title"),     mTitle );
  js.jsonString( QStringLiteral("Author"),    mAuthorKey );
  js.jsonInt(  QStringLiteral("Created"),     mCreateTime );
  js.jsonBool( QStringLiteral("Edit enable"), mEditEnable, true );
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
    mTitle      = sour->mTitle;
    mAuthorKey     = sour->mAuthorKey;
    mCreateTime = sour->mCreateTime;
    mEditEnable = sour->mEditEnable;
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






//On call this function time setup after previous time
void SdContainerFile::updateCreationTime()
  {
  int time = SvTime2x::current();
  if( time <= mCreateTime )
    mCreateTime++;
  else
    mCreateTime = time;
  }




void SdContainerFile::updateAuthor()
  {
  mAuthorKey = getDefaultAuthor();
  }



