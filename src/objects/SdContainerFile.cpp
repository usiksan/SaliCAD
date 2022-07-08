#include "SdContainerFile.h"
#include "SdUtil.h"
#include "SvLib/SvTime2x.h"
#include "SdJsonIO.h"

#include <QSettings>


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
  return headerUid( getType(), mTitle, mAuthor );
  }








QString SdContainerFile::getExtendTitle() const
  {
  return QString("%1 [r%2] (%3)").arg(mTitle).arg( SvTime2x::toLocalString(getTime()) ).arg(mAuthor);
  }



void SdContainerFile::getHeader(SdLibraryHeader &hdr) const
  {
  hdr.mName       = mTitle;
  hdr.mType       = getType();
  hdr.mAuthor     = mAuthor;
  hdr.mTime       = getTime();
  hdr.mClass      = getClass();
  hdr.mParamTable = mParamTable;
  }





bool SdContainerFile::isAnotherAuthor() const
  {
  return mAuthor != getDefaultAuthor();
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
  js.jsonString( QStringLiteral("Author"),    mAuthor );
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
  js.jsonString( QStringLiteral("Author"),    mAuthor );
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
    mAuthor     = sour->mAuthor;
    mCreateTime = sour->mCreateTime;
    mEditEnable = sour->mEditEnable;
    }
  }




//Return current registered author
QString SdContainerFile::getDefaultAuthor()
  {
  QSettings s;
  return s.value( SDK_GLOBAL_AUTHOR ).toString();
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
  mAuthor = getDefaultAuthor();
  }



